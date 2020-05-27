#include "sysinit/sysinit.h"
#include "os/os.h"
#include "os/os_mempool.h"
#include "shell/shell.h"

#include "host/ble_hs.h"
#include "host/ble_gap.h"

#include "expcfg/expcfg.h"
#include "seqstats/seqstats.h"


struct flood_evt {
    struct os_event e;
    unsigned size;
    unsigned num;
};


/* BLE connection state */
static uint16_t _handle = 0;
static struct ble_l2cap_chan *_coc = NULL;

/* buffer allocation */
static uint8_t _txbuf[MYNEWT_VAL(EXPCFG_MTU)];

static struct os_eventq _eq;
static struct flood_evt _evt;

static struct os_task _ftask;
static os_stack_t _fstack[MYNEWT_VAL(APP_STACKSIZE)];

static struct os_mutex _txlock;


static int _flood_cmd_func(int argc, char **argv)
{
    if (argc < 3) {
        printf("usage: %s <chunksize> <num>\n", argv[0]);
        return 1;
    }
    _evt.size =(unsigned)atoi(argv[1]);
    _evt.num = (unsigned)atoi(argv[2]);

    os_eventq_put(&_eq, &_evt.e);

    return 0;
}

static void _send(uint8_t type, uint8_t seq, size_t len)
{
    int res = 0;
    (void)res;
    struct os_mbuf *txd;

    _txbuf[0] = seq;
    txd = expcfg_getbuf();
    res = os_mbuf_append(txd, _txbuf, len);
    assert(res == 0);

    do {
        res = ble_l2cap_send(_coc, txd);
        if (res == BLE_HS_EBUSY) {
            os_mutex_pend(&_txlock, OS_TIMEOUT_NEVER);
        }
    } while (res == BLE_HS_EBUSY);

    if ((res != 0) && (res != BLE_HS_ESTALLED)) {
        printf("# err: failed to send (%i)\n", res);
        assert(0);
    }
}

static void _flood(struct os_event *ev)
{
    struct flood_evt *fl = (struct flood_evt *)ev;

    assert(_coc);
    assert(fl->size >= 2);
    assert(fl->size <= MYNEWT_VAL(EXPCFG_MTU));

    printf("# Flooding size:%u num:%u\n", fl->size, fl->num);
    for (unsigned i = 0; i < fl->num; i++) {
        _send(0, (uint8_t)i, (size_t)fl->size);
        seqstats_inc(SEQSTATS_APP_TX);
    }

    printf("# Flooding finished, all good.\n");
}

static void _flood_task(void *arg)
{
    (void)arg;
    while (1) {
        os_eventq_run(&_eq);
    }
}

static void _on_data(struct ble_l2cap_event *event)
{
    int res;
    (void)res;

    /* assigne new receive buffer right away */
    struct os_mbuf *rxnew = expcfg_getbuf();
    assert(rxnew != NULL);
    res = ble_l2cap_recv_ready(_coc, rxnew);
    assert(res == 0);

    /* ignore the incoming data */
    struct os_mbuf *rxd = event->receive.sdu_rx;
    res = os_mbuf_free_chain(rxd);
    assert(res == 0);
}

static int _on_l2cap_evt(struct ble_l2cap_event *event, void *arg)
{
    (void)arg;

    switch (event->type) {
        case BLE_L2CAP_EVENT_COC_CONNECTED:
            _coc = event->connect.chan;
            puts("# L2CAP: CONNECTED");
            break;
        case BLE_L2CAP_EVENT_COC_DISCONNECTED:
            _coc = NULL;
            puts("# L2CAP: DISCONNECTED");
            break;
        case BLE_L2CAP_EVENT_COC_DATA_RECEIVED:
            _on_data(event);
            break;
        case BLE_L2CAP_EVENT_COC_TX_UNSTALLED:
            os_mutex_release(&_txlock);
            break;
        case BLE_L2CAP_EVENT_COC_ACCEPT:
            /* this event should never be triggered for the L2CAP client */
            /* fallthrough */
        default:
            assert(0);
            break;
    }

    return 0;
}

static int _on_gap_evt(struct ble_gap_event *event, void *arg)
{
    (void)arg;
    printf("# GAP event: %i\n", (int)event->type);

    switch (event->type) {
        case BLE_GAP_EVENT_CONNECT: {
            _handle = event->connect.conn_handle;
            struct os_mbuf *sdu_rx = expcfg_getbuf();
            int res = ble_l2cap_connect(_handle, MYNEWT_VAL(EXPCFG_CID), MYNEWT_VAL(EXPCFG_MTU), sdu_rx,
                                       _on_l2cap_evt, NULL);
            assert(res == 0);
            (void)res;
            break;
        }
        case BLE_GAP_EVENT_DISCONNECT:
            _handle = 0;
            puts("# TERMINATED, bye bye");
            break;
        case BLE_GAP_EVENT_CONN_UPDATE:
            puts("# CONN update");
            break;
        case BLE_GAP_EVENT_CONN_UPDATE_REQ:
        default:
            break;
    }

    return 0;
}

static void _filter_and_connect(struct ble_gap_disc_desc *disc)
{
    int res;
    struct ble_hs_adv_fields fields;
    res = ble_hs_adv_parse_fields(&fields, disc->data,
                                 disc->length_data);
    if (res != 0) {
        return;
    }

    if ((fields.name != NULL) &&
        (memcmp(MYNEWT_VAL(EXPCFG_NODENAME), fields.name,
                strlen(MYNEWT_VAL(EXPCFG_NODENAME))) == 0)) {
        printf("# Found Server, connecting now\n");
        res = ble_gap_disc_cancel();
        assert(res == 0);

        res = ble_gap_connect(BLE_OWN_ADDR_RANDOM, &disc->addr,
                              BLE_HS_FOREVER, NULL, _on_gap_evt, NULL);
        assert(res == 0);
    }
}

static int _on_scan_evt(struct ble_gap_event *event, void *arg)
{
    (void)arg;

    switch (event->type) {
        case BLE_GAP_EVENT_DISC:
            printf("# Scan event\n");
            _filter_and_connect(&event->disc);
            break;
        case BLE_GAP_EVENT_DISC_COMPLETE:
        default:
            break;
    }

    return 0;
}

static void on_sync(void)
{
    int res;

    /* Generate a non-resolvable private address. */
    ble_addr_t addr;
    res = ble_hs_id_gen_rnd(1, &addr);
    assert(res == 0);
    res = ble_hs_id_set_rnd(addr.val);
    assert(res == 0);

    /* start scanning for a suitable test server */
    puts("# Scanning now");
    struct ble_gap_disc_params params = { 0 };
    res = ble_gap_disc(BLE_OWN_ADDR_RANDOM, BLE_HS_FOREVER,
                       &params, _on_scan_evt, NULL);
    assert(res == 0);
}

static struct shell_cmd _flood_cmd = {
    .sc_cmd = "flood",
    .sc_cmd_func = _flood_cmd_func,
};

int main(int argc, char **argv)
{
    int res;
    (void)res;

    printf("Experiment es0: SOURCE.\n");

    /* Initialize all packages. */
    sysinit();
    expcfg_init();

    /* setup worker thread and needed IPC stuff */
    os_eventq_init(&_eq);
    _evt.e.ev_cb = _flood;
    res = os_mutex_init(&_txlock);
    assert(res == 0);
    res = os_task_init(&_ftask, "flooder", _flood_task, NULL,
                       MYNEWT_VAL(APP_TASK_PRIO), OS_WAIT_FOREVER,
                       _fstack, MYNEWT_VAL(APP_STACKSIZE));
    assert(res == 0);

    /* register `flood` shell command */
    res = shell_cmd_register(&_flood_cmd);
    assert(res == 0);

    /* run stats module */
    seqstats_run();

    /* As the last thing, process events from default event queue. */
    printf("# Running eventq now\n");
    ble_hs_cfg.sync_cb = on_sync;
    while (1) {
        os_eventq_run(os_eventq_dflt_get());
    }

    return 0;
}

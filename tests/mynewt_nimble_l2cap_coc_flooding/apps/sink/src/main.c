#include "sysinit/sysinit.h"
#include "os/os.h"

#include "host/ble_hs.h"
#include "host/ble_gap.h"
#include "host/ble_hs_adv.h"

#include "expcfg/expcfg.h"
#include "seqstats/seqstats.h"

/* BLE connection state */
static uint16_t _handle = 0;
static struct ble_l2cap_chan *_coc = NULL;


static void _advertise_now(void);

static void _on_data(struct ble_l2cap_event *event)
{
    int res;
    (void)res;

    /* make sure there is room for new data right away */
    struct os_mbuf *rxnew = expcfg_getbuf();
    assert(rxnew != NULL);
    res = ble_l2cap_recv_ready(_coc, rxnew);
    assert(res == 0);

    seqstats_inc(SEQSTATS_APP_RX);

    /* look into the received data and set exp state accordingly */
    struct os_mbuf *rxd = event->receive.sdu_rx;
    assert(rxd != NULL);

    /* free receive buffer */
    res = os_mbuf_free_chain(rxd);
    assert(res == 0);
}


static int _on_gap_evt(struct ble_gap_event *event, void *arg)
{
    (void)arg;
    printf("# GAP event %i\n", (int)event->type);

    switch (event->type) {
        case BLE_GAP_EVENT_CONNECT:
            _handle = event->connect.conn_handle;
            break;
        case BLE_GAP_EVENT_DISCONNECT:
            _coc = NULL;
            _handle = 0;
            _advertise_now();
            break;
        case BLE_GAP_EVENT_CONN_UPDATE:
        case BLE_GAP_EVENT_CONN_UPDATE_REQ:
        default:
            break;
    }
    return 0;
}

static int _on_l2cap_evt(struct ble_l2cap_event *event, void *arg)
{
    (void)arg;
    struct ble_l2cap_chan_info info;

    switch (event->type) {
        case BLE_L2CAP_EVENT_COC_CONNECTED:
            _coc = event->connect.chan;
            puts("# L2CAP: CONNECTED");
            ble_l2cap_get_chan_info(_coc, &info);
            printf("# MTUs: our %i, remote %i\n", (int)info.our_l2cap_mtu,
                   (int)info.peer_l2cap_mtu);
            break;
        case BLE_L2CAP_EVENT_COC_DISCONNECTED:
            _coc = NULL;
            puts("# L2CAP: DISCONNECTED");
            break;
        case BLE_L2CAP_EVENT_COC_ACCEPT: {
            struct os_mbuf *sdu_rx = expcfg_getbuf();
            assert(sdu_rx != NULL);
            ble_l2cap_recv_ready(event->accept.chan, sdu_rx);
            break;
        }
        case BLE_L2CAP_EVENT_COC_DATA_RECEIVED:
            _on_data(event);
            break;
        case BLE_L2CAP_EVENT_COC_TX_UNSTALLED:
            /* this event is expected, but we have nothing to do here */
            break;
        default:
            assert(0);
            break;
    }

    return 0;
}

static void _advertise_now(void)
{
    struct ble_gap_adv_params ap;
    memset(&ap, 0, sizeof(ap));
    ap.conn_mode = BLE_GAP_CONN_MODE_UND;
    ap.disc_mode = BLE_GAP_DISC_MODE_GEN;

    int res = ble_gap_adv_start(BLE_OWN_ADDR_RANDOM, NULL, BLE_HS_FOREVER,
                                &ap, _on_gap_evt, NULL);
    assert(res == 0);
    (void)res;
}

static void on_sync(void)
{
    int res;

    puts("setting address");
    ble_addr_t addr;
    res = ble_hs_id_gen_rnd(1, &addr);
    assert(res == 0);
    res = ble_hs_id_set_rnd(addr.val);
    assert(res == 0);

    /* set advertising data, most important the name field in this context */
    const char *name = MYNEWT_VAL(EXPCFG_NODENAME);
    struct ble_hs_adv_fields fields;
    memset(&fields, 0, sizeof(fields));
    fields.name = (uint8_t *)name;
    fields.name_len = strlen(name);
    fields.name_is_complete = 1;
    fields.flags = BLE_HS_ADV_F_DISC_LTD;
    res = ble_gap_adv_set_fields(&fields);
    assert(res == 0);

    /* create l2cap server */
    puts("starting l2cap server");
    res = ble_l2cap_create_server(MYNEWT_VAL(EXPCFG_CID),
                                  MYNEWT_VAL(EXPCFG_MTU), _on_l2cap_evt, NULL);
    assert(res == 0);

    puts("starting to advertise");
    _advertise_now();
}

int main(int argc, char **argv)
{
    int res;
    (void)res;

    printf("Experiment es0: SINK.\n");

    /* Initialize all packages. */
    sysinit();
    expcfg_init();
    seqstats_run();


    /* run event loop */
    printf("# Running eventq now\n");
    ble_hs_cfg.sync_cb = on_sync;
    while (1) {
        os_eventq_run(os_eventq_dflt_get());
    }

    return 0;
}

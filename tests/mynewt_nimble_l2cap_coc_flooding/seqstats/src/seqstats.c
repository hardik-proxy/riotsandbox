/**
 * Depending on the type of package, there are different
 * compilation rules for this directory.  This comment applies
 * to packages of type "pkg."  For other types of packages,
 * please view the documentation at http://mynewt.apache.org/.
 *
 * Put source files in this directory.  All files that have a *.c
 * ending are recursively compiled in the src/ directory and its
 * descendants.  The exception here is the arch/ directory, which
 * is ignored in the default compilation.
 *
 * The arch/<your-arch>/ directories are manually added and
 * recursively compiled for all files that end with either *.c
 * or *.a.  Any directories in arch/ that don't match the
 * architecture being compiled are not compiled.
 *
 * Architecture is set by the BSP/MCU combination.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "assert.h"
#include "os/os.h"
#include "os/os_task.h"
#include "os/os_eventq.h"
#include "hal/hal_timer.h"
#include "seqstats/seqstats.h"

#define SLOT_NUMOF              ((MYNEWT_VAL(SEQSTATS_AVG_ITVL) * 1000) / \
                                 MYNEWT_VAL(SEQSTATS_SLOT_SIZE))

static struct os_task _print_task;
static os_stack_t _stack[MYNEWT_VAL(SEQSTATS_STACKSIZE)];

static volatile unsigned _slot = 0;
static struct os_event _evt;
static struct os_eventq _eq;
static struct hal_timer _tim;

static unsigned _data[SEQSTATS_NUMOF][SLOT_NUMOF];
static unsigned _res[SEQSTATS_NUMOF][SLOT_NUMOF];


static void _on_slot_timeout(void *arg)
{
    (void)arg;

    hal_timer_start(&_tim, MYNEWT_VAL(SEQSTATS_SLOT_SIZE) * 1000);

    if (_slot == (SLOT_NUMOF - 1)) {
        memcpy(_res, _data, sizeof(_data));
        memset(_data, 0, sizeof(_data));
        _slot = 0;
        os_eventq_put(&_eq, &_evt);
    }
    else {
        ++_slot;
    }
}

static void _on_print(struct os_event *ev)
{
    (void)ev;

    for (unsigned stat = 0; stat < (unsigned)SEQSTATS_NUMOF; stat++) {
        printf("%2u)", stat);
        for (unsigned i = 0; i < SLOT_NUMOF; i++) {
            printf(" %u", _res[stat][i]);
        }
        printf("\n");
    }
}

static void _printer_worker(void *arg)
{
    (void)arg;

    printf("moinsen\n");

    while (1) {
        os_eventq_run(&_eq);
    }
}

void seqstats_run(void)
{
    int res;

    memset(_data, 0, sizeof(_data));
    os_eventq_init(&_eq);
    _evt.ev_cb = _on_print;

    /* create printer thread */
    res = os_task_init(&_print_task, "printer", _printer_worker, NULL,
                       MYNEWT_VAL(SEQSTATS_TASK_PRIO), OS_WAIT_FOREVER,
                       _stack, MYNEWT_VAL(SEQSTATS_STACKSIZE));
    assert(res == 0);

    /* set time for the first time */
    res = hal_timer_config(MYNEWT_VAL(SEQSTATS_TIMER),
                           MYNEWT_VAL(SEQSTATS_TIMER_FREQ));
    assert(res == 0);

    hal_timer_set_cb(MYNEWT_VAL(SEQSTATS_TIMER), &_tim, _on_slot_timeout, NULL);
    hal_timer_start(&_tim, MYNEWT_VAL(SEQSTATS_SLOT_SIZE) * 1000);
}

void seqstats_inc(int type)
{
    _data[type][_slot]++;
}

void seqstats_add(int type, unsigned num)
{
    _data[type][_slot] += num;
}

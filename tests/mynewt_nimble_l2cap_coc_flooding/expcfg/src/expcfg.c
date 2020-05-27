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

#include "assert.h"
#include "os/os_mempool.h"
#include "expcfg/expcfg.h"


#define MBUFSIZE_OVHD       (sizeof(struct os_mbuf) + \
                             sizeof(struct os_mbuf_pkthdr))
#define MBUFS_PER_MTU       (MYNEWT_VAL(EXPCFG_MTU) / MYNEWT_VAL(EXPCFG_BUF_CHUNKSIZE))
#define MBUFSIZE            (MYNEWT_VAL(EXPCFG_BUF_CHUNKSIZE) + MBUFSIZE_OVHD)
#define MBUFCNT             (MYNEWT_VAL(EXPCFG_BUF_NUM) * MBUFS_PER_MTU)

static os_membuf_t _coc_mem[OS_MEMPOOL_SIZE(MBUFCNT, MBUFSIZE)];
static struct os_mempool _coc_mempool;
static struct os_mbuf_pool _coc_mbuf_pool;

void expcfg_init(void)
{
    int res;
    (void)res;

    res = os_mempool_init(&_coc_mempool, MBUFCNT, MBUFSIZE, _coc_mem, "appbuf");
    assert(res == 0);
    res = os_mbuf_pool_init(&_coc_mbuf_pool, &_coc_mempool, MBUFSIZE, MBUFCNT);
    assert(res == 0);
}

struct os_mbuf *expcfg_getbuf(void)
{
    struct os_mbuf *buf = os_mbuf_get_pkthdr(&_coc_mbuf_pool, 0);
    assert(buf != NULL);

    return buf;
}

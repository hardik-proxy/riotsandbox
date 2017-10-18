/*
 * Copyright (C) 2015 Freie Universität Berlin
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     sanbox/apps/gobserve
 * @{
 *
 * @file
 * @brief       Example application for demonstrating the RIOT network stack
 *
 * @author      Hauke Petersen <hauke.petersen@fu-berlin.de>
 *
 * @}
 */

#include <stdio.h>

#include "shell.h"
#include "msg.h"

#include "net/gcoap.h"

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

static ssize_t handle_test_hello(coap_pkt_t *pkt, uint8_t *buf, size_t len)
{
    (void)buf;
    (void)len;
    (void)pkt;

    return 0;
}

static ssize_t handle_test_putcount(coap_pkt_t *pkt, uint8_t *buf, size_t len)
{
    (void)buf;
    (void)len;
    (void)pkt;

    return 0;
}


/* lets setup some coap endpoints */
static const coap_resource_t resources[] = {
    {
        .path    = "/test/hello",
        .methods = (COAP_GET),
        .handler = handle_test_hello
    },
    {
        .path    = "/test/putcount",
        .methods = (COAP_GET | COAP_PUT),
        .handler = handle_test_putcount
    }
};

static gcoap_listener_t static_listener = {
    .resources     = (coap_resource_t *)&resources[0],
    .resources_len = (sizeof(resources) / sizeof(resources[0])),
    .next          = NULL
};

/* and some shell commands as well */
static const shell_command_t shell_commands[] = {
    { NULL, NULL, NULL }
};

int main(void)
{
    /* we need a message queue for the thread running the shell in order to
     * receive potentially fast incoming networking packets */
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    /* register static CoAP resources */
    gcoap_register_listener(&static_listener);

    /* start shell */
    puts("All up, running the shell now");
    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);

    /* should be never reached */
    return 0;
}

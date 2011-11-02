/*
 * Copyright (c) 2006, Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.  */

#include "contiki-net.h"
#include <string.h>

static struct etimer timer;
static struct psock ps;
static char buffer[100];

PROCESS(example_psock_client_process, "Pachube test client");
AUTOSTART_PROCESSES(&example_psock_client_process);

/*---------------------------------------------------------------------------*/
static int
handle_connection(struct psock *p)
{
  PSOCK_BEGIN(p);

  PSOCK_SEND_STR(p, "GET /feeds/504.csv HTTP/1.0\r\n");
  PSOCK_SEND_STR(p, "User-Agent: econotag\r\n");
  PSOCK_SEND_STR(p, "\r\n");

  while(1) {
    PSOCK_READTO(p, '\n');
    printf("Got: %s", buffer);
  }

  PSOCK_END(p);
}
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(example_psock_client_process, ev, data)
{
  uip_ip6addr_t addr;

  PROCESS_BEGIN();

  /* Use nginx proxy that is mapping our local
   * `::0:82` to `https://api.pachub.com/v2/`
  */
  uip_ip6addr(&addr, 0xaaaa,0,0,0,0,0,0,1);
  tcp_connect(&addr, UIP_HTONS(82), NULL);

  printf("Trying...\n");
  PROCESS_WAIT_EVENT_UNTIL(ev == tcpip_event);

  if(uip_aborted() || uip_timedout() || uip_closed()) {
    printf("Failed!\n");
  } else if(uip_connected()) {
    printf("Connected.\n");

    PSOCK_INIT(&ps, buffer, sizeof(buffer));

    do {
      handle_connection(&ps);
      PROCESS_WAIT_EVENT_UNTIL(ev == tcpip_event);
    } while(!(uip_closed() || uip_aborted() || uip_timedout()));

    printf("\nClosed.\n");
  }
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/

// Main module for netconsole2journal
// SPDX-License-Identifier:   GPL-3.0-or-later
// Copyright (C) 2022 Joel Lehtonen
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <errno.h>
#include <glib.h>
#include <systemd/sd-daemon.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include "common.h"

#define SD_LISTEN_FDS_START 3
#define DATAGRAM_MAX_LEN 1001 // 1000 is the maximum as defined in netconsole.txt

static gchar *from_bool(gboolean x);

gboolean verbose = FALSE;

static GOptionEntry entries[] =
{
	{ "verbose", 'v', 0, G_OPTION_ARG_NONE, &verbose, "Verbose output.", NULL},
	{ NULL }
};

int main(int argc, char **argv)
{
	g_autoptr(GError) error = NULL;
	g_autoptr(GOptionContext) context = g_option_context_new(NULL);
	g_option_context_add_main_entries(context, entries, NULL);
	g_option_context_set_summary(context, "Receives netconsole entries via systemd socket and outputs log entries.");
	g_option_context_set_description(context,
					 "See 'man systemd.socket' for more information about systemd socket activation\n"
					 "");

	if (!g_option_context_parse(context, &argc, &argv, &error))
	{
		errx(1, "Option parsing failed: %s", error->message);
	}

	// Options validation
	if (argc != 1) {
		errx(1, "This command only expects flags, not arguments. See %s --help", argv[0]);
	}

	int socks = sd_listen_fds(TRUE);
	if (socks != 1) {
		errx(2, "Must be invoked by systemd socket activation. For testing, use 'systemd-socket-activate'");
	}
	
	// Report startup variables
	if (verbose) {
		printf("TODO startup debug info\n");
	}


	struct sockaddr_storage peer_addr;

	while (TRUE) {
		// Buffer for holding a single datagram
		char buf[DATAGRAM_MAX_LEN];
		
		socklen_t peer_addr_len = sizeof(peer_addr);
		ssize_t got = recvfrom(SD_LISTEN_FDS_START,
				       buf, DATAGRAM_MAX_LEN, 0,
				       (struct sockaddr *)&peer_addr, &peer_addr_len);
		if (got == -1) {
			err(3, "Unable to read from socket");
		}

		char hbuf[NI_MAXHOST], sbuf[NI_MAXSERV];
		int code = getnameinfo((struct sockaddr *)&peer_addr, peer_addr_len,
				       hbuf, sizeof(hbuf),
				       sbuf, sizeof(sbuf),
				       NI_NUMERICHOST | NI_NUMERICSERV);
		if (code != 0) {
			warn("Converting of address has failed: %s", gai_strerror(code));
		}

		if (got == DATAGRAM_MAX_LEN) {
			warnx("Too long datagram received from %s:%s, ignoring", hbuf, sbuf);
			continue;
		}
		if (got == 0) {
			warn("Got an empty datagram from %s:%s, ignoring", hbuf, sbuf);
			continue;
		}

		// Outputting this. Truncating newline
		buf[got-1] = '\0';
		printf("Joo saatiinkin %s osoitteesta %s:%s\n", buf, hbuf, sbuf);
	}
	
	return 0;
}

// Returns just string constants, no dynamic strings are allocated.
static gchar *from_bool(gboolean x)
{
	return x ? "on" : "off";
}

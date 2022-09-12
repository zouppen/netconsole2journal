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
#include "common.h"

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

	// Report startup variables
	if (verbose) {
		printf("TODO startup debug info\n");
	}

	return 0;
}

// Returns just string constants, no dynamic strings are allocated.
static gchar *from_bool(gboolean x)
{
	return x ? "on" : "off";
}

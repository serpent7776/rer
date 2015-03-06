/*
 * Copyright © 2014 Serpent7776. All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *	1. Redistributions of source code must retain the above copyright
 *	   notice, this list of conditions and the following disclaimer.
 *	2. Redistributions in binary form must reproduce the above copyright
 *	   notice, this list of conditions and the following disclaimer in the
 *	   documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * Project:   rer
 * File:      main.c
 * By:        Serpent7776
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include "rer.h"
#include "debug.h"

//getopt options
#define OPT_HELP 1000
#define OPT_VERSION 1001
#define OPT_DRY_RUN 1002

extern int optind;

static int longopt = 0;
static struct option longopts[] = {
	{"help", no_argument, &longopt, OPT_HELP},
	{"version", no_argument, &longopt, OPT_VERSION},
	{"dry-run", no_argument, &longopt, OPT_DRY_RUN}
};

void show_help() {
	fprintf(stdout, "rer v%s\nregular expression based file renamer\nusage:\n\trer /pattern/replacement/flags file_list..\n", RER_VERSION);
}

void show_version() {
	fprintf(stdout, "rer v%s\n", RER_VERSION);
}

int main(int argc, char** argv)
{
	int ret;
	int f_show_help = 0;
	int f_show_version = 0;
	int f_dry_run = 0;
	while ((ret = getopt_long(argc, argv, "", longopts, NULL)) != -1) {
		if (ret == 0) {
			switch (longopt) {
				case OPT_HELP:
					f_show_help = 1;
					break;
				case OPT_VERSION:
					f_show_version = 1;
					break;
				case OPT_DRY_RUN:
					f_dry_run = 1;
					break;
			}
		} else if (ret == '?') {
			f_show_help = 1;
		}
	}
	argc -= optind;
	argv += optind;
	if ((argc<2 || f_show_help) && !f_show_version) {
		show_help();
	} else if (f_show_version) {
		show_version();
	} else {
		char* regex = strdup(argv[0]);
		if (regex) {
			const char re_separator[2] = {regex[0], '\0'};
			char* pattern = NULL;
			char* replacement = NULL;
			char* modifiers = NULL;
			char* pos = &regex[1];
			char* prev_pos = NULL;

			pattern = pos;
			prev_pos = strsep(&pos, re_separator);
			if (prev_pos != NULL) {
				replacement = pos;
			}
			prev_pos = strsep(&pos, re_separator);
			if (prev_pos != NULL) {
				modifiers = pos;
			}

			DEBUG_WRITE("main: pattern: '%s'\n", pattern);
			DEBUG_WRITE("main: replacement: '%s'\n", replacement);
			DEBUG_WRITE("main: modifiers: '%s'\n", modifiers);
			RER rer=rer_create(pattern, replacement, modifiers);
			if (rer) {
				if (f_dry_run) {
					rer_setflag(rer, RER_F_DRY_RUN);
				}
				for (int i=1; i<argc; i++) {
					DEBUG_WRITE("main: adding file '%s'\n", argv[i]);
					rer_addfile(rer, argv[i]);
				}
				rer_exec(rer);
				rer_destroy(rer);
			}
			free(regex);
			return EXIT_SUCCESS;
		}
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

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
#include "rer.h"
#include "debug.h"

int main(int argc, char** argv)
{
	if (argc<2) {
		//todo: show help
	} else {
		char* regex = strdup(argv[1]);
		if (regex) {
			const char re_separator[2] = {regex[0], '\0'};
			char* pattern = NULL;
			char* replacement = NULL;
			char* modifiers = NULL;
			char* pos = &regex[1];
			char* prev_pos = NULL;

			pattern = pos;
			prev_pos = strsep(&pos, re_separator);
			if (*prev_pos != '\0') {
				replacement = pos;
			}
			prev_pos = strsep(&pos, re_separator);
			if (*prev_pos != '\0') {
				modifiers = pos;
			}

			DEBUG_WRITE("main: pattern: '%s'\n", pattern);
			DEBUG_WRITE("main: replacement: '%s'\n", replacement);
			DEBUG_WRITE("main: modifiers: '%s'\n", modifiers);
			RER rer=rer_create(pattern, replacement, modifiers);
			if (rer) {
				//todo: parse commandline options
				for (int i=2; i<argc; i++) {
					DEBUG_WRITE("main: adding file '%s'\n", argv[i]);
					rer_addfile(rer, argv[i]);
				}
				rer_exec(rer);
				rer_destroy(rer);
				return 0;
			}
			free(regex);
			return 2;
		}
		return 1;
	}
}

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
 * File:      rer_def.h
 * By:        Serpent7776
 */

#ifndef RER_DEF_H
#define RER_DEF_H

#include <pcre.h>
#include "fdlist.h"
#include "rer.h"

typedef struct {
	//user specified values:
	char*        pattern;     //regex pattern
	char*        replacement;
	char*        modifiers;   //regex modifiers
	int          options;     //options
	fdlist*      files;       //list of files to rename
	rer_callback callback;
	void*        callback_userparam;
	//regex parameters:
	pcre*        re_pattern;  //pcre compiled pattern
	int          re_options;  //pcre options
	int*         re_ovec;     //pcre output vector
	int          re_ovecsize; //pcre output vector size
	//intermediate values:
	char*        newfilename; //pointer to internal buffer that holds new name for file; will be overwritten when processing next file
	int          offset;      //starting offset
	unsigned     counter;     //todo
	Rer_status   status;
} Rer;

typedef struct {
	char modifier;
	int  mod_flag;
} Rer_mod_map;

typedef struct {
	int ovec_size;
} Rer_defaults;

#endif

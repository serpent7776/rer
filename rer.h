/*
 * Copyright © 2014,2015 Serpent7776. All Rights Reserved.
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
 * File:      rer.h
 * By:        Serpent7776
 */

#ifndef RER_H
#define RER_H

#define _rer_s(x) #x
#define _rer_q(x) _rer_s(x)
#define RER_VERSION _rer_q(_RER_VERSION)

//options for rer_[sg]etopti
typedef enum {
	RER_OPT_OVEC_SIZE = 0x0001, /* set default ovec size */
} Rer_option;

typedef enum {
	RER_STATUS_OK            = 0, /* rename successful */
	RER_STATUS_NOMATCH       = 1, /* file name did not match given regex */
	RER_STATUS_ERROR         = 2, /* an error occured */
	RER_STATUS_RENAME_FAILED = 3, /* error while renaming file */
	RER_STATUS_FILE_EXISTS   = 4, /* output file already exists */
} Rer_status;

typedef enum {
	RER_ERR_OK      = 0, /* no error */
	RER_ERR_NULLPTR = 1, /* null pointer */
	RER_ERR_NULL    = 2, /* null pointer */
} Rer_error;

// rer flags
typedef enum {
	RER_F_GLOBAL  = 1,
	RER_F_DRY_RUN = 2,
} Rer_flag;

typedef	void* RER;
typedef void (*rer_callback)(const char* path_original, const char* path_renamed, Rer_status status, void* userparam);

RER        rer_create(const char* pattern, const char* replacement, const char* modifiers);
void       rer_destroy(RER);
Rer_error  rer_addfile(RER, const char* path);
void       rer_setcallback(RER, rer_callback, void* userparam);
int        rer_setdefopti(Rer_option, int);
int        rer_getdefopti(Rer_option);
int        rer_getflags(RER);
int        rer_setflags(RER, int);
int        rer_setflag(RER, Rer_flag);
int        rer_clrflag(RER, Rer_flag);
Rer_error  rer_exec(RER);
char*      rer_processname(RER, const char*);
Rer_status rer_rename(RER, const char*, const char*);
Rer_error  rer_reset(RER);

#endif

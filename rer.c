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
 * File:      rer.c
 * By:        Serpent7776
 */

#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include "platform.h"
#include "rer_int.h"
#include "replace_str.h"
#include "debug.h"

//global variables
static Rer_defaults rer_defaults = {
	16	//ovec_size
};
static Rer_mod_map pcre_mod_map[] = {
	{'i', PCRE_CASELESS},
	{'x', PCRE_EXTENDED},
	{'U', PCRE_UNGREEDY},
	{0, 0}
};
static Rer_mod_map rer_mod_map[]= {
	{'g', RER_MOD_GLOBAL},
	{0, 0}
};

int rer_files_callback(const char* path, void* param)
{
	Rer* rer = (Rer*)param;
	char* newfilename = rer_processname(rer, path);
	if (rer->status == RER_STATUS_OK) {
		rer_rename(rer, path, newfilename);
	}
	if (rer->callback) {
		rer->callback(path, newfilename, rer->status, rer->callback_userparam);
	}
	return 0;
}

RER rer_create(const char* pattern, const char* replacement, const char* modifiers)
{
	if (pattern==NULL || replacement==NULL || modifiers==NULL) {
		return NULL;
	}
	DEBUG_WRITE("rer: creating new rer object\n");
	Rer* rer = malloc(sizeof(Rer));
	if (rer) {
		int errorpos;
		const char* errormsg;
		Rer_modifiers rmods;
		//TODO: cleanup on error
		rer->pattern = strdup(pattern);
		rer->replacement = strdup(replacement);
		rer->modifiers = strdup(modifiers);
		rer_translate_modifiers(modifiers, &rmods);
		rer->options = rmods.rer_mods;
		rer->re_options = rmods.pcre_mods;
		rer->files = fdlist_create();
		rer->re_pattern = pcre_compile(pattern, rer->re_options, &errormsg, &errorpos, NULL);
		rer->callback = NULL;
		rer->callback_userparam = NULL;
		rer->counter = 0;
		//TODO: get vecsize from pcre_info
		rer->re_ovecsize = (rer_defaults.ovec_size+1)*3;
		rer->re_ovec = malloc(sizeof(int*)*rer->re_ovecsize);
		rer->newfilename = NULL;
		DEBUG_WRITE("rer: created object %p\n", rer);
		return rer;
	}
	return NULL;
}

void rer_destroy(RER _rer)
{
	if (_rer) {
		Rer* rer=(Rer*)_rer;
		DEBUG_WRITE("rer: destroying rer object %p\n", rer);
		free(rer->re_ovec);
		free(rer->pattern);
		free(rer->replacement);
		free(rer->modifiers);
		if (rer->newfilename) {
			free(rer->newfilename);
		}
		fdlist_destroy(rer->files);
		DEBUG_WRITE("rer: destroying done\n");
	}
}

Rer_error rer_addfile(RER _rer, const char* path)
{
	if (_rer && path) {
		Rer* rer=(Rer*)_rer;
		DEBUG_WRITE("rer: adding path '%s' to rer %p\n", path, rer);
		fdlist_node* node=fdlist_addfile(rer->files, path, rer->files->lastnode);
		if (node) {
			return RER_ERR_OK;
		}
		return RER_ERR_NULL;
	}
	return RER_ERR_NULLPTR;
}

void rer_setcallback(RER _rer, rer_callback callback, void* param)
{
	if (_rer) {
		Rer* rer=(Rer*)_rer;
		rer->callback=callback;
		rer->callback_userparam=param;
	}
}

int rer_setopti(Rer_options option, int value)
{
	int ret = 0;
	switch (option) {
		case RER_OPT_OVEC_SIZE: 
			ret = rer_defaults.ovec_size;
			rer_defaults.ovec_size = value;
			break;
	}
	return ret;
}

int rer_getopti(Rer_options option)
{
	int ret = 0;
	switch (option) {
		case RER_OPT_OVEC_SIZE:
			ret = rer_defaults.ovec_size;
			break;
	}
	return ret;
}

Rer_error rer_exec(RER _rer)
{
	if (_rer) {
		Rer* rer=(Rer*)_rer;
		DEBUG_WRITE("rer: executing file list for rer %p\n", rer);
		fdlist_walk(rer->files, rer_files_callback, _rer);
	}
	return RER_ERR_OK;
}

char* rer_processname(RER _rer, const char* path)
{
	if (_rer && path) {
		Rer* rer=(Rer*)_rer;
		DEBUG_WRITE("rer: processing path '%s' for rer %p\n", path, rer);
		if (rer->newfilename) {
			free(rer->newfilename);
			rer->newfilename = NULL;
		}
		char* file_name = NULL;
		char* file_dir = NULL;
		int result_ok = 0;
		PLAT_BASE_DIR_NAME(path, file_name, file_dir, result_ok);
		DEBUG_WRITE("basename: %s, dirname: %s\n", file_name, file_dir);
		if (result_ok) {
			const size_t file_dir_length = strlen(file_dir);
			rer->offset = 0;
			rer->newfilename = strdup(file_name);
			size_t repl_step = 0;
			const size_t limit = (rer->options & RER_MOD_GLOBAL) ? -1 : 1;
			for (; repl_step<limit && rer_replace_part(rer)>0 ; repl_step++)
				;
			if (rer->newfilename && repl_step>0) {
				const size_t newfilename_length = strlen(rer->newfilename);
				const size_t newpath_length = file_dir_length+newfilename_length+2;
				char* buf = malloc(newpath_length);
				snprintf(buf, newpath_length, "%s/%s", file_dir, rer->newfilename);
				free(rer->newfilename);
				rer->newfilename = buf;
				rer->status = RER_STATUS_OK;
			} else {
				rer->status = repl_step>0 ? RER_STATUS_ERROR : RER_STATUS_NOMATCH;
			}
			return rer->newfilename;
		}
	}
	return NULL;
}

Rer_status rer_rename(RER _rer, const char* from_name, const char* to_name)
{
	const int file_exists = access(to_name, F_OK);
	if (file_exists == -1 && errno == ENOENT) {
		//file to_name does not exists
		DEBUG_WRITE("rer: renaming file from '%s' to '%s'\n", from_name, to_name);
		const int status = rename(from_name, to_name);
		if (_rer) {
			Rer* rer=(Rer*)_rer;
			rer->status = status==0 ? RER_STATUS_OK : RER_STATUS_RENAME_FAILED;
		}
		return status;
	}
	DEBUG_WRITE("rer: could not rename file '%s' to '%s': file exists\n", from_name, to_name);
	return RER_STATUS_FILE_EXISTS;
}

Rer_error rer_reset(RER _rer)
{
	if (_rer) {
		Rer* rer=(Rer*)_rer;
		DEBUG_WRITE("rer: resetting rer %p\n", rer);
		fdlist_clear(rer->files);
		free(rer->newfilename);
		rer->newfilename = NULL;
		rer->offset = 0;
		rer->counter = 0;
		rer->status = RER_STATUS_OK;
		return RER_ERR_OK;
	}
	return RER_ERR_NULLPTR;
}

/*
 * internal functions:
 */

//TODO: this function is too complicated
int rer_replace_part(RER _rer)
{
	if (_rer) {
		Rer* rer=(Rer*)_rer;
		const size_t newfilename_length = strlen(rer->newfilename);
		const int N = pcre_exec(rer->re_pattern, NULL, rer->newfilename, newfilename_length, rer->offset, 0, rer->re_ovec, rer->re_ovecsize);
		if (N>0) {
			char* repl = strdup(rer->replacement);
			if (repl) {
				//replace backreferences
				for (int i=1; i<N; i++) {
					if (rer->re_ovec[2*i]!=-1 && rer->re_ovec[2*i+1]!=-1) {
						const char* s;
						const size_t ref_maxsize = 8;
						char ref[ref_maxsize];
						snprintf(ref, ref_maxsize, "\\%i", i);
						pcre_get_substring(rer->newfilename, rer->re_ovec, N, i, &s);
						char* newstring = replace_str(repl, ref, s);
						pcre_free_substring(s);
						free(repl);
						if (newstring) {
							repl = newstring;
						} else {
							return 0;
						}
					}
				}
			} else {
				return 0;
			}
			if (repl) {
				const int match_length = rer->re_ovec[2*0+1]-rer->re_ovec[2*0];
				const int repl_length = strlen(repl);
				if (rer->re_ovec[2*0]!=-1 && rer->re_ovec[2*0+1]!=-1) {
					char* buf = replace_stri(rer->newfilename, rer->re_ovec[2*0], match_length, repl);
					if (buf) {
						if (rer->newfilename) {
							free(rer->newfilename);
						}
						rer->newfilename = buf;
					}
					rer->offset = rer->re_ovec[2*0+1]+(repl_length-match_length);
				} else {
				}
			} else {
				return 0;
			}
		}
		return N;
	}
	return 0;
}

int rer_translate_modifiers(const char* mods, Rer_modifiers* rmods)
{
	if (mods && rmods) {
		const int mods_length = strlen(mods);
		rmods->pcre_mods = 0;
		rmods->rer_mods = 0;
		for (int i=0; i<mods_length; i++) {
			const char ch = mods[i];
			int found = 0;
			for (int j=0; pcre_mod_map[j].mod_flag; j++) {
				if (ch == pcre_mod_map[j].modifier) {
					rmods->pcre_mods |= pcre_mod_map[j].mod_flag;
					found = 1;
					break;
				}
			}
			if (!found) {
				for (int j=0; rer_mod_map[j].mod_flag; j++) {
					if (ch == rer_mod_map[j].modifier) {
						rmods->rer_mods |= rer_mod_map[j].mod_flag;
						break;
					}
				}
			}
		}
		return 0;
	}
	return 1;
}

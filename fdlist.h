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
 * File:      fdlist.h
 * By:        Serpent7776
 */

#include <stddef.h>

typedef int (*fdlist_walkproc)(const char* file, void* userparam);

typedef struct _fdlist_node{
	char*	file;
	struct _fdlist_node*	next;
}fdlist_node;

typedef struct{
	fdlist_node*	firstnode;
	fdlist_node*	lastnode;
}fdlist;

fdlist*      fdlist_create();
void         fdlist_destroy(fdlist*);
void         fdlist_clear(fdlist*);
fdlist_node* fdlist_mknode(const char* file);
fdlist_node* fdlist_delnode(fdlist_node*);
fdlist_node* fdlist_addfile(fdlist*, const char* file, fdlist_node* prevnode);
fdlist_node* fdlist_addnode(fdlist*, fdlist_node* restrict, fdlist_node* prevnode);
fdlist_node* fdlist_rmnode(fdlist*, fdlist_node*);
fdlist_node* fdlist_walk(const fdlist*, fdlist_walkproc, void* userparam);
size_t       fdlist_count(const fdlist*);
fdlist_node* fdlist_prevnode(const fdlist*, const fdlist_node* node);

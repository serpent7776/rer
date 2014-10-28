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
 * File:      fdlist.c
 * By:        Serpent7776
 */

#include <stdlib.h>
#include <string.h>
#include "fdlist.h"
#include "debug.h"

fdlist* fdlist_create()
{
	fdlist* list = malloc(sizeof(fdlist));
	if (list) {
		list->firstnode = NULL;
		list->lastnode = NULL;
	}
	DEBUG_WRITE("fdlist: created new list %p\n", list);
	return list;
}

void fdlist_destroy(fdlist* list)
{
	if (list) {
		DEBUG_WRITE("fdlist: destroying list %p\n", list);
		fdlist_clear(list);
		free(list);
	}
}

void fdlist_clear(fdlist* list)
{
	if (list) {
		DEBUG_WRITE("fdlist: clearing list %p\n", list);
		fdlist_node* nextnode;
		for (fdlist_node* node=list->firstnode; node; node=nextnode) {
			nextnode = fdlist_delnode(node);
		}
		list->firstnode = NULL;
		list->lastnode = NULL;
		DEBUG_WRITE("fdlist: clearing done\n");
	}
}

fdlist_node* fdlist_mknode(const char* file)
{
	if (file==NULL) {
		return NULL;
	}
	DEBUG_WRITE("fdlist: making new node for file '%s'\n", file);
	fdlist_node* node=malloc(sizeof(fdlist_node));
	if (node) {
		char* file_dup=strdup(file);
		if (file_dup) {
			node->file=file_dup;
			node->next=NULL;
			DEBUG_WRITE("fdlist: created node %p\n", node);
			return node;
		} else {
			free(node);
		}
	}
	return NULL;
}

fdlist_node* fdlist_delnode(fdlist_node* node)
{
	if (node) {
		DEBUG_WRITE("fdlist: deleting node %p\n", node);
		fdlist_node* nextnode=node->next;
		free(node->file);
		free(node);
		return nextnode;
	}
	return NULL;
}

/*
 * Function: fdlist_addnode
 * Add specified node to the list
 *
 * Parameters:
 * 	fdlist list	-	list
 * 	fdlist_node node	-	node to be added to list
 * 	fdlist_node prevnode	-	node after which specified node should be added
 *
 * Returns:
 * 	if node was successfully added to the list, function returns node or NULL otherwise
 *
 * list and node cannot be NULL;
 * if prevnode is NULL, node is inserted before first element
 */
fdlist_node* fdlist_addnode(fdlist* list, fdlist_node* restrict node, fdlist_node* prevnode)
{
	if (list && node) {
		DEBUG_WRITE("fdlist: adding new node %p to list %p\n", node, list);
		if (prevnode) {
			fdlist_node* const nextnode = prevnode->next;
			node->next = nextnode;
			prevnode->next = node;
			//update last node of list if neccessary
			if (prevnode == list->lastnode) { 
				list->lastnode = node;
			}
		} else {
			//set first element
			node->next = list->firstnode;
			list->firstnode = node;
			if (list->lastnode == NULL) {
				list->lastnode = node;
			}
		}
		return node;
	}
	return NULL;
}

fdlist_node* fdlist_addfile(fdlist* list, const char* file, fdlist_node* prevnode)
{
	if (list && file) {
		DEBUG_WRITE("fdlist: adding file '%s' to list %p after node %p\n", file, list, prevnode);
		fdlist_node* node=fdlist_mknode(file);
		if (node) {
			fdlist_node* retnode = fdlist_addnode(list, node, prevnode);
			if (retnode!=node) {
				free(node);
				return NULL;
			}
			DEBUG_WRITE("fdlist: created node %p\n", retnode);
			return retnode;
		}
	}
	return NULL;
}

fdlist_node* fdlist_rmnode(fdlist* list, fdlist_node* node)
{
	if (list && node) {
		DEBUG_WRITE("fdlist: removing node %p from list %p\n", node, list);
		fdlist_node* prevnode=fdlist_prevnode(list, node);
		if (prevnode) {
			prevnode->next=node->next;
			node->next=NULL;
			return node;
		}
	}
	return NULL;
}

fdlist_node* fdlist_walk(const fdlist* list, fdlist_walkproc proc, void* userparam)
{
	if (list && proc) {
		fdlist_node* node = NULL;
		DEBUG_WRITE("fdlist: walking list %p\n", list);
		for (node=list->firstnode; node; node=node->next) {
			const int ret = proc(node->file, userparam);	//callback return value
			if (ret) {
				break;
			}
		}
		DEBUG_WRITE("fdlist: walk done\n");
		return node;
	}
	return NULL;
}

size_t fdlist_count(const fdlist* list)
{
	if (list) {
		DEBUG_WRITE("fdlist: counting elements in list %p\n", list);
		size_t count=0;
		fdlist_node*  node;
		for (node=list->firstnode; node; node=node->next, count++)
			;
		DEBUG_WRITE("fdlist: %zu elements found\n", count);
		return count;
	}
	return 0;
}

fdlist_node* fdlist_prevnode(const fdlist* list, const fdlist_node* nextnode)
{
	if (list && nextnode) {
		DEBUG_WRITE("fdlist: searching for node before %p\n", nextnode);
		fdlist_node*  node;
		for (node=list->firstnode; node && node->next!=nextnode; node=node->next)
			;
		DEBUG_WRITE("fdlist: %p found\n", node);
		return node;
	}
	return NULL;
}

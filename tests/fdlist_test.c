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

#include <CUnit/CUnit.h>
#include "../fdlist.h"

void fdlist_test_create_destroy()
{
	fdlist* list=fdlist_create();
	CU_ASSERT_PTR_NOT_NULL(list);
	CU_ASSERT_PTR_NULL(list->firstnode);
	CU_ASSERT_PTR_NULL(list->lastnode);
	CU_ASSERT_EQUAL(fdlist_count(list), 0);
	fdlist_destroy(list);
}

void fdlist_test_mknode()
{
	{
		fdlist_node* node=fdlist_mknode(NULL);
		CU_ASSERT_PTR_NULL(node);
	}
	{
		const char* path="/tmp/test";
		fdlist_node* node=fdlist_mknode(path);
		CU_ASSERT_PTR_NOT_NULL(node);
		CU_ASSERT_PTR_NULL(node->next);
		CU_ASSERT_PTR_NOT_NULL(node->file);
		CU_ASSERT_STRING_EQUAL(node->file, path);
	}

}

void fdlist_test_delnode()
{
	{
		fdlist_node* node=fdlist_delnode(NULL);
		CU_ASSERT_PTR_NULL(node);
	}
	{
		fdlist_node* node=fdlist_mknode("/tmp/");
		CU_ASSERT_PTR_NOT_NULL(node);
		fdlist_node* nextNode=fdlist_delnode(node);
		CU_ASSERT_PTR_NULL(nextNode);
	}
}

void fdlist_test_addnode()
{
	fdlist* list=fdlist_create();
	CU_ASSERT_PTR_NOT_NULL(list);
	//
	const char* path1="/tmp/1/";
	fdlist_node* node1=fdlist_mknode(path1);
	CU_ASSERT_PTR_NOT_NULL(node1);
	fdlist_node* retnode1=fdlist_addnode(list, node1, NULL);
	CU_ASSERT_PTR_EQUAL(retnode1, node1);
	CU_ASSERT_PTR_EQUAL(list->firstnode, node1);
	CU_ASSERT_PTR_EQUAL(list->lastnode, node1);
	CU_ASSERT_PTR_NULL(node1->next);
	const size_t size1=fdlist_count(list);
	CU_ASSERT_EQUAL(size1, 1);
	//
	const char* path2="/tmp/2/";
	fdlist_node* node2=fdlist_mknode(path2);
	CU_ASSERT_PTR_NOT_NULL(node2);
	fdlist_node* retnode2=fdlist_addnode(list, node2, node1);
	CU_ASSERT_PTR_EQUAL(retnode2, node2);
	CU_ASSERT_PTR_EQUAL(list->firstnode, node1);
	CU_ASSERT_PTR_EQUAL(list->lastnode, node2);
	CU_ASSERT_PTR_EQUAL(node1->next, node2);
	CU_ASSERT_PTR_NULL(node2->next);
	const size_t size2=fdlist_count(list);
	CU_ASSERT_EQUAL(size2, 2);
	//
	const char* path3="/tmp/3/";
	fdlist_node* node3=fdlist_mknode(path3);
	CU_ASSERT_PTR_NOT_NULL(node3);
	fdlist_node* retnode3=fdlist_addnode(list, node3, NULL);
	CU_ASSERT_PTR_EQUAL(retnode3, node3);
	CU_ASSERT_PTR_EQUAL(list->firstnode, node3);
	CU_ASSERT_PTR_EQUAL(list->lastnode, node2);
	CU_ASSERT_PTR_EQUAL(node3->next, node1);
	const size_t size3=fdlist_count(list);
	CU_ASSERT_EQUAL(size3, 3);
	//
	fdlist_destroy(list);
}

void fdlist_test_addfile()//todo
{
	fdlist* list=fdlist_create();
	CU_ASSERT_PTR_NOT_NULL(list);
	//
	const char* path1="/tmp/1/";
	fdlist_node* node1=fdlist_addfile(list, path1, NULL);
	CU_ASSERT_PTR_EQUAL(list->firstnode, node1);
	CU_ASSERT_PTR_EQUAL(list->lastnode, node1);
	CU_ASSERT_PTR_NULL(node1->next);
	const size_t size1=fdlist_count(list);
	CU_ASSERT_EQUAL(size1, 1);
	//
	const char* path2="/tmp/2/";
	fdlist_node* node2=fdlist_addfile(list, path2, node1);
	CU_ASSERT_PTR_EQUAL(list->firstnode, node1);
	CU_ASSERT_PTR_EQUAL(list->lastnode, node2);
	CU_ASSERT_PTR_EQUAL(node1->next, node2);
	CU_ASSERT_PTR_NULL(node2->next);
	const size_t size2=fdlist_count(list);
	CU_ASSERT_EQUAL(size2, 2);
	//
	const char* path3="/tmp/3/";
	fdlist_node* node3=fdlist_addfile(list, path3, NULL);
	CU_ASSERT_PTR_EQUAL(list->firstnode, node3);
	CU_ASSERT_PTR_EQUAL(list->lastnode, node2);
	CU_ASSERT_PTR_EQUAL(node3->next, node1);
	const size_t size3=fdlist_count(list);
	CU_ASSERT_EQUAL(size3, 3);
	//
	fdlist_destroy(list);
}

void fdlist_test_rmnode()
{
	fdlist* list=fdlist_create();
	CU_ASSERT_PTR_NOT_NULL(list);
	const char* path1="/tmp/1/";
	fdlist_node* node1=fdlist_addfile(list, path1, NULL);
	const char* path2="/tmp/2/";
	fdlist_node* node2=fdlist_addfile(list, path2, NULL);
	const char* path3="/tmp/3/";
	fdlist_node* node3=fdlist_addfile(list, path3, NULL);
	//remove middle element
	fdlist_node* retnode1=fdlist_rmnode(list, node2);
	CU_ASSERT_PTR_EQUAL(retnode1, node2);
	CU_ASSERT_PTR_EQUAL(list->firstnode, node1);
	CU_ASSERT_PTR_EQUAL(list->lastnode, node3);
	CU_ASSERT_PTR_EQUAL(list->firstnode->next, node3);
	const size_t size1=fdlist_count(list);
	CU_ASSERT_EQUAL(size1, 2);
	//remove first element
	fdlist_addfile(list, path2, node1);
	fdlist_node* retnode2=fdlist_rmnode(list, node1);
	CU_ASSERT_PTR_EQUAL(retnode2, node1);
	CU_ASSERT_PTR_EQUAL(list->firstnode, node2);
	CU_ASSERT_PTR_EQUAL(list->lastnode, node3);
	CU_ASSERT_PTR_EQUAL(list->firstnode->next, node3);
	const size_t size2=fdlist_count(list);
	CU_ASSERT_EQUAL(size2, 2);
	//remove last element
	fdlist_addfile(list, path1, NULL);
	fdlist_node* retnode3=fdlist_rmnode(list, node3);
	CU_ASSERT_PTR_EQUAL(retnode3, node3);
	CU_ASSERT_PTR_EQUAL(list->firstnode, node1);
	CU_ASSERT_PTR_EQUAL(list->lastnode, node2);
	CU_ASSERT_PTR_EQUAL(list->firstnode->next, node2);
	const size_t size3=fdlist_count(list);
	CU_ASSERT_EQUAL(size3, 2);
	//
	fdlist_destroy(list);
}

void fdlist_test_clear()
{
	fdlist* list = fdlist_create();
	CU_ASSERT_PTR_NOT_NULL(list);
	fdlist_node* node;
	node = fdlist_addfile(list, "/path/file1", NULL);
	CU_ASSERT_PTR_NOT_NULL(node);
	node = fdlist_addfile(list, "/path/file3", NULL);
	CU_ASSERT_PTR_NOT_NULL(node);
	node = fdlist_addfile(list, "/path/file3", NULL);
	CU_ASSERT_PTR_NOT_NULL(node);
	size_t count;
	count = fdlist_count(list);
	CU_ASSERT_EQUAL(count, 3);
	fdlist_clear(list);
	count = fdlist_count(list);
	CU_ASSERT_PTR_NOT_NULL(list);
	CU_ASSERT_EQUAL(count, 0);
	node = fdlist_addfile(list, "/path/file1", NULL);
	CU_ASSERT_PTR_NOT_NULL(node);
	node = fdlist_addfile(list, "/path/file3", NULL);
	CU_ASSERT_PTR_NOT_NULL(node);
	node = fdlist_addfile(list, "/path/file3", NULL);
	CU_ASSERT_PTR_NOT_NULL(node);
	count = fdlist_count(list);
	CU_ASSERT_EQUAL(count, 3);
	CU_ASSERT_STRING_EQUAL(list->firstnode->file, "/path/file3");
	CU_ASSERT_STRING_EQUAL(list->lastnode->file, "/path/file1");
	fdlist_destroy(list);
}

size_t gcount;
const char* gpath1="/tmp/1/";
const char* gpath2="/tmp/2/";
const char* gpath3="/tmp/3/";
int walkprocparam;
#define WALKPROCPARAM1	740
#define WALKPROCPARAM2	921

int walkproc1(const char* file, void* param)
{
	CU_ASSERT_EQUAL(*(int*)param, WALKPROCPARAM1);
	switch (gcount++) {
		case 0: CU_ASSERT_STRING_EQUAL(file, gpath1); break;
		case 1: CU_ASSERT_STRING_EQUAL(file, gpath2); break;
		case 2: CU_ASSERT_STRING_EQUAL(file, gpath3); break;
	}
	return 0;
}

int walkproc2(const char* file, void* param)
{
	CU_ASSERT_EQUAL(*(int*)param, WALKPROCPARAM2);
	switch (gcount++) {
		case 0: CU_ASSERT_STRING_EQUAL(file, gpath1); return 0;
		case 1: CU_ASSERT_STRING_EQUAL(file, gpath2); return 1;
		case 2: CU_ASSERT_STRING_EQUAL(file, gpath3); return 0;
	}
	return 0;
}

void fdlist_test_walk()
{
	fdlist* list=fdlist_create();
	CU_ASSERT_PTR_NOT_NULL(list);
	fdlist_node* node1=fdlist_addfile(list, gpath1, NULL);
	fdlist_node* node2=fdlist_addfile(list, gpath2, node1);
	fdlist_node* node3=fdlist_addfile(list, gpath3, node2);
	const size_t size1=fdlist_count(list);
	CU_ASSERT_EQUAL(size1, 3);
	//
	gcount=0;
	walkprocparam=WALKPROCPARAM1;
	fdlist_walk(list, walkproc1, &walkprocparam);
	CU_ASSERT_EQUAL(gcount, 3);
	//
	gcount=0;
	walkprocparam=WALKPROCPARAM2;
	fdlist_node* retnode1=fdlist_walk(list, walkproc2, &walkprocparam);
	CU_ASSERT_EQUAL(gcount, 2);
	CU_ASSERT_PTR_EQUAL(retnode1, node2);
	//
	fdlist_destroy(list);
}

void fdlist_test_prev()
{
	fdlist* list=fdlist_create();
	CU_ASSERT_PTR_NOT_NULL(list);
	fdlist_node* node1=fdlist_addfile(list, gpath1, NULL);
	fdlist_node* node2=fdlist_addfile(list, gpath2, node1);
	fdlist_node* node3=fdlist_addfile(list, gpath3, node2);
	const size_t size1=fdlist_count(list);
	CU_ASSERT_EQUAL(size1, 3);
	//
	fdlist_node* retnode1=fdlist_prevnode(list, node1);
	fdlist_node* retnode2=fdlist_prevnode(list, node2);
	fdlist_node* retnode3=fdlist_prevnode(list, node3);
	fdlist_node* retnode4=fdlist_prevnode(list, NULL);
	fdlist_node* retnode5=fdlist_prevnode(NULL, NULL);
	CU_ASSERT_PTR_EQUAL(retnode1, NULL);
	CU_ASSERT_PTR_EQUAL(retnode2, node1);
	CU_ASSERT_PTR_EQUAL(retnode3, node2);
	CU_ASSERT_PTR_EQUAL(retnode4, NULL);
	CU_ASSERT_PTR_EQUAL(retnode5, NULL);
	//
	fdlist_destroy(list);
}

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

#include <stdlib.h>
#include <CUnit/CUnit.h>
#include "../replace_str.h"
#include "replace_str_test.h"

void replace_str_test()
{
	{
		const char* string="foobar";
		const char* search="foo";
		const char* replace="baz";
		const char* result="bazbar";
		char* ret=replace_str(string, search, replace);
		CU_ASSERT_STRING_EQUAL(ret, result);
		free(ret);
	}
	{
		const char* string="foobar";
		const char* search="bar";
		const char* replace="baz";
		const char* result="foobaz";
		char* ret=replace_str(string, search, replace);
		CU_ASSERT_STRING_EQUAL(ret, result);
		free(ret);
	}
	{
		const char* string="foobar";
		const char* search="o";
		const char* replace="xx";
		const char* result="fxxxxbar";
		char* ret=replace_str(string, search, replace);
		CU_ASSERT_STRING_EQUAL(ret, result);
		free(ret);
	}
	{
		const char* string="xxxxxxxxxxxxxxxxxxxxxxx";
		const char* search="x";
		const char* replace="foobar";
		const char* result="foobarfoobarfoobarfoobarfoobarfoobarfoobarfoobarfoobarfoobarfoobarfoobarfoobarfoobarfoobarfoobarfoobarfoobarfoobarfoobarfoobarfoobarfoobar";
		char* ret=replace_str(string, search, replace);
		CU_ASSERT_STRING_EQUAL(ret, result);
		free(ret);
	}
}

void replace_stri_test()
{
	{
		const char* string="foobar";
		int start=0;
		size_t len=3;
		const char* replace="baz";
		const char* result="bazbar";
		char* ret=replace_stri(string, start, len, replace);
		CU_ASSERT_STRING_EQUAL(ret, result);
		free(ret);
	}
	{
		const char* string="foobar";
		int start=3;
		size_t len=3;
		const char* replace="baz";
		const char* result="foobaz";
		char* ret=replace_stri(string, start, len, replace);
		CU_ASSERT_STRING_EQUAL(ret, result);
		free(ret);
	}
	{
		const char* string="foobar";
		int start=1;
		size_t len=2;
		const char* replace="xx";
		const char* result="fxxbar";
		char* ret=replace_stri(string, start, len, replace);
		CU_ASSERT_STRING_EQUAL(ret, result);
		free(ret);
	}
	{
		const char* string="foobar";
		int start=2;
		size_t len=4;
		const char* replace="z";
		const char* result="foz";
		char* ret=replace_stri(string, start, len, replace);
		CU_ASSERT_STRING_EQUAL(ret, result);
		free(ret);
	}
}


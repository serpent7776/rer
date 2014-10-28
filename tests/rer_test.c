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

#define RER_INTERNAL

#include <stdlib.h>
#include <pcre.h>
#include <CUnit/CUnit.h>
#include "../rer.h"

void rer_test_processname()
{
	{
		//whole subject replace with text of same length
		RER rer=rer_create("bar", "baz", "");
		CU_ASSERT_PTR_NOT_NULL_FATAL(rer);
		char* name=rer_processname(rer, "bar");
		const char* expected="./baz";
		CU_ASSERT_PTR_NOT_NULL(name);
		if (name) {
			CU_ASSERT_STRING_EQUAL(name, expected);
		}
		rer_destroy(rer);
	}
	{
		//substring replace of same length
		RER rer=rer_create("bar", "baz", "");
		CU_ASSERT_PTR_NOT_NULL_FATAL(rer);
		char* name=rer_processname(rer, "foobar");
		const char* expected="./foobaz";
		CU_ASSERT_PTR_NOT_NULL(name);
		if (name) {
			CU_ASSERT_STRING_EQUAL(name, expected);
		}
		rer_destroy(rer);
	}
	{
		//substring replace of bigger length
		RER rer=rer_create("o", "xz", "");
		CU_ASSERT_PTR_NOT_NULL_FATAL(rer);
		char* name=rer_processname(rer, "foobar");
		const char* expected="./fxzxzbar";
		CU_ASSERT_PTR_NOT_NULL(name);
		if (name) {
			CU_ASSERT_STRING_EQUAL(name, expected);
		}
		rer_destroy(rer);
	}
	{
		//substring replace of shorter length
		RER rer=rer_create("bar", "1", "");
		CU_ASSERT_PTR_NOT_NULL_FATAL(rer);
		char* name=rer_processname(rer, "foobar");
		const char* expected="./foo1";
		CU_ASSERT_PTR_NOT_NULL(name);
		if (name) {
			CU_ASSERT_STRING_EQUAL(name, expected);
		}
		rer_destroy(rer);
	}
	{
		//multiple replace
		RER rer=rer_create("12", "2", "");
		CU_ASSERT_PTR_NOT_NULL_FATAL(rer);
		char* name=rer_processname(rer, "1212foo12bar121212baz121212");
		const char* expected="./22foo2bar222baz222";
		CU_ASSERT_PTR_NOT_NULL(name);
		if (name) {
			CU_ASSERT_STRING_EQUAL(name, expected);
		}
		rer_destroy(rer);
	}
	{
		//nop
		RER rer=rer_create("foo", "foo", "");
		CU_ASSERT_PTR_NOT_NULL_FATAL(rer);
		char* name=rer_processname(rer, "foo12foobarfoobaz");
		const char* expected="./foo12foobarfoobaz";
		CU_ASSERT_PTR_NOT_NULL(name);
		if (name) {
			CU_ASSERT_STRING_EQUAL(name, expected);
		}
		rer_destroy(rer);
	}
	{
		//replace with /i modifier
		RER rer=rer_create("foo", "bar", "i");
		CU_ASSERT_PTR_NOT_NULL_FATAL(rer);
		char* name=rer_processname(rer, "FoofoofOO");
		const char* expected="./barbarbar";
		CU_ASSERT_PTR_NOT_NULL(name);
		if (name) {
			CU_ASSERT_STRING_EQUAL(name, expected);
		}
		rer_destroy(rer);
	}
	{
		//replace regex \d
		RER rer=rer_create("\\d", "12", "");
		CU_ASSERT_PTR_NOT_NULL_FATAL(rer);
		char* name=rer_processname(rer, "123456789");
		const char* expected="./121212121212121212";
		CU_ASSERT_PTR_NOT_NULL(name);
		if (name) {
			CU_ASSERT_STRING_EQUAL(name, expected);
		}
		rer_destroy(rer);
	}
}

void rer_test_translate_modifiers()
{
	{
		//null string
		const char* mods=NULL;
		const int output = rer_translate_modifiers(mods);
		const int expected = 0;
		CU_ASSERT_EQUAL(output, expected);
	}
	{
		//empty string
		const char* mods="";
		const int output = rer_translate_modifiers(mods);
		const int expected = 0;
		CU_ASSERT_EQUAL(output, expected);
	}
	{
		//empty string
		const char* mods="i";
		const int output = rer_translate_modifiers(mods);
		const int expected = PCRE_CASELESS;
		CU_ASSERT_EQUAL(output, expected);
	}
	{
		//empty string
		const char* mods="ix";
		const int output = rer_translate_modifiers(mods);
		const int expected = PCRE_CASELESS | PCRE_EXTENDED;
		CU_ASSERT_EQUAL(output, expected);
	}
	{
		//empty string
		const char* mods="U";
		const int output = rer_translate_modifiers(mods);
		const int expected = PCRE_UNGREEDY;
		CU_ASSERT_EQUAL(output, expected);
	}
}

void rer_test_options()
{
	int wanted = 24;
	rer_setopti(RER_OPT_OVEC_SIZE, wanted);
	int output = rer_getopti(RER_OPT_OVEC_SIZE);
	CU_ASSERT_EQUAL(wanted, output);
}

void callback1(const char* path_old, const char* path_new, Rer_status status, void* param)
{
	int* val=(int*)param;
	if (strcmp(path_old, "foo") == 0 && status == RER_STATUS_NOMATCH) {
		(*val) |= 1;
	} else if (strcmp(path_old, "bar") == 0 && status == RER_STATUS_NOMATCH) {
		(*val) |= 2;
	} else if (strcmp(path_old, "baz") == 0 && status == RER_STATUS_NOMATCH) {
		(*val) |= 4;
	} else {
		(*val) -= 1;
	}
}

void rer_test_callback()
{
	RER rer = rer_create("xyz", "123", "");
	CU_ASSERT_PTR_NOT_NULL_FATAL(rer);
	int res=0;
	rer_setcallback(rer, callback1, &res);
	rer_addfile(rer, "foo");
	rer_addfile(rer, "bar");
	rer_addfile(rer, "baz");
	rer_exec(rer);
	rer_destroy(rer);
	CU_ASSERT_EQUAL(res, 7);
}

void callback2(const char* path_old, const char* path_new, Rer_status status, void* param)
{
	int* val=(int*)param;
	if (strcmp(path_old, "foo12") == 0 && strcmp(path_new, "./foo1212") == 0 && status == RER_STATUS_RENAME_FAILED) {
		(*val) |= 1;
	} else if (strcmp(path_old, "bar123") == 0 && strcmp(path_new, "./bar121212") == 0 && status == RER_STATUS_RENAME_FAILED) {
		(*val) |= 2;
	} else if (strcmp(path_old, "baz0") == 0 && strcmp(path_new, "./baz12") == 0 && status == RER_STATUS_RENAME_FAILED) {
		(*val) |= 4;
	} else {
		(*val) -= 1;
	}
}

void rer_test_exec()
{
	RER rer = rer_create("\\d", "12", "");
	CU_ASSERT_PTR_NOT_NULL_FATAL(rer);
	int res=0;
	rer_setcallback(rer, callback2, &res);
	rer_addfile(rer, "foo12");
	rer_addfile(rer, "bar123");
	rer_addfile(rer, "baz0");
	rer_exec(rer);
	rer_destroy(rer);
	CU_ASSERT_EQUAL(res, 7);
}

void rer_test_reset()
{
	RER rer = rer_create("\\d", "12", "");
	CU_ASSERT_PTR_NOT_NULL_FATAL(rer);
	int res=0;
	rer_setcallback(rer, callback2, &res);
	Rer_error err;
	err = rer_addfile(rer, "foo12");
	CU_ASSERT_EQUAL(err, RER_ERR_OK);
	err = rer_addfile(rer, "bar123");
	CU_ASSERT_EQUAL(err, RER_ERR_OK);
	err = rer_addfile(rer, "baz0");
	CU_ASSERT_EQUAL(err, RER_ERR_OK);
	rer_exec(rer);
	CU_ASSERT_EQUAL(res, 7);
	err = rer_reset(rer);
	CU_ASSERT_EQUAL(err, RER_ERR_OK);
	res=0;
	err = rer_addfile(rer, "foo12");
	CU_ASSERT_EQUAL(err, RER_ERR_OK);
	err = rer_addfile(rer, "bar123");
	CU_ASSERT_EQUAL(err, RER_ERR_OK);
	err = rer_addfile(rer, "baz0");
	CU_ASSERT_EQUAL(err, RER_ERR_OK);
	rer_exec(rer);
	rer_destroy(rer);
	CU_ASSERT_EQUAL(res, 7);
}

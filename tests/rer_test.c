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

#include <stdlib.h>
#include <pcre.h>
#include <CUnit/CUnit.h>
#include "../rer_int.h"

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
		const char* expected="./fxzobar";
		CU_ASSERT_PTR_NOT_NULL(name);
		if (name) {
			CU_ASSERT_STRING_EQUAL(name, expected);
		}
		rer_destroy(rer);
	}
	{
		//substring replace of bigger length (global)
		RER rer=rer_create("o", "xz", "g");
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
		const char* expected= "./212foo12bar121212baz121212";
		CU_ASSERT_PTR_NOT_NULL(name);
		if (name) {
			CU_ASSERT_STRING_EQUAL(name, expected);
		}
		rer_destroy(rer);
	}
	{
		//multiple replace (global)
		RER rer=rer_create("12", "2", "g");
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
		char* name=rer_processname(rer, "FoOfoofOO");
		const char* expected="./barfoofOO";
		CU_ASSERT_PTR_NOT_NULL(name);
		if (name) {
			CU_ASSERT_STRING_EQUAL(name, expected);
		}
		rer_destroy(rer);
	}
	{
		//replace with /i modifier (global)
		RER rer=rer_create("foo", "bar", "ig");
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
		RER rer=rer_create("\\d", "123", "");
		CU_ASSERT_PTR_NOT_NULL_FATAL(rer);
		char* name=rer_processname(rer, "123456789");
		const char* expected="./12323456789";
		CU_ASSERT_PTR_NOT_NULL(name);
		if (name) {
			CU_ASSERT_STRING_EQUAL(name, expected);
		}
		rer_destroy(rer);
	}
	{
		//replace regex \d (global)
		RER rer=rer_create("\\d", "12", "g");
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
		Rer_modifiers rmods;
		rmods.pcre_mods = 1;
		rmods.rer_mods = 1;
		const char* mods=NULL;
		const int output = rer_translate_modifiers(mods, &rmods);
		const int expected_retcode = 1;
		const int expected_pcre_mods = 1;
		const int expected_rer_mods = 1;
		CU_ASSERT_EQUAL(output, expected_retcode);
		CU_ASSERT_EQUAL(rmods.pcre_mods, expected_pcre_mods);
		CU_ASSERT_EQUAL(rmods.rer_mods, expected_rer_mods);
	}
	{
		//empty string
		Rer_modifiers rmods;
		const char* mods="";
		const int output = rer_translate_modifiers(mods, &rmods);
		const int expected_retcode = 0;
		const int expected_pcre_mods = 0;
		const int expected_rer_mods = 0;
		CU_ASSERT_EQUAL(output, expected_retcode);
		CU_ASSERT_EQUAL(rmods.pcre_mods, expected_pcre_mods);
		CU_ASSERT_EQUAL(rmods.rer_mods, expected_rer_mods);
	}
	{
		//empty string
		Rer_modifiers rmods;
		const char* mods="i";
		const int output = rer_translate_modifiers(mods, &rmods);
		const int expected_retcode = 0;
		const int expected_pcre_mods = PCRE_CASELESS;
		const int expected_rer_mods = 0;
		CU_ASSERT_EQUAL(output, expected_retcode);
		CU_ASSERT_EQUAL(rmods.pcre_mods, expected_pcre_mods);
		CU_ASSERT_EQUAL(rmods.rer_mods, expected_rer_mods);
	}
	{
		//empty string
		Rer_modifiers rmods;
		const char* mods="ix";
		const int output = rer_translate_modifiers(mods, &rmods);
		const int expected_retcode = 0;
		const int expected_pcre_mods = PCRE_CASELESS | PCRE_EXTENDED;
		const int expected_rer_mods = 0;
		CU_ASSERT_EQUAL(output, expected_retcode);
		CU_ASSERT_EQUAL(rmods.pcre_mods, expected_pcre_mods);
		CU_ASSERT_EQUAL(rmods.rer_mods, expected_rer_mods);
	}
	{
		//empty string
		Rer_modifiers rmods;
		const char* mods="U";
		const int output = rer_translate_modifiers(mods, &rmods);
		const int expected_retcode = 0;
		const int expected_pcre_mods = PCRE_UNGREEDY;
		const int expected_rer_mods = 0;
		CU_ASSERT_EQUAL(output, expected_retcode);
		CU_ASSERT_EQUAL(rmods.pcre_mods, expected_pcre_mods);
		CU_ASSERT_EQUAL(rmods.rer_mods, expected_rer_mods);
	}
	{
		//rer /g option
		Rer_modifiers rmods;
		const char* mods="g";
		const int output = rer_translate_modifiers(mods, &rmods);
		const int expected_retcode = 0;
		const int expected_pcre_mods = 0;
		const int expected_rer_mods = RER_F_GLOBAL;
		CU_ASSERT_EQUAL(output, expected_retcode);
		CU_ASSERT_EQUAL(rmods.pcre_mods, expected_pcre_mods);
		CU_ASSERT_EQUAL(rmods.rer_mods, expected_rer_mods);
	}
}

void rer_test_defopts()
{
	int wanted = 24;
	rer_setdefopti(RER_OPT_OVEC_SIZE, wanted);
	int output = rer_getdefopti(RER_OPT_OVEC_SIZE);
	CU_ASSERT_EQUAL(wanted, output);
}

void rer_test_flags()
{
	RER rer=rer_create("foo", "bar", "");
	CU_ASSERT_PTR_NOT_NULL_FATAL(rer);
	int flags = rer_getflags(rer);
	CU_ASSERT_EQUAL(flags, 0);
	flags = rer_setflag(rer, RER_F_GLOBAL);
	CU_ASSERT_EQUAL(flags, RER_F_GLOBAL);
	flags = rer_setflag(rer, RER_F_DRY_RUN);
	CU_ASSERT_EQUAL(flags, RER_F_GLOBAL | RER_F_DRY_RUN);
	flags = rer_getflags(rer);
	CU_ASSERT_EQUAL(flags, RER_F_GLOBAL | RER_F_DRY_RUN);
	flags = rer_clrflag(rer, RER_F_GLOBAL);
	CU_ASSERT_EQUAL(flags, RER_F_DRY_RUN);
	flags = rer_clrflag(rer, RER_F_DRY_RUN);
	CU_ASSERT_EQUAL(flags, 0);
	rer_destroy(rer);
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
	if (strcmp(path_old, "foo12") == 0 && strcmp(path_new, "./fooabc") == 0 && status == RER_STATUS_RENAME_FAILED) {
		(*val) |= 1;
	} else if (strcmp(path_old, "bar123") == 0 && strcmp(path_new, "./barabc") == 0 && status == RER_STATUS_RENAME_FAILED) {
		(*val) |= 2;
	} else if (strcmp(path_old, "baz0") == 0 && strcmp(path_new, "./bazabc") == 0 && status == RER_STATUS_RENAME_FAILED) {
		(*val) |= 4;
	} else {
		(*val) -= 1;
	}
}

void rer_test_exec()
{
	RER rer = rer_create("\\d+", "abc", "");
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
	RER rer = rer_create("\\d+", "abc", "");
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

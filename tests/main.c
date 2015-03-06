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
#include <CUnit/Basic.h>
#include "fdlist_test.h"
#include "replace_str_test.h"
#include "rer_test.h"

int main()
{
	CU_ErrorCode cuError=CU_initialize_registry();
	if (cuError!=CUE_SUCCESS) {
		return 1;
	}
	//
	CU_TestInfo fdlist_tests[]={
		{"create_destroy", fdlist_test_create_destroy},
		{"mknode", fdlist_test_mknode},
		{"delnode", fdlist_test_delnode},
		{"addnode", fdlist_test_addnode},
		{"addfile", fdlist_test_addnode},
		{"rmnode", fdlist_test_addnode},
		{"clear", fdlist_test_clear},
		{"walk", fdlist_test_walk},
		{"prevnode", fdlist_test_prev},
		CU_TEST_INFO_NULL
	};
	CU_TestInfo replace_str_tests[]={
		{"replace_str", replace_str_test},
		{"replace_str1", replace_str1_test},
		{"replace_stri", replace_stri_test},
		CU_TEST_INFO_NULL
	};
	CU_TestInfo rer_tests[]={
		{"rer_processname", rer_test_processname},
		{"rer_translate_modifiers", rer_test_translate_modifiers},
		{"rer_defopts", rer_test_defopts},
		{"rer_flags", rer_test_flags},
		{"rer_callback", rer_test_callback},
		{"rer_exec", rer_test_exec},
		{"rer_reset", rer_test_reset},
		CU_TEST_INFO_NULL
	};
	//
	CU_SuiteInfo testSuites[]={
		{"fdlist", NULL, NULL, NULL, NULL, fdlist_tests},
		{"replace_str", NULL, NULL, NULL, NULL, replace_str_tests},
		{"rer", NULL, NULL, NULL, NULL, rer_tests},
		CU_SUITE_INFO_NULL
	};
	CU_register_suites(testSuites);
	//
	CU_basic_run_tests();
	//
	CU_cleanup_registry();
	return 0;
}

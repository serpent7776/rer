#!/bin/sh

RER=../rer
MKDIR=mkdir
TOUCH=touch
RM_RF='rm -fr'

test_start ( )
{
	TEST=$1
	ASSERT_COUNT=0
	PASS=0
	FAIL=0
	DIR=out/test$1
}

test_end ( )
{
	echo "TEST #${TEST}:"
	echo "	asserts: ${ASSERT_COUNT} total, ${PASS} passed, ${FAIL} failed"
}

assert_pass ( )
{
	ASSERT_COUNT=$(expr ${ASSERT_COUNT} + 1)
	PASS=$(expr ${PASS} + 1)
	return 0
}

assert_fail ( )
{
	ASSERT_COUNT=$(expr ${ASSERT_COUNT} + 1)
	FAIL=$(expr ${FAIL} + 1)
	echo $1
	return 1
}

assert_cmd ( )
{
	eval "$1"
	if [ "$?" != "0" ]; then
		assert_fail "TEST ${TEST}, command '$1' FAILED: $2"
		return $?
	fi
	assert_pass
	return $?
}

${RM_RF} out
${MKDIR} out

test_start 1
${MKDIR} ${DIR}
${TOUCH} ${DIR}/foobar
${RER} /bar/baz/ ${DIR}/foobar
assert_cmd '[ $? -eq 0 ]' "exit status != 0"
assert_cmd "[ -f ${DIR}/foobaz ]" "output file does not exist"
test_end

test_start 2
${MKDIR} ${DIR}
${TOUCH} ${DIR}/foo
${TOUCH} ${DIR}/bar
${TOUCH} ${DIR}/baz
${RER} /ba/foo/ ${DIR}/*
assert_cmd '[ $? -eq 0 ]' "exit status != 0"
assert_cmd "[ -f ${DIR}/foo ]" "output file does not exist"
assert_cmd "[ -f ${DIR}/foor ]" "output file does not exist"
assert_cmd "[ -f ${DIR}/fooz ]" "output file does not exist"
test_end

test_start 3
${MKDIR} ${DIR}
${TOUCH} ${DIR}/xx
${TOUCH} ${DIR}/xxx
${TOUCH} ${DIR}/xyz
${RER} /x/xy/ ${DIR}/*
assert_cmd '[ $? -eq 0 ]' "exit status != 0"
assert_cmd "[ -f ${DIR}/xyx ]" "output file does not exist"
assert_cmd "[ -f ${DIR}/xyxx ]" "output file does not exist"
assert_cmd "[ -f ${DIR}/xyyz ]" "output file does not exist"
test_end

test_start 4
${MKDIR} ${DIR}
${TOUCH} ${DIR}/foo12
${TOUCH} ${DIR}/bar0
${TOUCH} ${DIR}/baz1234
${RER} '/\d+/foo/' ${DIR}/*
assert_cmd '[ $? -eq 0 ]' "exit status != 0"
assert_cmd "[ -f ${DIR}/foofoo ]" "output file does not exist"
assert_cmd "[ -f ${DIR}/barfoo ]" "output file does not exist"
assert_cmd "[ -f ${DIR}/bazfoo ]" "output file does not exist"
test_end

test_start 5
${MKDIR} ${DIR}
${TOUCH} ${DIR}/foo12
${TOUCH} ${DIR}/bar123
${TOUCH} ${DIR}/baz0
${TOUCH} ${DIR}/foobar
${RER} '/([a-z]+)(\d+)/\2\1/' ${DIR}/*
assert_cmd '[ $? -eq 0 ]' "exit status != 0"
assert_cmd "[ -f ${DIR}/foobar ]" "output file does not exist"
assert_cmd "[ -f ${DIR}/12foo ]" "output file does not exist"
assert_cmd "[ -f ${DIR}/123bar ]" "output file does not exist"
assert_cmd "[ -f ${DIR}/0baz ]" "output file does not exist"
test_end

test_start 6
${MKDIR} ${DIR}
${TOUCH} ${DIR}/foobar
${TOUCH} ${DIR}/foobaz
${RER} /bar/baz/ ${DIR}/foobar
assert_cmd '[ $? -eq 0 ]' "exit status != 0"
assert_cmd "[ -f ${DIR}/foobar ]" "output file does not exist"
assert_cmd "[ -f ${DIR}/foobaz ]" "output file does not exist"
test_end

test_start 7
${MKDIR} ${DIR}
${TOUCH} ${DIR}/foobar
${TOUCH} ${DIR}/foobaz
${RER} /foo// ${DIR}/*
assert_cmd '[ $? -eq 0 ]' "exit status != 0"
assert_cmd "[ -f ${DIR}/bar ]" "output file does not exist"
assert_cmd "[ -f ${DIR}/baz ]" "output file does not exist"
test_end

test_start 8
${MKDIR} ${DIR}
${TOUCH} ${DIR}/xx
${TOUCH} ${DIR}/xxx
${TOUCH} ${DIR}/xyz
${RER} /x/xy/g ${DIR}/*
assert_cmd '[ $? -eq 0 ]' "exit status != 0"
assert_cmd "[ -f ${DIR}/xyxy ]" "output file does not exist"
assert_cmd "[ -f ${DIR}/xyxyxy ]" "output file does not exist"
assert_cmd "[ -f ${DIR}/xyyz ]" "output file does not exist"
test_end


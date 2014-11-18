/*
 * Description:	Replaces in the string str all the occurrences of the source string old with the destination string new. The lengths of the strings old and new may differ. The string new may be of any length, but the string "old" must be of non-zero length - the penalty for providing an empty string for the "old" parameter is an infinite loop. In addition, none of the three parameters may be NULL.
 * Returns:	The post-replacement string, or NULL if memory for the new string could not be allocated. Does not modify the original string. The memory for the returned post-replacement string may be deallocated with the standard library function free() when it is no longer required.
 * Dependencies:	For this function to compile, you will need to also #include the following files: <string.h>, <stdlib.h> and <stddef.h>.
 * Licence:	Public domain. You may use this code in any way you see fit, optionally crediting its author (me, Laird Shaw, with assistance from comp.lang.c).
 * http://creativeandcritical.net/str-replace-c/
 */

char* replace_str(const char *str, const char *old, const char *new);
char* replace_str1(const char *str, const char *old, const char *new);
char* replace_stri(const char* str, int start_pos, size_t length, const char* new_str);

/*
 * Description:	Replaces in the string str all the occurrences of the source string old with the destination string new. The lengths of the strings old and new may differ. The string new may be of any length, but the string "old" must be of non-zero length - the penalty for providing an empty string for the "old" parameter is an infinite loop. In addition, none of the three parameters may be NULL.
 * Returns:	The post-replacement string, or NULL if memory for the new string could not be allocated. Does not modify the original string. The memory for the returned post-replacement string may be deallocated with the standard library function free() when it is no longer required.
 * Dependencies:	For this function to compile, you will need to also #include the following files: <string.h>, <stdlib.h> and <stddef.h>.
 * Licence:	Public domain. You may use this code in any way you see fit, optionally crediting its author (me, Laird Shaw, with assistance from comp.lang.c).
 * http://creativeandcritical.net/str-replace-c/
 *
 * Notes
 * Sometimes - see, for example, user Rhomboid's post in this reddit thread - it is suggested that the calculation of the original string's length in the 12th line would be better simplified from p - str + strlen(p) to strlen(str). Whilst this would indeed improve the code's simplicity and readability, it would also remove an optimisation whose effect is often measurable (even if not large), and which is why the code has been written as it has been. The code as it is requires strlen() to iterate over the string only from the last match onwards, whereas the simplified code would require strlen() to iterate over the entire string - a redundant waste most of the time (i.e. when there is at least one match) seeing that strstr() has done some, and often even a lot, of that iteration already. Especially in cases where the last match is very near the end of the string, and where the string is long, this is a significant enough saving for the optimisation to be worth retaining despite the slight loss of simplicity and readability it entails - at least in my view.
 */

#include <string.h>
#include <stdlib.h>
#include <stddef.h>

char *replace_str(const char *str, const char *old, const char *new)
{
	char *ret, *r;
	const char *p, *q;
	size_t oldlen = strlen(old);
	size_t count, retlen, newlen = strlen(new);

	if (oldlen != newlen) {
		for (count = 0, p = str; (q = strstr(p, old)) != NULL; p = q + oldlen)
			count++;
		/* this is undefined if p - str > PTRDIFF_MAX */
		retlen = p - str + strlen(p) + count * (newlen - oldlen);
	} else
		retlen = strlen(str);

	if ((ret = malloc(retlen + 1)) == NULL)
		return NULL;

	for (r = ret, p = str; (q = strstr(p, old)) != NULL; p = q + oldlen) {
		/* this is undefined if q - p > PTRDIFF_MAX */
		ptrdiff_t l = q - p;
		memcpy(r, p, l);
		r += l;
		memcpy(r, new, newlen);
		r += newlen;
	}
	strcpy(r, p);

	return ret;
}

char *replace_stri(const char *str, int start_pos, size_t length, const char *new)
{
	char *ret, *r;
	const char *p, *q;
	const size_t oldlen = length;
	const size_t newlen = strlen(new);
	size_t retlen;

	if (oldlen != newlen) {
		/* this is undefined if p - str > PTRDIFF_MAX */
		q = &str[start_pos];
		p = q + oldlen;
		retlen = p - str + strlen(p) + (newlen - oldlen);
	} else
		retlen = strlen(str);

	if ((ret = malloc(retlen + 1)) == NULL)
		return NULL;

	r = ret;
	p = str;
	q = &p[start_pos];
	/* this is undefined if q - p > PTRDIFF_MAX */
	ptrdiff_t l = q - p;
	memcpy(r, p, l);
	r += l;
	memcpy(r, new, newlen);
	r += newlen;
	p = q + oldlen;
	strcpy(r, p);

	return ret;
}

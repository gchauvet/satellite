/*
 * The MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef __DEIMOS_REPLACE_H__
#define __DEIMOS_REPLACE_H__

/**
 * Replace all occurrences of mch in old with the new string rpl, and
 * stores the result in new, provided that its length (specified in len)
 * is enough.
 *
 * @param new The buffer where the result of the replace operation will be
 *            stored into.
 * @param len The length of the previous buffer.
 * @param old The string where occurrences of mtch must be searched.
 * @param mch The characters to match in old (and to be replaced)
 * @param rpl The characters that will be replaced in place of mch.
 * @return Zero on success, a value less than 0 if an error was encountered
 *         or a value greater than zero (indicating the required storage size
 *         for new) if the buffer was too short to hold the new string.
 */
int replace(char *new, int len, char *old, char *mch, char *rpl);

#endif /* ifndef __DEIMOS_REPLACE_H__ */


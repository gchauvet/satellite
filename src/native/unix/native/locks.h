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

#ifndef __JSVC_LOCKS_H__
#define __JSVC_LOCKS_H__

/*
 * as Cygwin does not support locks, jsvc use NT API to emulate them.
 */
#ifdef OS_CYGWIN

#define F_ULOCK 0               /* Unlock a previously locked region */
#define F_LOCK  1               /* Lock a region for exclusive use */

/*
 * allow a file to be locked
 * @param fildes an open file descriptor
 * @param function a control value that specifies  the action to be taken
 * @param size number of bytes to lock
 * @return Zero on success, a value less than 0 if an error was encountered
 */
int lockf(int fildes, int function, off_t size);

#endif
#endif /* __JSVC_LOCKS_H__ */


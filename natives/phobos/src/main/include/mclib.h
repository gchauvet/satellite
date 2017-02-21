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

#ifndef _MCLIB_H_INCLUDED_
#define _MCLIB_H_INCLUDED_

#ifndef  MIN
#define  MIN(a,b)    (((a)<(b)) ? (a) : (b))
#endif
#ifndef  MAX
#define  MAX(a,b)    (((a)>(b)) ? (a) : (b))
#endif

extern void AplZeroMemory(PVOID Destination, SIZE_T Length);
extern LPVOID AplCopyMemory(PVOID Destination, const VOID* Source, SIZE_T Length);

extern LPSTR lstrlcatA(LPSTR dst, int siz, LPCSTR src);
extern LPWSTR lstrlcatW(LPWSTR dst, int siz, LPCWSTR src);
extern LPSTR lstrlcpyA(LPSTR dst, int siz, LPCSTR src);
extern LPWSTR lstrlcpyW(LPWSTR dst, int siz, LPCWSTR src);
extern LPWSTR lstrlocaseW(LPWSTR str);

#endif /* _MCLIB_H_INCLUDED_ */

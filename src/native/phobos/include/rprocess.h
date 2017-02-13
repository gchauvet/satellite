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

#ifndef _RPROCESS_H_INCLUDED_
#define _RPROCESS_H_INCLUDED_

__APXBEGIN_DECLS

BOOL        apxProcessExecute(APXHANDLE hProcess);

APXHANDLE   apxCreateProcessA(APXHANDLE hPool, DWORD dwOptions,
                              LPAPXFNCALLBACK fnCallback,
                              LPCSTR szUsername, LPCSTR szPassword,
                              BOOL bLogonAsService);
APXHANDLE   apxCreateProcessW(APXHANDLE hPool, DWORD dwOptions,
                              LPAPXFNCALLBACK fnCallback,
                              LPCWSTR szUsername, LPCWSTR szPassword,
                              BOOL bLogonAsService);

#ifdef _UNICODE
#define apxCreateProcess    apxCreateProcessW
#else
#define apxCreateProcess    apxCreateProcessA
#endif

BOOL        apxProcessSetExecutableA(APXHANDLE hProcess, LPCSTR szName);
BOOL        apxProcessSetExecutableW(APXHANDLE hProcess, LPCWSTR szName);

BOOL        apxProcessSetCommandLineA(APXHANDLE hProcess, LPCSTR szCmdline);
BOOL        apxProcessSetCommandLineW(APXHANDLE hProcess, LPCWSTR szCmdline);
BOOL        apxProcessSetCommandArgsW(APXHANDLE hProcess, LPCWSTR szTitle,
                                      DWORD dwArgc, LPCWSTR *lpArgs);

BOOL        apxProcessSetWorkingPathA(APXHANDLE hProcess, LPCSTR szPath);
BOOL        apxProcessSetWorkingPathW(APXHANDLE hProcess, LPCWSTR szPath);

DWORD       apxProcessPutcA(APXHANDLE hProcess, INT ch);
DWORD       apxProcessPutcW(APXHANDLE hProcess, INT ch);
DWORD       apxProcessPutsA(APXHANDLE hProcess, LPCSTR szString);
DWORD       apxProcessPutsW(APXHANDLE hProcess, LPCWSTR szString);

#ifndef _UNICODE
#define     apxProcessPutc  apxProcessPutcA
#define     apxProcessPuts  apxProcessPutsA
#else
#define     apxProcessPutc  apxProcessPutcW
#define     apxProcessPuts  apxProcessPutsW
#endif

DWORD       apxProcessWrite(APXHANDLE hProcess, LPCVOID lpData, DWORD dwLen);

VOID        apxProcessCloseInputStream(APXHANDLE hProcess);
BOOL        apxProcessFlushStdin(APXHANDLE hProcess);

DWORD       apxProcessWait(APXHANDLE hProcess, DWORD dwMilliseconds,
                           BOOL bKill);

BOOL        apxProcessRunning(APXHANDLE hProcess);
DWORD       apxProcessGetPid(APXHANDLE hProcess);


__APXEND_DECLS

#endif /* _RPROCESS_H_INCLUDED_ */

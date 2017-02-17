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

/* Logfile handling
 */

#ifndef _LOG_H_INCLUDED_
#define _LOG_H_INCLUDED_

__APXBEGIN_DECLS

#define APX_LOGLOCK(file)                           \
    APXMACRO_BEGIN                                  \
        DWORD _lmax = 0;                            \
        while(!LockFile(file, 0, 0, 512, 0)) {      \
            Sleep(10);                              \
            if (_lmax++ > 1000) break;              \
        }                                           \
        SetFilePointer(file, 0, NULL, FILE_END);    \
    APXMACRO_END

#define APX_LOGUNLOCK(file)                         \
    APXMACRO_BEGIN                                  \
        UnlockFile(file, 0, 0, 512, 0);             \
    APXMACRO_END

#define APX_LOGENTER()                              \
    if (_pt_sys_loglock)                            \
        EnterCriticalSection(_pt_sys_loglock);      \
    else (void)0

#define APX_LOGLEAVE()                              \
    if (_pt_sys_loglock)                            \
        LeaveCriticalSection(_pt_sys_loglock);      \
    else (void)0

#define APXLOG_LEVEL_DEBUG  0
#define APXLOG_LEVEL_INFO   1
#define APXLOG_LEVEL_WARN   2
#define APXLOG_LEVEL_ERROR  3

#define APXLOG_MARK_INFO    NULL, APXLOG_LEVEL_INFO,  TRUE,  __FILE__, __LINE__, ""
#define APXLOG_MARK_WARN    NULL, APXLOG_LEVEL_WARN,  TRUE,  __FILE__, __LINE__, ""
#define APXLOG_MARK_ERROR   NULL, APXLOG_LEVEL_ERROR, TRUE,  __FILE__, __LINE__, ""
#define APXLOG_MARK_DEBUG   NULL, APXLOG_LEVEL_DEBUG, TRUE,  __FILE__, __LINE__, ""
#define APXLOG_MARK_RAW     NULL, APXLOG_LEVEL_INFO,  FALSE, NULL, 0,
#define APXLOG_MARK_SYSERR  NULL, APXLOG_LEVEL_ERROR, TRUE,  __FILE__, __LINE__, NULL

LPWSTR apxLogFile(
    APXHANDLE hPool,
    LPCWSTR szPath,
    LPCWSTR szPrefix,
    LPCWSTR szName,
    BOOL bTimeStamp,
    DWORD dwRotate
);

HANDLE apxLogOpen(
    APXHANDLE hPool,
    LPCWSTR szPath,
    LPCWSTR szPrefix,
    DWORD dwRotate
);

void apxLogClose(
    HANDLE hFile
);

void apxLogLevelSet(
    HANDLE  hFile,
    DWORD dwLevel
);

void apxLogLevelSetW(
    HANDLE  hFile,
    LPCWSTR szLevel
);

int
apxLogWrite(
    HANDLE  hFile,
    DWORD   dwLevel,
    BOOL    bTimeStamp,
    LPCSTR  szFile,
    DWORD   dwLine,
    LPCSTR  szFormat,
    ...
);

int
apxDisplayError(
    BOOL    bDisplay,
    LPCSTR  szFile,
    DWORD   dwLine,
    LPCSTR  szFormat,
    ...
);

__APXEND_DECLS

#endif /* _LOG_H_INCLUDED_ */

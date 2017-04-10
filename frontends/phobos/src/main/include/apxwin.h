/* Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
 
#ifndef _APXWIN_H_INCLUDED_
#define _APXWIN_H_INCLUDED_

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN 
#endif
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501
#endif

#include <windows.h>
#include <windowsx.h>
#include <commdlg.h>
#include <commctrl.h>
#include <objbase.h>
#include <shlobj.h>
#include <shlwapi.h>
#include <shellapi.h>
#include <zmouse.h>
#include <richedit.h>
#include <lm.h>
#include <stdint.h>

#define APXMACRO_BEGIN                  do {
#define APXMACRO_END                    } while(0)

#ifdef  __cplusplus
#define __APXBEGIN_DECLS    extern "C" {
#define __APXEND_DECLS  }
#else
#define __APXBEGIN_DECLS
#define __APXEND_DECLS
#endif

#define SET_BIT_FLAG(x, b) ((x) |= (1 << b))
#define CLR_BIT_FLAG(x, b) ((x) &= ~(1 << b))
#define TST_BIT_FLAG(x, b) ((x) & (1 << b))

#define IS_INVALID_HANDLE(h) ((((void *)h) == (void *)NULL || ((void *)h) == INVALID_HANDLE_VALUE))
#define IS_VALID_STRING(s)   ((s) != NULL && *(s) != 0)
#define IS_EMPTY_STRING(s)   ((s) == NULL || *(s) == 0)

#define DYNOLAD_TYPE_DECLARE(fnName, callconv, retType)             \
    typedef retType (callconv *PFN_##fnName)                        \

#define DYNLOAD_FPTR_DECLARE(fnName)                                \
    PFN_##fnName FP_##fnName

#define DYNLOAD_FPTR(fnName)  FP_##fnName

#define DYNLOAD_FPTR_ADDRESS(fnName, dllName)                       \
    FP_##fnName = (PFN_##fnName)GetProcAddress(                     \
                                GetModuleHandle(TEXT(#dllName)),    \
                                #fnName)    

#define DYNLOAD_FPTR_LOAD(fnName, dllHandle)                        \
    FP_##fnName = (PFN_##fnName)GetProcAddress(                     \
                                dllHandle,                          \
                                #fnName)    

#define DYNLOAD_CALL(fnName)    (*FP_##fnName)

#ifndef ABS
#define ABS(x)       (((x) > 0) ? (x) : (x) * (-1))
#endif

#define SIZ_RESLEN         256
#define SIZ_RESMAX         (SIZ_RESLEN -1)
#define SIZ_BUFLEN         512
#define SIZ_BUFMAX         (SIZ_BUFLEN -1)
#define SIZ_DESLEN         1024
#define SIZ_DESMAX         (SIZ_DESLEN -1)
#define SIZ_HUGLEN         8192
#define SIZ_HUGMAX         (SIZ_HUGLEN -1)
#define SIZ_PATHLEN        4096
#define SIZ_PATHMAX        4095

#include "handles.h"
#include "log.h"
#include "cmdline.h"
#include "registry.h"
#include "service.h"
#include "java.h"

__APXBEGIN_DECLS

typedef enum {
        APX_WINVER_UNK =       0,
        APX_WINVER_UNSUP =     1,
        APX_WINVER_95 =       10,
        APX_WINVER_95_B =     11,
        APX_WINVER_95_OSR2 =  12,
        APX_WINVER_98 =       14,
        APX_WINVER_98_SE =    16,
        APX_WINVER_ME =       18,

        APX_WINVER_UNICODE =  20, /* Prior versions support only narrow chars */

        APX_WINVER_CE_3 =     23, /* CE is an odd beast, not supporting */
                               /* some pre-NT features, such as the    */
        APX_WINVER_NT =       30, /* narrow charset APIs (fooA fns), while  */
        APX_WINVER_NT_3_5 =   35, /* not supporting some NT-family features.  */
        APX_WINVER_NT_3_51 =  36,

        APX_WINVER_NT_4 =     40,
        APX_WINVER_NT_4_SP2 = 42,
        APX_WINVER_NT_4_SP3 = 43,
        APX_WINVER_NT_4_SP4 = 44,
        APX_WINVER_NT_4_SP5 = 45,
        APX_WINVER_NT_4_SP6 = 46,

        APX_WINVER_2000 =     50,
        APX_WINVER_2000_SP1 = 51,
        APX_WINVER_2000_SP2 = 52,
        APX_WINVER_XP =       60
} APX_OSLEVEL;

extern APX_OSLEVEL apxGetOsLevel();
extern LPWSTR      AsciiToWide(LPCSTR s, LPWSTR ws);
extern LPSTR       WideToAscii(LPCWSTR ws, LPSTR s);
extern LPSTR       WideToANSI(LPCWSTR ws);
extern LPSTR       MzWideToANSI(LPCWSTR ws);

typedef struct APXMULTISZ APXMULTISZ;
typedef APXMULTISZ*       LPAPXMULTISZ;

extern DWORD   apxMultiSzToArrayA(APXHANDLE hPool, LPCSTR lpString, LPSTR **lppArray);
extern DWORD   apxMultiSzToArrayW(APXHANDLE hPool, LPCWSTR lpString, LPWSTR **lppArray);
extern LPWSTR  apxMultiSzCombine(APXHANDLE hPool, LPCWSTR lpStrA, LPCWSTR lpStrB,
                          LPDWORD lpdwLength);

extern LPAPXMULTISZ    apxMultiSzStrdup(LPCTSTR szSrc);
extern LPTSTR          apxMultiSzStrcat(LPAPXMULTISZ lpmSz, LPCTSTR szSrc);
extern LPCTSTR         apxMultiSzGet(LPAPXMULTISZ lpmSz);
/** Remove the specified character from the string
 * Replacement is done in-place.
 */
extern LPTSTR          apxStrCharRemove(LPTSTR szString, TCHAR chSkip);
extern DWORD           apxStrUnQuoteInplaceA(LPSTR szString);
extern DWORD           apxStrUnQuoteInplaceW(LPWSTR szString);

extern BOOL            apxUltohex(ULONG n, LPTSTR lpBuff, DWORD dwBuffLength);
extern BOOL            apxUptohex(ULONG_PTR n, LPTSTR lpBuff, DWORD dwBuffLength);
extern ULONG           apxStrToul(LPCTSTR szNum);
extern ULONG           apxStrToulW(LPCWSTR szNum);
extern ULONG           apxAtoulW(LPCWSTR szNum);

extern BOOL            apxMakeResourceName(LPCTSTR szPrefix, LPTSTR lpBuff,
                                    DWORD dwBuffLength);

extern INT             apxStrMatchA(LPCSTR szString, LPCSTR szPattern, BOOL bIgnoreCase);
extern INT             apxStrMatchW(LPCWSTR szString, LPCWSTR szPattern, BOOL bIgnoreCase);
extern INT             apxMultiStrMatchW(LPCWSTR szString, LPCWSTR szPattern,
                                  WCHAR chSeparator, BOOL bIgnoreCase);
extern LPSTR           apxArrayToMultiSzA(APXHANDLE hPool, DWORD nArgs, LPCSTR *lpArgs);
extern void            apxStrQuoteInplaceW(LPWSTR szString);
extern LPWSTR          apxMszToCRLFW(APXHANDLE hPool, LPCWSTR szStr);
extern LPWSTR          apxCRLFToMszW(APXHANDLE hPool, LPCWSTR szStr, LPDWORD lpdwBytes);
extern LPSTR           apxExpandStrA(APXHANDLE hPool, LPCSTR szString);
extern LPWSTR          apxExpandStrW(APXHANDLE hPool, LPCWSTR szString);
extern void            apxStrCharReplaceA(LPSTR szString, CHAR chReplace, CHAR chReplaceWith);
extern void            apxStrCharReplaceW(LPWSTR szString, WCHAR chReplace, WCHAR chReplaceWith);
extern BOOL            apxAddToPathW(APXHANDLE hPool, LPCWSTR szAdd);

/*
 * Find the first occurrence of lpFind in lpMem.
 * dwLen:   The length of lpFind
 * dwSize:  The length of lpMem
 */
extern LPBYTE  ApcMemSearch(LPCVOID lpMem, LPCVOID lpFind, SIZE_T dwLen, SIZE_T dwSize);
extern LPSTR   AplRindexA(LPCSTR lpStr, int ch);

#define AplMoveMemory   memcpy

extern PSECURITY_ATTRIBUTES GetNullACL();
extern void CleanNullACL(void *sa);

__APXEND_DECLS  

#endif /* _APXWIN_H_INCLUDED_ */


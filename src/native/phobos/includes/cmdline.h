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
 
#ifndef _CMDLINE_H_INCLUDED_
#define _CMDLINE_H_INCLUDED_

__APXBEGIN_DECLS

#define APXCMDOPT_NIL   0x00000000  /* Argopt value not needed */
#define APXCMDOPT_INT   0x00000001  /* Argopt value is unsigned integer */
#define APXCMDOPT_STR   0x00000002  /* Argopt value is string */
#define APXCMDOPT_STE   0x00000006  /* Argopt value is expandable string */
#define APXCMDOPT_MSZ   0x00000010  /* Multiline string '#' separated */
#define APXCMDOPT_BIN   0x00000020  /* Encrypted binary */

#define APXCMDOPT_REG   0x00000100  /* Save to registry */
#define APXCMDOPT_SRV   0x00000200  /* Save to service registry */
#define APXCMDOPT_USR   0x00000400  /* Save to user registry */

#define APXCMDOPT_FOUND 0x00001000  /* The option is present in cmdline as -- */
#define APXCMDOPT_ADD   0x00002000  /* The option is present in cmdline as ++ */


typedef struct APXCMDLINEOPT APXCMDLINEOPT;

struct APXCMDLINEOPT {
    LPWSTR          szName;         /* Long Argument Name */
    LPWSTR          szRegistry;     /* Registry Association */
    LPWSTR          szSubkey;       /* Registry Association */
    DWORD           dwType;         /* Argument type (string, number, multistring */
    LPWSTR          szValue;        /* Return string value  */
    DWORD           dwValue;        /* Return numeric value or present if NIL */
};

typedef struct APXCMDLINE {
    APXCMDLINEOPT       *lpOptions;
    LPWSTR              szArgv0;
    LPWSTR              szExecutable;   /* Parsed argv0 */
    LPWSTR              szExePath;      /* Parsed argv0 */
    LPWSTR              szApplication;  /* Fist string after //CMD// */
    DWORD               dwCmdIndex;     /* Command index */
    LPWSTR              *lpArgvw;
    DWORD               dwArgc;
    APXHANDLE           hPool;
    
} APXCMDLINE, *LPAPXCMDLINE;

LPAPXCMDLINE apxCmdlineParse(
    APXHANDLE hPool,
    APXCMDLINEOPT   *lpOptions,
    LPCWSTR         *lpszCommands,
    LPCWSTR         *lpszAltcmds
);

void apxCmdlineLoadEnvVars(
    LPAPXCMDLINE lpCmdline
);

void apxCmdlineFree(
    LPAPXCMDLINE lpCmdline
);


__APXEND_DECLS

#endif /* _CMDLINE_H_INCLUDED_ */

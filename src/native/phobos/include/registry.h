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
 
#ifndef _REGISTRY_H_INCLUDED_
#define _REGISTRY_H_INCLUDED_

__APXBEGIN_DECLS

#define APXREG_SOFTWARE         0x0001
#define APXREG_SERVICE          0x0002
#define APXREG_USER             0x0004

#define APXREG_PARAMSOFTWARE    0x0010
#define APXREG_PARAMSERVICE     0x0020
#define APXREG_PARAMUSER        0x0040

/** Create or open the process registry keys
 */
APXHANDLE apxCreateRegistryA(APXHANDLE hPool, REGSAM samDesired,
                             LPCSTR szRoot, LPCSTR szKeyName,
                             DWORD dwOptions);
APXHANDLE apxCreateRegistryW(APXHANDLE hPool, REGSAM samDesired,
                             LPCWSTR szRoot, LPCWSTR szKeyName,
                             DWORD dwOptions);

#ifdef _UNICODE
#define apxCreateRegistry   apxCreateRegistryW
#else
#define apxCreateRegistry   apxCreateRegistryA
#endif

/** Delete the process registry keys
 *  samDesired only needs to be KREG_WOW6432 or 0
 */
BOOL      apxDeleteRegistryA(LPCSTR szRoot, LPCSTR szKeyName,
                            REGSAM samDesired, BOOL bDeleteEmptyRoot);

BOOL      apxDeleteRegistryW(LPCWSTR szRoot, LPCWSTR szKeyName,
                            REGSAM samDesired, BOOL bDeleteEmptyRoot);

#ifdef _UNICODE
#define apxDeleteRegistry   apxDeleteRegistryW
#else
#define apxDeleteRegistry   apxDeleteRegistryA
#endif

/** Get the JavaHome path from registry
 * and set the JAVA_HOME environment variable if not found
 * If bPreferJre is set use the JRE's path as JAVA_HOME
 */
LPWSTR    apxGetJavaSoftHome(APXHANDLE hPool, BOOL bPreferJre);

/** Get the Java RuntimeLib from registry (jvm.dll)
 */
LPWSTR    apxGetJavaSoftRuntimeLib(APXHANDLE hPool);

LPSTR     apxRegistryGetStringA(APXHANDLE hRegistry, DWORD dwFrom,
                                LPCSTR szSubkey, LPCSTR szValueName);

LPWSTR    apxRegistryGetStringW(APXHANDLE hRegistry, DWORD dwFrom,
                                LPCWSTR szSubkey, LPCWSTR szValueName);

#ifdef _UNICODE
#define apxRegistryGetString    apxRegistryGetStringW
#else
#define apxRegistryGetString    apxRegistryGetStringA
#endif

LPBYTE    apxRegistryGetBinaryA(APXHANDLE hRegistry, DWORD dwFrom,
                                LPCSTR szSubkey, LPCSTR szValueName,
                                LPBYTE lpData, LPDWORD lpdwLength);

LPBYTE    apxRegistryGetBinaryW(APXHANDLE hRegistry, DWORD dwFrom,
                                LPCWSTR szSubkey, LPCWSTR szValueName,
                                LPBYTE lpData, LPDWORD lpdwLength);

#ifdef _UNICODE
#define apxRegistryGetBinary    apxRegistryGetBinaryW
#else
#define apxRegistryGetBinary    apxRegistryGetBinaryA
#endif

BOOL    apxRegistrySetBinaryA(APXHANDLE hRegistry, DWORD dwFrom,
                              LPCSTR szSubkey, LPCSTR szValueName,
                              const LPBYTE lpData, DWORD dwLength);

BOOL    apxRegistrySetBinaryW(APXHANDLE hRegistry, DWORD dwFrom,
                              LPCWSTR szSubkey, LPCWSTR szValueName,
                              const LPBYTE lpData, DWORD dwLength);

LPWSTR  apxRegistryGetMzStrW(APXHANDLE hRegistry, DWORD dwFrom,
                             LPCWSTR szSubkey, LPCWSTR szValueName,
                             LPWSTR lpData, LPDWORD lpdwLength);

BOOL    apxRegistrySetMzStrW(APXHANDLE hRegistry, DWORD dwFrom,
                             LPCWSTR szSubkey, LPCWSTR szValueName,
                             LPCWSTR lpData, DWORD dwLength);

BOOL    apxRegistrySetStrA(APXHANDLE hRegistry, DWORD dwFrom,
                           LPCSTR szSubkey, LPCSTR szValueName,
                           LPCSTR szValue);

BOOL    apxRegistrySetStrW(APXHANDLE hRegistry, DWORD dwFrom,
                           LPCWSTR szSubkey, LPCWSTR szValueName,
                           LPCWSTR szValue);


#ifdef _UNICODE
#define apxRegistrySetStr    apxRegistrySetStrW
#else
#define apxRegistrySetStr    apxRegistrySetStrA
#endif

BOOL    apxRegistrySetNumW(APXHANDLE hRegistry, DWORD dwFrom,
                           LPCWSTR szSubkey, LPCWSTR szValueName,
                           DWORD dwValue);

DWORD   apxRegistryGetNumberW(APXHANDLE hRegistry, DWORD dwFrom,
                              LPCWSTR szSubkey, LPCWSTR szValueName);


BOOL    apxRegistryDeleteW(APXHANDLE hRegistry, DWORD dwFrom,
                           LPCWSTR szSubkey, LPCWSTR szValueName);

#ifdef _UNICODE
#define apxRegistryDelete    apxRegistryDeleteW
#else
#define apxRegistryDelete    apxRegistryDeleteA
#endif

__APXEND_DECLS

#endif /* _REGISTRY_H_INCLUDED_ */

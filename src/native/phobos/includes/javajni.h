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

#ifndef _JAVAJNI_H_INCLUDED_
#define _JAVAJNI_H_INCLUDED_

#define IDD_HALL_CL                   2100
#define IDD_HALL_JAR                  2101

__APXBEGIN_DECLS

#define     APX_JVM_DESTROY 0x00000001

typedef struct {
    LPCSTR      szClassPath;
    LPCVOID     lpOptions;
    DWORD       dwMs;
    DWORD       dwMx;
    DWORD       dwSs;
    LPCSTR      szJarName;
    LPCVOID     lpArguments;
    BOOL        setErrorOrOut;
    LPCWSTR     szStdErrFilename;
    LPCWSTR     szStdOutFilename;
    LPCWSTR     szLibraryPath;
    DWORD       bJniVfprintf;
} APXJAVA_INIT, *LAPXJAVA_INIT;

APXHANDLE   apxCreateJava(APXHANDLE hPool, LPCWSTR szJvmDllPath);

BOOL        apxJavaInit(APXHANDLE instance, LAPXJAVA_INIT options);

BOOL        apxJavaCall(APXHANDLE instance, LPCSTR szMethod);

DWORD       apxJavaWait(APXHANDLE hJava, DWORD dwMilliseconds, BOOL bKill);

BOOL        apxDestroyJvm(DWORD dwTimeout);

DWORD       apxGetVmExitCode();

void        apxSetVmExitCode(DWORD exitCode);

void        apxJavaDumpAllStacks(APXHANDLE hJava);

__APXEND_DECLS

#endif /* _JAVAJNI_H_INCLUDED_ */

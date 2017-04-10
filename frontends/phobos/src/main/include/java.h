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
 * 
 * Contributed by Guillaume Chauvet <gchauvet@zatarox.com> 10 Nov 2016
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
    LPVOID      failed;
    LPVOID      shutdown;
} APXJAVA_INIT, *LAPXJAVA_INIT;

APXHANDLE   apxCreateJava(APXHANDLE hPool, LPCWSTR szJvmDllPath);

BOOL        apxJavaInit(APXHANDLE instance, LAPXJAVA_INIT options);

BOOL        apxJavaCall(APXHANDLE instance, LPCSTR szMethod);

BOOL        apxDestroyJvm(DWORD dwTimeout);

DWORD       apxGetVmExitCode();

void        apxSetVmExitCode(DWORD exitCode);

void        apxJavaDumpAllStacks(APXHANDLE hJava);

__APXEND_DECLS

#endif /* _JAVAJNI_H_INCLUDED_ */

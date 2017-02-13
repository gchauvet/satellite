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
 
#ifndef _SERVICE_H_INCLUDED_
#define _SERVICE_H_INCLUDED_

__APXBEGIN_DECLS

typedef struct APXSERVENTRY {
    WCHAR   szServiceName[SIZ_RESLEN];
    WCHAR   szObjectName[SIZ_RESLEN];
    WCHAR   szServiceDescription[SIZ_DESLEN];
    DWORD   dwStart;
    LPQUERY_SERVICE_CONFIGW lpConfig;
    SERVICE_STATUS          stServiceStatus;
    SERVICE_STATUS_PROCESS  stStatusProcess;

} APXSERVENTRY, *LPAPXSERVENTRY;


APXHANDLE   apxCreateService(APXHANDLE hPool, DWORD dwOptions,
                             BOOL bManagerMode);

BOOL        apxServiceOpen(APXHANDLE hService, LPCWSTR szServiceName, DWORD dwOptions);


BOOL        apxServiceSetNames(APXHANDLE hService, LPCWSTR szImagePath,
                               LPCWSTR szDisplayName, LPCWSTR szDescription,
                               LPCWSTR szUsername, LPCWSTR szPassword);

BOOL        apxServiceSetOptions(APXHANDLE hService, DWORD dwServiceType,
                                 DWORD dwStartType, DWORD dwErrorControl);

BOOL        apxServiceControl(APXHANDLE hService, DWORD dwControl, UINT uMsg,
                              LPAPXFNCALLBACK fnControlCallback,
                              LPVOID lpCbData);
BOOL        apxServiceInstall(APXHANDLE hService, LPCWSTR szServiceName,
                              LPCWSTR szDisplayName, LPCWSTR szImagePath,
                              LPCWSTR lpDependencies, DWORD dwServiceType,
                              DWORD dwStartType);

LPAPXSERVENTRY  apxServiceEntry(APXHANDLE hService, BOOL bRequeryStatus);

/** Delete te service
 * Stops the service if running
 */
BOOL        apxServiceDelete(APXHANDLE hService);

DWORD       apxServiceBrowse(APXHANDLE hService,
                             LPCWSTR szIncludeNamePattern,
                             LPCWSTR szIncludeImagePattern,
                             LPCWSTR szExcludeNamePattern,
                             LPCWSTR szExcludeImagePattern,
                             UINT uMsg,
                             LPAPXFNCALLBACK fnDisplayCallback,
                             LPVOID lpCbData);

DWORD       apxGetMaxServiceTimeout(APXHANDLE hPool);

__APXEND_DECLS

#endif /* _SERVICE_H_INCLUDED_ */

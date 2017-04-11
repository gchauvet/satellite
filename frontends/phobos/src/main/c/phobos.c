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
 * Contributed by Mladen Turk <mturk@apache.org> 05 Aug 2003
 * Contributed by Guillaume Chauvet <gchauvet@zatarox.com> 10 Nov 2016
 */

/* Force the JNI vprintf functions */
#define _DEBUG_JNI  1
#include "apxwin.h"
#include "phobos.h"

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <fcntl.h>
#include <io.h>         /* _open_osfhandle */

#include <jni.h>

#ifndef  MIN
#define  MIN(a,b)    (((a)<(b)) ? (a) : (b))
#endif

#define STDIN_FILENO  0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2
#define ONE_MINUTE    (60 * 1000)

#ifdef _WIN64
#define KREG_WOW6432  KEY_WOW64_32KEY
#define PRG_BITS      64
#else
#define KREG_WOW6432  0
#define PRG_BITS      32
#endif

typedef struct APX_STDWRAP {
    LPCWSTR szLogPath;
    LPCWSTR szStdOutFilename;
    LPCWSTR szStdErrFilename;
    FILE   *fpStdOutFile;
    FILE   *fpStdErrFile;
} APX_STDWRAP;

/* Use static variables instead of #defines */
static LPCWSTR  PRSRV_AUTO        = L"auto";
static LPCWSTR  PRSRV_MANUAL      = L"manual";
static LPCWSTR  PRSRV_SIGNAL      = L"SIGNAL";
static LPCWSTR  STYPE_INTERACTIVE = L"interactive";

static LPWSTR       _service_name = NULL;
static DWORD        timeout = 0;

/* Allowed commands */
static LPCWSTR _commands[] = {
    L"LS",      /* 1 launch Service */
    L"US",      /* 2 Update Service parameters */
    L"IS",      /* 3 Install Service */
    L"DS",      /* 4 Delete Service */
    L"??",      /* 5 Help */
    L"VS",      /* 6 Version */
    NULL
};

static LPCWSTR _altcmds[] = {
    L"launch",     /* 1 Run Service */
    L"update",      /* 2 Update Service parameters */
    L"install",     /* 3 Install Service */
    L"delete",      /* 4 Delete Service */
    L"help",        /* 5 Help */
    L"version",     /* 6 Version */
    NULL
};

/* Allowed parameters */
static APXCMDLINEOPT _options[] = {

/* 0  */    { L"Description",       L"Description",     NULL,           APXCMDOPT_STR | APXCMDOPT_SRV, NULL, 0},
/* 1  */    { L"DisplayName",       L"DisplayName",     NULL,           APXCMDOPT_STR | APXCMDOPT_SRV, NULL, 0},
/* 2  */    { L"Install",           L"ImagePath",       NULL,           APXCMDOPT_STE | APXCMDOPT_SRV, NULL, 0},
/* 3  */    { L"ServiceUser",       L"ServiceUser",     NULL,           APXCMDOPT_STR | APXCMDOPT_SRV, NULL, 0},
/* 4  */    { L"ServicePassword",   L"ServicePassword", NULL,           APXCMDOPT_STR | APXCMDOPT_SRV, NULL, 0},
/* 5  */    { L"Startup",           L"Startup",         NULL,           APXCMDOPT_STR | APXCMDOPT_SRV, NULL, 0},
/* 6  */    { L"Type",              L"Type",            NULL,           APXCMDOPT_STR | APXCMDOPT_SRV, NULL, 0},

/* 7  */    { L"DependsOn",         L"DependsOn",       NULL,           APXCMDOPT_MSZ | APXCMDOPT_REG, NULL, 0},
/* 8  */    { L"Environment",       L"Environment",     NULL,           APXCMDOPT_MSZ | APXCMDOPT_REG, NULL, 0},
/* 9  */    { L"User",              L"User",            NULL,           APXCMDOPT_STR | APXCMDOPT_REG, NULL, 0},
/* 10 */    { L"Password",          L"Password",        NULL,           APXCMDOPT_BIN | APXCMDOPT_REG, NULL, 0},
/* 11 */    { L"LibraryPath",       L"LibraryPath",     NULL,           APXCMDOPT_STE | APXCMDOPT_REG, NULL, 0},

/* 12 */    { L"JavaHome",          L"JavaHome",        L"Java",        APXCMDOPT_STE | APXCMDOPT_REG, NULL, 0},
/* 13 */    { L"Jvm",               L"Jvm",             L"Java",        APXCMDOPT_STE | APXCMDOPT_REG, NULL, 0},
/* 14 */    { L"JvmOptions",        L"Options",         L"Java",        APXCMDOPT_MSZ | APXCMDOPT_REG, NULL, 0},
/* 15 */    { L"Classpath",         L"Classpath",       L"Java",        APXCMDOPT_STE | APXCMDOPT_REG, NULL, 0},
/* 16 */    { L"JvmMs",             L"JvmMs",           L"Java",        APXCMDOPT_INT | APXCMDOPT_REG, NULL, 0},
/* 17 */    { L"JvmMx",             L"JvmMx",           L"Java",        APXCMDOPT_INT | APXCMDOPT_REG, NULL, 0},
/* 18 */    { L"JvmSs",             L"JvmSs",           L"Java",        APXCMDOPT_INT | APXCMDOPT_REG, NULL, 0},

/* 19 */    { L"StopTimeout",       L"Timeout",         L"Conf",        APXCMDOPT_INT | APXCMDOPT_REG, NULL, 0},
/* 20 */    { L"StartPath",         L"WorkingPath",     L"Conf",        APXCMDOPT_STE | APXCMDOPT_REG, NULL, 0},
/* 21 */    { L"MainJar",           L"MainJar",         L"Conf",        APXCMDOPT_STE | APXCMDOPT_REG, NULL, 0},
/* 22 */    { L"Arguments",         L"Params",          L"Conf",        APXCMDOPT_MSZ | APXCMDOPT_REG, NULL, 0},

/* 23 */    { L"LogPath",           L"Path",            L"Log",         APXCMDOPT_STE | APXCMDOPT_REG, NULL, 0},
/* 24 */    { L"LogPrefix",         L"Prefix",          L"Log",         APXCMDOPT_STR | APXCMDOPT_REG, NULL, 0},
/* 25 */    { L"LogLevel",          L"Level",           L"Log",         APXCMDOPT_STR | APXCMDOPT_REG, NULL, 0},
/* 26 */    { L"StdError",          L"StdError",        L"Log",         APXCMDOPT_STE | APXCMDOPT_REG, NULL, 0},
/* 27 */    { L"StdOutput",         L"StdOutput",       L"Log",         APXCMDOPT_STE | APXCMDOPT_REG, NULL, 0},
/* 28 */    { L"LogJniMessages",    L"LogJniMessages",  L"Log",         APXCMDOPT_INT | APXCMDOPT_REG, NULL, 1},
/* 29 */    { L"Rotate",            L"Rotate",          L"Log",         APXCMDOPT_INT | APXCMDOPT_REG, NULL, 0},
            /* NULL terminate the array */
            { NULL }
};

#define GET_OPT_V(x)  _options[x].szValue
#define GET_OPT_I(x)  _options[x].dwValue
#define GET_OPT_T(x)  _options[x].dwType

#define ST_DESCRIPTION      GET_OPT_T(0)
#define ST_DISPLAYNAME      GET_OPT_T(1)
#define ST_INSTALL          GET_OPT_T(2)
#define ST_SUSER            GET_OPT_T(3)
#define ST_SPASSWORD        GET_OPT_T(4)
#define ST_STARTUP          GET_OPT_T(5)
#define ST_TYPE             GET_OPT_T(6)

#define SO_DESCRIPTION      GET_OPT_V(0)
#define SO_DISPLAYNAME      GET_OPT_V(1)
#define SO_INSTALL          GET_OPT_V(2)
#define SO_SUSER            GET_OPT_V(3)
#define SO_SPASSWORD        GET_OPT_V(4)
#define SO_STARTUP          GET_OPT_V(5)
#define SO_TYPE             GET_OPT_V(6)

#define SO_DEPENDSON        GET_OPT_V(7)
#define SO_ENVIRONMENT      GET_OPT_V(8)

#define SO_USER             GET_OPT_V(9)
#define SO_PASSWORD         GET_OPT_V(10)
#define SO_LIBPATH          GET_OPT_V(11)

#define SO_JAVAHOME         GET_OPT_V(12)
#define SO_JVM              GET_OPT_V(13)
#define SO_JVMOPTIONS       GET_OPT_V(14)
#define SO_CLASSPATH        GET_OPT_V(15)
#define SO_JVMMS            GET_OPT_I(16)
#define SO_JVMMX            GET_OPT_I(17)
#define SO_JVMSS            GET_OPT_I(18)

#define SO_STOPTIMEOUT      GET_OPT_I(19)

#define SO_STARTPATH        GET_OPT_V(20)
#define SO_MAINJAR          GET_OPT_V(21)
#define SO_ARGUMENTS        GET_OPT_V(22)

#define SO_LOGPATH          GET_OPT_V(23)
#define SO_LOGPREFIX        GET_OPT_V(24)
#define SO_LOGLEVEL         GET_OPT_V(25)

#define SO_STDERROR         GET_OPT_V(26)
#define SO_STDOUTPUT        GET_OPT_V(27)
#define SO_JNIVFPRINTF      GET_OPT_I(28)
#define SO_LOGROTATE        GET_OPT_I(29)

static SERVICE_STATUS        _service_status;
static SERVICE_STATUS_HANDLE _service_status_handle = NULL;
/* Set if launched by SCM   */
static BOOL                  _service_mode = FALSE;
/* Global variables and objects */
static APXHANDLE    gPool;
static APXHANDLE    gWorker;
static APX_STDWRAP  gStdwrap;           /* stdio/stderr redirection */
static int          gExitval;
static LPWSTR       gStartPath;

static LPWSTR   _jni_jvmpath              = NULL;   /* Path to jvm dll */
static LPSTR    _jni_jvmoptions           = NULL;   /* Path to jvm options */

static LPSTR    _jni_classpath            = NULL;
static LPSTR    _jni_mainjar              = NULL;
static LPCWSTR  _jni_rparam               = NULL;    /* Startup  arguments */

/* redirect console stdout/stderr to files
 * so that java messages can get logged
 * If stderrfile is not specified it will
 * go to stdoutfile.
 */
static BOOL redirectStdStreams(APX_STDWRAP *lpWrapper, LPAPXCMDLINE lpCmdline)
{
    BOOL aErr = FALSE;
    BOOL aOut = FALSE;

    /* Allocate console if we have none
     */
    /* redirect to file or console */
    if (lpWrapper->szStdOutFilename) {
        if (lstrcmpiW(lpWrapper->szStdOutFilename, PRSRV_AUTO) == 0) {
            WCHAR lsn[1024];
            aOut = TRUE;
            wcsncpy(lsn, lpCmdline->szApplication, 1020);
            wcsncat(lsn, L"-stdout", 1020);
            CharLower(lsn);
            lpWrapper->szStdOutFilename = apxLogFile(gPool,
                                                     lpWrapper->szLogPath,
                                                     lsn,
                                                     NULL, TRUE,
                                                     SO_LOGROTATE);
        }
        /* Delete the file if not in append mode
         * XXX: See if we can use the params instead of that.
         */
        if (!aOut)
            DeleteFileW(lpWrapper->szStdOutFilename);
        if ((lpWrapper->fpStdOutFile = _wfopen(lpWrapper->szStdOutFilename, L"a"))) {
            _dup2(_fileno(lpWrapper->fpStdOutFile), 1);
            *stdout = *lpWrapper->fpStdOutFile;
            setvbuf(stdout, NULL, _IONBF, 0);
        }
        else
            lpWrapper->szStdOutFilename = NULL;
    }
    if (lpWrapper->szStdErrFilename) {
        if (lstrcmpiW(lpWrapper->szStdErrFilename, PRSRV_AUTO) == 0) {
            WCHAR lsn[1024];
            aErr = TRUE;
            wcsncpy(lsn, lpCmdline->szApplication, 1020);
            wcsncat(lsn, L"-stderr", 1020);
            CharLower(lsn);
            lpWrapper->szStdErrFilename = apxLogFile(gPool,
                                                     lpWrapper->szLogPath,
                                                     lsn,
                                                     NULL, TRUE,
                                                     SO_LOGROTATE);
        }
        if (!aErr)
            DeleteFileW(lpWrapper->szStdErrFilename);
        if ((lpWrapper->fpStdErrFile = _wfopen(lpWrapper->szStdErrFilename,
                                              L"a"))) {
            _dup2(_fileno(lpWrapper->fpStdErrFile), 2);
            *stderr = *lpWrapper->fpStdErrFile;
            setvbuf(stderr, NULL, _IONBF, 0);
        }
        else
            lpWrapper->szStdOutFilename = NULL;
    }
    else if (lpWrapper->fpStdOutFile) {
        _dup2(_fileno(lpWrapper->fpStdOutFile), 2);
        *stderr = *lpWrapper->fpStdOutFile;
         setvbuf(stderr, NULL, _IONBF, 0);
    }
    return TRUE;
}

/* Debugging functions */
static void printUsage(LPAPXCMDLINE lpCmdline, BOOL isHelp)
{
    int i = 0;
    fwprintf(stderr, L"Usage:\n\t%s command [ServiceName] [--options]\n", lpCmdline->szExecutable);
    fwprintf(stderr, L"\nCommands:\n");
    if (isHelp)
        fwprintf(stderr, L"\thelp\t\t\tThis page\n");
    fwprintf(stderr, L"\tinstall\t[ServiceName]\tInstall Service\n");
    fwprintf(stderr, L"\tupdate\t[ServiceName]\tUpdate Service parameters\n");
    fwprintf(stderr, L"\tdelete\t[ServiceName]\tDelete Service\n");
    fwprintf(stderr, L"\tlaunch\t[ServiceName]\tStart Service (used by Microsoft Service Control Manager)\n");
    fwprintf(stderr, L"\tversion \t\tDisplay version\n");
    fwprintf(stderr, L"\nOptions:\n");
    while (_options[i].szName) {
        fwprintf(stderr, L"\t--%s\n", _options[i].szName);
        ++i;
    }
}

static void printVersion(void)
{
    fwprintf(stderr, L"Satellite Service Wrapper version %u.%u.%u/Win%d (%S)\n",
            PRG_VERSION_MAJOR,PRG_VERSION_MINOR, PRG_VERSION_PATCH, PRG_BITS, __DATE__);
    fwprintf(stderr, L"Copyright 2017 Zatarox, based on Commons Daemon");
}

static void setInprocEnvironment()
{
    LPWSTR p, e;

    if (!SO_ENVIRONMENT)
        return;    /* Nothing to do */

    for (p = SO_ENVIRONMENT; *p; p++) {
        e = apxExpandStrW(gPool, p);
        _wputenv(e);
        apxFree(e);
        while (*p)
            p++;
    }
}

/* Load the configuration from Registry
 * loads only nonspecified items
 */
static BOOL loadConfiguration(LPAPXCMDLINE lpCmdline)
{
    APXHANDLE hRegistry;
    int i = 0;

    if (!lpCmdline->szApplication) {
        /* Handle empty service names */
        apxLogWrite(APXLOG_MARK_WARN "No service name provided");
        return FALSE;
    }
    SetLastError(ERROR_SUCCESS);
    hRegistry = apxCreateRegistryW(gPool, KEY_READ | KREG_WOW6432,
                                   PRG_REGROOT,
                                   lpCmdline->szApplication,
                                   APXREG_SOFTWARE | APXREG_SERVICE);
    if (IS_INVALID_HANDLE(hRegistry)) {
        if (GetLastError() == ERROR_FILE_NOT_FOUND)
            apxLogWrite(APXLOG_MARK_WARN "The system cannot find the Registry key for service '%S'",
                        lpCmdline->szApplication);
        else
            apxLogWrite(APXLOG_MARK_SYSERR);
        return FALSE;
    }
    /* browse through options */
    while (_options[i].szName) {
        DWORD dwFrom;

        dwFrom = (_options[i].dwType & APXCMDOPT_REG) ? APXREG_PARAMSOFTWARE : APXREG_SERVICE;
        if (!(_options[i].dwType & APXCMDOPT_FOUND)) {
            if (_options[i].dwType & APXCMDOPT_STR) {
                _options[i].szValue = apxRegistryGetStringW(hRegistry,
                                                            dwFrom,
                                                            _options[i].szSubkey,
                                                            _options[i].szRegistry);
                /* Expand environment variables */
                if (_options[i].szValue && (_options[i].dwType & APXCMDOPT_STE)) {
                    LPWSTR exp = apxExpandStrW(gPool, _options[i].szValue);
                    if (exp != _options[i].szValue)
                        apxFree(_options[i].szValue);
                    _options[i].szValue = exp;
                }
            }
            else if (_options[i].dwType & APXCMDOPT_INT) {
                _options[i].dwValue = apxRegistryGetNumberW(hRegistry,
                                                            dwFrom,
                                                            _options[i].szSubkey,
                                                            _options[i].szRegistry);
            }
            else if (_options[i].dwType & APXCMDOPT_MSZ) {
                _options[i].szValue = apxRegistryGetMzStrW(hRegistry,
                                                           dwFrom,
                                                           _options[i].szSubkey,
                                                           _options[i].szRegistry,
                                                           NULL,
                                                           &(_options[i].dwValue));
            }
        }
        /* Merge the command line options with registry */
        else if (_options[i].dwType & APXCMDOPT_ADD) {
            LPWSTR cv = _options[i].szValue;
            LPWSTR ov = NULL;
            if (_options[i].dwType & APXCMDOPT_MSZ) {
                ov = apxRegistryGetMzStrW(hRegistry, dwFrom,
                                          _options[i].szSubkey,
                                          _options[i].szRegistry,
                                          NULL,
                                          &(_options[i].dwValue));
                _options[i].szValue = apxMultiSzCombine(gPool, ov, cv,
                                                        &(_options[i].dwValue));
                if (ov)
                    apxFree(ov);
            }
        }
        ++i;
    }
    apxCloseHandle(hRegistry);
    return TRUE;
}

/* Save changed configuration to registry
 */
static BOOL saveConfiguration(LPAPXCMDLINE lpCmdline)
{
    APXHANDLE hRegistry;
    int i = 0;
    hRegistry = apxCreateRegistryW(gPool, KEY_WRITE | KREG_WOW6432,
                                   PRG_REGROOT,
                                   lpCmdline->szApplication,
                                   APXREG_SOFTWARE | APXREG_SERVICE);
    if (IS_INVALID_HANDLE(hRegistry))
        return FALSE;
    /* TODO: Use array size */
    while (_options[i].szName) {
        /* Skip the service params */
        if ((_options[i].dwType & APXCMDOPT_SRV) ||
            !(_options[i].dwType & APXCMDOPT_FOUND)) {
                /* Skip non-modified version */
        }
        /* Update only modified params */
        else if (_options[i].dwType & APXCMDOPT_STR)
            apxRegistrySetStrW(hRegistry, APXREG_PARAMSOFTWARE,
                               _options[i].szSubkey,
                               _options[i].szRegistry,
                               _options[i].szValue);
        else if (_options[i].dwType & APXCMDOPT_INT)
            apxRegistrySetNumW(hRegistry, APXREG_PARAMSOFTWARE,
                               _options[i].szSubkey,
                               _options[i].szRegistry,
                               _options[i].dwValue);
        else if (_options[i].dwType & APXCMDOPT_MSZ)
            apxRegistrySetMzStrW(hRegistry, APXREG_PARAMSOFTWARE,
                                 _options[i].szSubkey,
                                 _options[i].szRegistry,
                                 _options[i].szValue,
                                 _options[i].dwValue);
        ++i;
    }
    apxCloseHandle(hRegistry);
    return TRUE;
}

/* Operations */
static BOOL docmdInstallService(LPAPXCMDLINE lpCmdline)
{
    APXHANDLE hService;
    BOOL  rv;
    DWORD dwStart = SERVICE_DEMAND_START;
    DWORD dwType  = SERVICE_WIN32_OWN_PROCESS;
    WCHAR szImage[SIZ_HUGLEN];
    WCHAR szName[SIZ_BUFLEN];

    apxLogWrite(APXLOG_MARK_DEBUG "Installing service...");
    hService = apxCreateService(gPool, SC_MANAGER_CREATE_SERVICE, FALSE);
    if (IS_INVALID_HANDLE(hService)) {
        apxLogWrite(APXLOG_MARK_ERROR "Unable to open the Service Manager");
        return FALSE;
    }
    /* Check the startup mode */
    if ((ST_STARTUP & APXCMDOPT_FOUND) &&
        lstrcmpiW(SO_STARTUP, PRSRV_AUTO) == 0)
        dwStart = SERVICE_AUTO_START;
    /* Check the service type */
    if ((ST_TYPE & APXCMDOPT_FOUND) &&
        lstrcmpiW(SO_TYPE, STYPE_INTERACTIVE) == 0)
        dwType |= SERVICE_INTERACTIVE_PROCESS;

    /* Check if --Install is provided */
    if (!IS_VALID_STRING(SO_INSTALL)) {
        wcsncpy(szImage, lpCmdline->szExePath, SIZ_HUGLEN);
        wcsncat(szImage, L"\\", SIZ_HUGLEN);
        wcsncat(szImage, lpCmdline->szExecutable, SIZ_HUGLEN);
        wcsncat(szImage, L".exe", SIZ_HUGLEN);
    }
    else
        wcsncpy(szImage, SO_INSTALL, SIZ_HUGLEN);
    /* Replace not needed quotes */
    apxStrQuoteInplaceW(szImage);
    /* Add run-service command line option */
    wcsncat(szImage, L" launch ", SIZ_HUGLEN);
    wcsncat(szName, lpCmdline->szApplication, SIZ_BUFLEN);
    apxStrQuoteInplaceW(szName);
    wcsncat(szImage, szName, SIZ_HUGLEN);
    SO_INSTALL = apxPoolStrdupW(gPool, szImage);
    /* Ensure that option gets saved in the registry */
    ST_INSTALL |= APXCMDOPT_FOUND;
    apxLogWrite(APXLOG_MARK_INFO "Service %S name %S", lpCmdline->szApplication,
                SO_DISPLAYNAME);
    rv = apxServiceInstall(hService,
                          lpCmdline->szApplication,
                          SO_DISPLAYNAME,    /* --DisplayName  */
                          SO_INSTALL,
                          SO_DEPENDSON,      /* --DependendsOn */
                          dwType,
                          dwStart);
    /* Set the --Description */
    if (rv) {
        LPCWSTR sd = NULL;
        LPCWSTR su = NULL;
        LPCWSTR sp = NULL;
        if (ST_DESCRIPTION & APXCMDOPT_FOUND) {
            sd = SO_DESCRIPTION;
            apxLogWrite(APXLOG_MARK_DEBUG "Setting service description %S",
                        SO_DESCRIPTION);
        }
        if (ST_SUSER & APXCMDOPT_FOUND) {
            su = SO_SUSER;
            apxLogWrite(APXLOG_MARK_DEBUG "Setting service user %S",
                        SO_SUSER);
        }
        if (ST_SPASSWORD & APXCMDOPT_FOUND) {
            sp = SO_SPASSWORD;
            apxLogWrite(APXLOG_MARK_DEBUG "Setting service password %S",
                        SO_SPASSWORD);
        }
        apxServiceSetNames(hService, NULL, NULL, sd, su, sp);
    }
    apxCloseHandle(hService);
    if (rv) {
        saveConfiguration(lpCmdline);
        apxLogWrite(APXLOG_MARK_INFO "Service '%S' installed",
                    lpCmdline->szApplication);
    }
    else
        apxLogWrite(APXLOG_MARK_ERROR "Failed installing '%S' service",
                    lpCmdline->szApplication);

    return rv;
}

static BOOL docmdDeleteService(LPAPXCMDLINE lpCmdline)
{
    APXHANDLE hService;
    BOOL  rv = FALSE;

    apxLogWrite(APXLOG_MARK_INFO "Deleting service...");
    hService = apxCreateService(gPool, SC_MANAGER_CONNECT, FALSE);
    if (IS_INVALID_HANDLE(hService)) {
        apxLogWrite(APXLOG_MARK_ERROR "Unable to open the Service Manager");
        return FALSE;
    }
    /* Delete service will stop the service if running */
    if (apxServiceOpen(hService, lpCmdline->szApplication, SERVICE_ALL_ACCESS)) {
        WCHAR szWndManagerClass[SIZ_RESLEN];
        HANDLE hWndManager = NULL;
        wcsncpy(szWndManagerClass, lpCmdline->szApplication, SIZ_RESLEN);
        wcsncat(szWndManagerClass, L"_CLASS", SIZ_RESLEN);
        /* Close the monitor application if running */
        if ((hWndManager = FindWindowW(szWndManagerClass, NULL)) != NULL) {
            SendMessage(hWndManager, WM_CLOSE, 0, 0);
        }
        rv = apxServiceDelete(hService);
    }
    if (rv) {
        /* Delete all service registry settings */
        apxDeleteRegistryW(PRG_REGROOT, lpCmdline->szApplication, KREG_WOW6432, TRUE);
        apxLogWrite(APXLOG_MARK_DEBUG "Service '%S' deleted",
                    lpCmdline->szApplication);
    }
    else {
        apxDisplayError(FALSE, NULL, 0, "Unable to delete '%S' service",
                        lpCmdline->szApplication);
    }
    apxCloseHandle(hService);
    apxLogWrite(APXLOG_MARK_INFO "Delete service finished.");
    return rv;
}

static BOOL docmdUpdateService(LPAPXCMDLINE lpCmdline)
{
    APXHANDLE hService;
    BOOL  rv = TRUE;

    apxLogWrite(APXLOG_MARK_INFO "Updating service...");

    hService = apxCreateService(gPool, SC_MANAGER_CREATE_SERVICE, FALSE);
    if (IS_INVALID_HANDLE(hService)) {
        apxLogWrite(APXLOG_MARK_ERROR "Unable to open the Service Manager");
        return FALSE;
    }
    SetLastError(0);
    /* Open the service */
    if (!apxServiceOpen(hService, lpCmdline->szApplication, SERVICE_ALL_ACCESS)) {
        /* Close the existing manager handler.
         * It will be reopened inside install.
         */
        apxCloseHandle(hService);
        /* In case service doesn't exist try to install it.
         * Install will fail if there is no minimum parameters required.
         */
        return docmdInstallService(lpCmdline);
    }
    else {
        DWORD dwStart = SERVICE_NO_CHANGE;
        DWORD dwType  = SERVICE_NO_CHANGE;
        LPCWSTR su = NULL;
        LPCWSTR sp = NULL;
        if (ST_SUSER & APXCMDOPT_FOUND) {
            su = SO_SUSER;
            apxLogWrite(APXLOG_MARK_DEBUG "Setting service user %S",
                        SO_SUSER);
        }
        if (ST_SPASSWORD & APXCMDOPT_FOUND) {
            sp = SO_SPASSWORD;
            apxLogWrite(APXLOG_MARK_DEBUG "Setting service password %S",
                        SO_SPASSWORD);
        }
        rv = (rv && apxServiceSetNames(hService,
                                       NULL,                /* Never update the ImagePath */
                                       SO_DISPLAYNAME,
                                       SO_DESCRIPTION,
                                       su,
                                       sp));
        /* Update the --Startup mode */
        if (ST_STARTUP & APXCMDOPT_FOUND) {
            if (!lstrcmpiW(SO_STARTUP, PRSRV_AUTO))
                dwStart = SERVICE_AUTO_START;
            else if (!lstrcmpiW(SO_STARTUP, PRSRV_MANUAL))
                dwStart = SERVICE_DEMAND_START;
        }
        if (ST_TYPE & APXCMDOPT_FOUND) {
            if (!lstrcmpiW(SO_TYPE, STYPE_INTERACTIVE))
                dwType = SERVICE_WIN32_OWN_PROCESS | SERVICE_INTERACTIVE_PROCESS;
        }
        rv = (rv && apxServiceSetOptions(hService,
                                         dwType,
                                         dwStart,
                                         SERVICE_NO_CHANGE));

        apxLogWrite(APXLOG_MARK_INFO "Service '%S' updated",
                    lpCmdline->szApplication);

        rv = (rv && saveConfiguration(lpCmdline));
    }
    apxCloseHandle(hService);
    if (rv)
        apxLogWrite(APXLOG_MARK_INFO "Update service finished.");
    else
        apxLogWrite(APXLOG_MARK_INFO "Update service '%S' failed.",
                                     lpCmdline->szApplication);
    return rv;
}


/* Report the service status to the SCM, including service specific exit code
 */
static BOOL reportServiceStatusE(DWORD dwCurrentState,
                                 DWORD dwWin32ExitCode,
                                 DWORD dwWaitHint,
                                 DWORD dwServiceSpecificExitCode)
{
   static DWORD dwCheckPoint = 1;
   BOOL fResult = TRUE;

   apxLogWrite(APXLOG_MARK_DEBUG "reportServiceStatusE: %d, %d, %d, %d", dwCurrentState, dwWin32ExitCode, dwWaitHint, dwServiceSpecificExitCode);

   if (_service_mode && _service_status_handle) {
       switch(dwCurrentState) {
           case SERVICE_RUNNING: 
               _service_status.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_PAUSE_CONTINUE | SERVICE_ACCEPT_SHUTDOWN;
               break;
            case SERVICE_PAUSED: 
                _service_status.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_PAUSE_CONTINUE;
                break;
           default:
               _service_status.dwControlsAccepted = 0;
               break;
       }

       _service_status.dwCurrentState  = dwCurrentState;
       _service_status.dwWin32ExitCode = dwWin32ExitCode;
       _service_status.dwWaitHint      = dwWaitHint;
       _service_status.dwServiceSpecificExitCode = dwServiceSpecificExitCode;

       switch(dwCurrentState) {
           case SERVICE_RUNNING:
           case SERVICE_STOPPED:
           case SERVICE_PAUSED:
                _service_status.dwCheckPoint = dwCheckPoint++;
                break;
           default:
               _service_status.dwCheckPoint = 0;
       }
       fResult = SetServiceStatus(_service_status_handle, &_service_status);
       if (!fResult) {
           /* TODO: Deal with error */
           apxLogWrite(APXLOG_MARK_ERROR "Failed to set service status");
       }
   }
   return fResult;
}

/* Report the service status to the SCM
 */
static BOOL reportServiceStatus(DWORD dwCurrentState,
                                DWORD dwWin32ExitCode,
                                DWORD dwWaitHint)
{
    return reportServiceStatusE(dwCurrentState, dwWin32ExitCode, dwWaitHint, 0);
}

static BOOL reportServiceStatusStopped(DWORD exitCode)
{
    if (exitCode) {
        return reportServiceStatusE(SERVICE_STOPPED, ERROR_SERVICE_SPECIFIC_ERROR, 0, exitCode);
    } else {
        return reportServiceStatus(SERVICE_STOPPED, NO_ERROR, 0);
    }
}

/* Executed when the service receives stop event */
static DWORD WINAPI serviceShutdown()
{
    DWORD  rv = 0;

    apxLogWrite(APXLOG_MARK_INFO "Shutdown background process...");
    if (apxJavaCall(gWorker, "shutdown") == FALSE) {
        apxLogWrite(APXLOG_MARK_ERROR "Failed to shutdown");
        rv = 3;
    }
    apxLogWrite(APXLOG_MARK_INFO "Shutdown completed.");
    return rv;
}

static
DWORD callServiceMethod(const char *method)
{
    DWORD  rv = 0;
    if (apxJavaCall(gWorker, method) == FALSE) {
        rv = 4;
        apxLogWrite(APXLOG_MARK_ERROR "Failed calling method '%s'", method);
        if (!IS_INVALID_HANDLE(gWorker))
            apxCloseHandle(gWorker);    /* Close the worker handle */
        gWorker = NULL;
    }
    return rv;
}


/* Executed when the service receives start event */
static DWORD serviceStart()
{
    DWORD result;
    apxLogWrite(APXLOG_MARK_INFO "Resume background process...");
    if((result = callServiceMethod("resume")) == 0)
        apxLogWrite(APXLOG_MARK_DEBUG "Background process started");
    return result;
}

/* Executed when the service receives stop event */
static DWORD WINAPI serviceStop()
{
    DWORD result;
    apxLogWrite(APXLOG_MARK_INFO "Send pause signal to background process...");
    if((result = callServiceMethod("pause")) == 0)
        apxLogWrite(APXLOG_MARK_DEBUG "Background process stopped.");
    return result;
}

static void shutdown(JNIEnv *env, jobject source, jboolean reload)
{
    apxLogWrite(APXLOG_MARK_DEBUG "Shutdown requested (reload is %s)", reload == JNI_TRUE ? "enabled" : "disabled");
    serviceStop();
    if (reload == TRUE)
        serviceStart();
    else
        serviceShutdown();
}

static void failed(JNIEnv *env, jobject source, jstring message)
{
    if (message) {
        const char *msg = (*env)->GetStringUTFChars(env, message, NULL);
        apxLogWrite(APXLOG_MARK_ERROR "Failed %s", msg ? msg : "(null)");
        if (msg)
            (*env)->ReleaseStringUTFChars(env, message, msg);
    }
    else
        apxLogWrite(APXLOG_MARK_ERROR "Failed requested");
    serviceStop();
}

/* Executed when initialize the service */
static int serviceInit()
{
    DWORD  rv = 0;
    FILETIME fts;
    APXJAVA_INIT gArgs;
    
    if (!IS_INVALID_HANDLE(gWorker)) {
        apxLogWrite(APXLOG_MARK_INFO "Worker is not defined");
        return TRUE;    /* Nothing to do */
    }
    GetSystemTimeAsFileTime(&fts);
    if (IS_EMPTY_STRING(SO_STARTPATH))
        SO_STARTPATH = gStartPath;
    if (IS_VALID_STRING(SO_STARTPATH)) {
        /* If the Working path is specified change the current directory */
        SetCurrentDirectoryW(SO_STARTPATH);
    }
    if (IS_VALID_STRING(SO_LIBPATH)) {
        /* Add LibraryPath to the PATH */
       apxAddToPathW(gPool, SO_LIBPATH);
    }
    
    timeout = SO_STOPTIMEOUT * 1000;
    if (timeout > 0x7FFFFFFF)
        timeout = INFINITE;     /* If the timeout was '-1' wait forever */
    if (!timeout)
        timeout = 5 * 60 * 1000;   /* Use the 5 minute default shutdown */
    
    /* Set the environment using putenv, so JVM can use it */
    setInprocEnvironment();
    /* Create the JVM global worker */
    gWorker = apxCreateJava(gPool, _jni_jvmpath);
    
    // Start init entry point
    gArgs.szJarName        = _jni_mainjar;
    gArgs.szClassPath      = _jni_classpath;
    gArgs.lpOptions        = _jni_jvmoptions;
    gArgs.lpArguments      = _jni_rparam;
    gArgs.dwMs             = SO_JVMMS;
    gArgs.dwMx             = SO_JVMMX;
    gArgs.dwSs             = SO_JVMSS;
    gArgs.szStdErrFilename = gStdwrap.szStdErrFilename;
    gArgs.szStdOutFilename = gStdwrap.szStdOutFilename;
    gArgs.szLibraryPath    = SO_LIBPATH;
    gArgs.bJniVfprintf     = SO_JNIVFPRINTF;
    gArgs.failed           = &failed;
    gArgs.shutdown         = &shutdown;
 
    if (!apxJavaInit(gWorker, &gArgs)) {
        apxLogWrite(APXLOG_MARK_ERROR "Failed connecting JVM");
        rv = 3;
    } else {
        FILETIME fte;
        ULARGE_INTEGER s, e;
        DWORD    nms;

        GetSystemTimeAsFileTime(&fte);
        s.LowPart  = fts.dwLowDateTime;
        s.HighPart = fts.dwHighDateTime;
        e.LowPart  = fte.dwLowDateTime;
        e.HighPart = fte.dwHighDateTime;
        nms = (DWORD)((e.QuadPart - s.QuadPart) / 10000);
        apxLogWrite(APXLOG_MARK_INFO "Service started in %d ms.", nms);
    }
    return rv;
}

/* Service control handler
 */
static
void WINAPI service_ctrl_handler(DWORD dwCtrlCode)
{
    switch (dwCtrlCode) {
        case SERVICE_CONTROL_INTERROGATE:
            reportServiceStatus(_service_status.dwCurrentState, _service_status.dwWin32ExitCode, _service_status.dwWaitHint);
            break;
        case SERVICE_CONTROL_PAUSE:
            apxLogWrite(APXLOG_MARK_INFO "Service PAUSE signalled");
            reportServiceStatus(SERVICE_PAUSE_PENDING, NO_ERROR, timeout);
            serviceStop();
            reportServiceStatus(SERVICE_PAUSED, NO_ERROR, 0);
            break;
        case SERVICE_CONTROL_CONTINUE:
            apxLogWrite(APXLOG_MARK_INFO "Service CONTINUE signalled");
            reportServiceStatus(SERVICE_CONTINUE_PENDING, NO_ERROR, timeout);
            serviceStart();
            reportServiceStatus(SERVICE_RUNNING, NO_ERROR, 0);
            break;
        case SERVICE_CONTROL_STOP:
            apxLogWrite(APXLOG_MARK_INFO "Service STOP signalled");
            goto perform_stop;
        case SERVICE_CONTROL_SHUTDOWN:
            apxLogWrite(APXLOG_MARK_INFO "Service SHUTDOWN signalled");
perform_stop:
            reportServiceStatus(SERVICE_STOP_PENDING, NO_ERROR, timeout);
            serviceStop();
            reportServiceStatus(SERVICE_STOPPED, NO_ERROR, timeout);
            apxLogWrite(APXLOG_MARK_INFO "Service SHUTDOWN signalled");
            serviceShutdown();
            break;
   }
}

/* Main service execution loop */
void WINAPI serviceMain(DWORD argc, LPTSTR *argv)
{
    DWORD rc = 0;
    _service_status.dwServiceType      = SERVICE_WIN32_OWN_PROCESS;
    _service_status.dwCurrentState     = SERVICE_START_PENDING;
    _service_status.dwControlsAccepted = SERVICE_CONTROL_INTERROGATE;
    _service_status.dwWin32ExitCode    = 0;
    _service_status.dwCheckPoint       = 0;
    _service_status.dwWaitHint         = 0;
    _service_status.dwServiceSpecificExitCode = 0;

    apxLogWrite(APXLOG_MARK_DEBUG "Inside ServiceMain...");

    /* Find the classpath */
    _jni_jvmpath = SO_JVM;
    _jni_rparam = SO_ARGUMENTS;
    _jni_classpath = WideToANSI(SO_CLASSPATH);
    _jni_mainjar = WideToANSI(SO_MAINJAR);
    _jni_jvmoptions = MzWideToANSI(SO_JVMOPTIONS);

    apxLogWrite(APXLOG_MARK_DEBUG "Waiting for service initialization...");
    const int result = serviceInit();
    if (result == 0) {
        apxLogWrite(APXLOG_MARK_DEBUG "Initialization service finished.");
        
        if (_service_mode) {
            /* Register Service Control handler */
            _service_status_handle = RegisterServiceCtrlHandlerW(_service_name, service_ctrl_handler);
            if (IS_INVALID_HANDLE(_service_status_handle)) {
                apxLogWrite(APXLOG_MARK_ERROR "Failed to register Service Control for %S", _service_name);
                return;
            }
            reportServiceStatus(SERVICE_START_PENDING, NO_ERROR, 3000);
        }

        if ((rc = serviceStart()) == 0) {
            if (IS_VALID_STRING(_service_name)) {
                WCHAR en[SIZ_HUGLEN];
                int i;
                PSECURITY_ATTRIBUTES sa = GetNullACL();
                wcsncpy(en, L"Global\\", SIZ_DESLEN);
                wcsncat(en, _service_name, SIZ_DESLEN);
                wcsncat(en, PRSRV_SIGNAL, SIZ_DESLEN);
                for (i = 7; i < lstrlenW(en); i++) {
                    if (en[i] == L' ')
                        en[i] = L'_';
                    else
                        en[i] = towupper(en[i]);
                }
                CleanNullACL((void *)sa);
            }
            /* Service is started */
            reportServiceStatus(SERVICE_RUNNING, NO_ERROR, 0);
        }
    } else {
        apxLogWrite(APXLOG_MARK_DEBUG "Failing JVM instantiation");
        exit(reportServiceStatusStopped(result)); // TODO: Stop service gracefully
    }
}

static
BOOL docmdRunService(LPAPXCMDLINE lpCmdline)
{
    BOOL rv;
    SERVICE_TABLE_ENTRYW dispatch_table[] = {
        { lpCmdline->szApplication, (LPSERVICE_MAIN_FUNCTIONW)serviceMain },
        { NULL, NULL }
    };
    _service_mode = TRUE;
    _service_name = lpCmdline->szApplication;
    apxLogWrite(APXLOG_MARK_INFO "Running '%S' Service...", _service_name);
    if (StartServiceCtrlDispatcherW(dispatch_table)) {
        apxLogWrite(APXLOG_MARK_INFO "Run service finished.");
        rv = TRUE;
    }
    else {
        apxLogWrite(APXLOG_MARK_ERROR "StartServiceCtrlDispatcher for '%S' failed",
                    lpCmdline->szApplication);
        rv = FALSE;
    }
    return rv;
}

static const char *gSzProc[] = {
    "parse command line arguments",
    "load configuration",
    "run service",
    "update service parameters",
    "install service",
    "delete service",
    NULL
};

void __cdecl main(int argc, char **argv)
{
    UINT rv = 0;

    LPAPXCMDLINE lpCmdline;
    
    apxHandleManagerInitialize();
    /* Create the main Pool */
    gPool = apxPoolCreate(NULL, 0);
    
    /* Parse the command line */
    if ((lpCmdline = apxCmdlineParse(gPool, _options, _commands, _altcmds)) == NULL) {
        apxLogWrite(APXLOG_MARK_ERROR "Invalid command line arguments");
        rv = 1;
        goto cleanup;
    }
    apxCmdlineLoadEnvVars(lpCmdline);

    if (lpCmdline->dwCmdIndex < 5) {
        if (!loadConfiguration(lpCmdline) &&
            lpCmdline->dwCmdIndex < 2) {
            apxLogWrite(APXLOG_MARK_ERROR "Load configuration failed");
            rv = 2;
            goto cleanup;
        }
    }

    apxLogOpen(gPool, SO_LOGPATH, SO_LOGPREFIX, SO_LOGROTATE);
    apxLogLevelSetW(NULL, SO_LOGLEVEL);
    apxLogWrite(APXLOG_MARK_DEBUG "Phobos log initialized");
    if (SO_LOGROTATE)
        apxLogWrite(APXLOG_MARK_DEBUG "Log will rotate each %d seconds.", SO_LOGROTATE);

    apxLogWrite(APXLOG_MARK_INFO "Phobos (%u.%u.%u %d-bit) started",
                PRG_VERSION_MAJOR, PRG_VERSION_MINOR, PRG_VERSION_PATCH, PRG_BITS);

    memset(&gStdwrap, 0, sizeof(APX_STDWRAP));
    gStartPath = lpCmdline->szExePath;
    gStdwrap.szLogPath = SO_LOGPATH;
    gStdwrap.szStdOutFilename = SO_STDOUTPUT;
    gStdwrap.szStdErrFilename = SO_STDERROR;
    redirectStdStreams(&gStdwrap, lpCmdline);

    switch (lpCmdline->dwCmdIndex) {
        case 1: {/* Run Service */
            SYSTEMTIME t;
            GetLocalTime(&t);
            fprintf(stdout, "\n%d-%02d-%02d %02d:%02d:%02d "
                            "Phobos stdout initialized\n",
                            t.wYear, t.wMonth, t.wDay,
                            t.wHour, t.wMinute, t.wSecond);
            fprintf(stderr, "\n%d-%02d-%02d %02d:%02d:%02d "
                            "Phobos stderr initialized\n",
                            t.wYear, t.wMonth, t.wDay,
                            t.wHour, t.wMinute, t.wSecond);
            if (!docmdRunService(lpCmdline))
                rv = 2;
        }
        break;
        case 2: /* Update Service parameters */
            if (!docmdUpdateService(lpCmdline))
                rv = 3;
        break;
        case 3: /* Install Service */
            if (!docmdInstallService(lpCmdline))
                rv = 4;
        break;
        case 4: /* Delete Service */
            if (!docmdDeleteService(lpCmdline))
                rv = 5;
        break;
        case 5: /* Print Usage and exit */
            printUsage(lpCmdline, TRUE);
        break;
        case 6: /* Print version and exit */
            printVersion();
        break;
        default:
            /* Unknown command option */
            apxLogWrite(APXLOG_MARK_ERROR "Unknown command line option");
            printUsage(lpCmdline, FALSE);
            rv = 99;
        break;
    }

cleanup:
    if (rv) {
        int ix = 0;
        if (rv > 0 && rv < 10)
            ix = rv;
        apxLogWrite(APXLOG_MARK_ERROR "Phobos failed "
                                      "with exit value: %d (Failed to %s)",
                                      rv, gSzProc[ix]);
        if (ix > 2 && !_service_mode) {
            /* Print something to the user console */
            apxDisplayError(FALSE, NULL, 0, "Failed to %s", gSzProc[ix]);
        }
    }
    else
        apxLogWrite(APXLOG_MARK_INFO "Phobos finished");
    if (lpCmdline)
        apxCmdlineFree(lpCmdline);
    _service_status_handle = NULL;
    _service_mode = FALSE;
    _flushall();
    apxLogClose(NULL);
    apxHandleManagerDestroy();
    ExitProcess(rv);
}

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

#ifndef _PRUNMGR_H
#define _PRUNMGR_H

#if !defined(PRG_VERSION_MAJOR) || !defined(PRG_VERSION_MINOR) || !defined(PRG_VERSION_PATCH)
#define PRG_VERSION_MAJOR 0
#define PRG_VERSION_MINOR 0
#define PRG_VERSION_PATCH 0
#endif

#define PRG_REGROOT   L"Apache Software Foundation\\Procrun 2.0"

#define IDM_TM_EXIT                     2000
#define IDM_TM_START                    2001
#define IDM_TM_STOP                     2002
#define IDM_TM_PAUSE                    2003
#define IDM_TM_RESTART                  2004
#define IDM_TM_CONFIG                   2005
#define IDM_TM_ABOUT                    2006
#define IDM_TM_DUMP                     2007

#define IDMS_REFRESH                    2020

#define IDI_ICONSTOP                    2030
#define IDI_ICONRUN                     2031



/* Property pages */

#define IDD_PROPPAGE_SGENERAL           2600
#define IDC_PPSGNAME                    2601
#define IDC_PPSGDISP                    2602
#define IDC_PPSGDESC                    2603
#define IDC_PPSGDEXE                    2604
#define IDC_PPSGCMBST                   2605
#define IDC_PPSGSTATUS                  2606
#define IDC_PPSGSTART                   2607
#define IDC_PPSGSTOP                    2608
#define IDC_PPSGPAUSE                   2609
#define IDC_PPSGRESTART                 2610

#define IDD_PROPPAGE_LOGON              2620
#define IDC_PPSLLS                      2621
#define IDC_PPSLID                      2622
#define IDC_PPSLUA                      2623
#define IDC_PPSLUSER                    2624
#define IDC_PPSLBROWSE                  2625
#define IDC_PPSLPASS                    2626
#define IDC_PPSLCPASS                   2627
#define IDL_PPSLPASS                    2628
#define IDL_PPSLCPASS                   2629

#define IDD_PROPPAGE_LOGGING            2640
#define IDC_PPLGLEVEL                   2641
#define IDC_PPLGPATH                    2642
#define IDC_PPLGBPATH                   2643
#define IDC_PPLGPREFIX                  2644
#define IDC_PPLGPIDFILE                 2645
#define IDC_PPLGSTDOUT                  2646
#define IDC_PPLGBSTDOUT                 2647
#define IDC_PPLGSTDERR                  2648
#define IDC_PPLGBSTDERR                 2649

#define IDD_PROPPAGE_JVM                2660
#define IDC_PPJAUTO                     2661
#define IDC_PPJJVM                      2662
#define IDC_PPJBJVM                     2663
#define IDC_PPJCLASSPATH                2664
#define IDC_PPJOPTIONS                  2665
#define IDC_PPJMS                       2666
#define IDC_PPJMX                       2667
#define IDC_PPJSS                       2668

#define IDD_PROPPAGE_SETUP              2680
#define IDC_PPRJAR                    	2681
#define IDC_PPRJPATH                    2683
#define IDC_PPRWPATH                    2684
#define IDC_PPRBWPATH                   2685
#define IDC_PPRBJPATH                   2686
#define IDC_PPRARGS                     2687
#define IDC_PPRTIMEOUT                  2688

#define IDS_ALREAY_RUNING               3100
#define IDS_ERRORCMD                    3101
#define IDS_HSSTART                     3102
#define IDS_HSSTOP                      3103
#define IDS_HSPAUSE                     3104
#define IDS_HSRESTART                   3105
#define IDS_VALIDPASS                   3106
#define IDS_PPGENERAL                   3107
#define IDS_PPLOGON                     3108
#define IDS_PPLOGGING                   3109
#define IDS_PPJAVAVM                    3110
#define IDS_PPSETUP                     3111
#define IDS_LGPATHTITLE                 3113
#define IDS_ALLFILES                    3114
#define IDS_DLLFILES                    3115
#define IDS_EXEFILES                    3116
#define IDS_LGSTDERR                    3117
#define IDS_LGSTDOUT                    3118
#define IDS_PPJBJVM                     3119
#define IDS_PPWPATH                     3120
#define IDS_PPIMAGE                     3121
#define IDS_ERRSREG                     3122

#define IDS_PPJSS                       3200
#define IDS_PPJMS                       3201
#define IDS_PPJMX                       3202
#define IDC_PPJBCLASSPATH               3203
#define IDS_PPRTIMEOUT                  3203

#define IDS_NOTIMPLEMENTED              3199

#endif /* _PRUNMGR_H */


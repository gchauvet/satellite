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

#ifndef __VERSION_H__
#define __VERSION_H__

/**
 * Major API changes that could cause compatibility problems for older
 * programs such as structure size changes.  No binary compatibility is
 * possible across a change in the major version.
 */
#define DEIMOS_MAJOR_VERSION      2

/**
 * Minor API changes that do not cause binary compatibility problems.
 * Should be reset to 0 when upgrading DEIMOS_MAJOR_VERSION
 */
#define DEIMOS_MINOR_VERSION      0

/** patch level */
#define DEIMOS_PATCH_VERSION      0

/**
 *  This symbol is defined for internal, "development" copies of DEIMOS.
 *  This symbol will be #undef'd for releases.
 */
#define DEIMOS_IS_DEV_VERSION     1

/** Properly quote a value as a string in the C preprocessor */
#define DEIMOS_STRINGIFY(n) DEIMOS_STRINGIFY_HELPER(n)
/** Helper macro for DEIMOS_STRINGIFY */
#define DEIMOS_STRINGIFY_HELPER(n) #n


/** The formatted string of APU's version */
#define DEIMOS_VERSION_STRING \
     DEIMOS_STRINGIFY(DEIMOS_MAJOR_VERSION) "."   \
     DEIMOS_STRINGIFY(DEIMOS_MINOR_VERSION) "."   \
     DEIMOS_STRINGIFY(DEIMOS_PATCH_VERSION)       \
     DEIMOS_IS_DEV_STRING

/** Internal: string form of the "is dev" flag */
#if DEIMOS_IS_DEV_VERSION
#define DEIMOS_IS_DEV_STRING "-dev"
#else
#define DEIMOS_IS_DEV_STRING ""
#endif

#endif /* __VERSION_H__ */


/*
 *  Licensed to the Apache Software Foundation (ASF) under one or more
 *  contributor license agreements.  See the NOTICE file distributed with
 *  this work for additional information regarding copyright ownership.
 *  The ASF licenses this file to You under the Apache License, Version 2.0
 *  (the "License"); you may not use this file except in compliance with
 *  the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */
package org.apache.commons.daemon.impl;

import java.io.ByteArrayOutputStream;
import java.util.zip.ZipEntry;
import java.util.zip.ZipOutputStream;
import org.junit.Before;
import org.junit.Test;
import static org.junit.Assert.*;

public final class EmbeddedClassLoaderTest {

    private EmbeddedClassLoader instance;

    @Before
    public void setUp() throws Exception {
        final ByteArrayOutputStream baos = new ByteArrayOutputStream();
        final ZipOutputStream zos = new ZipOutputStream(baos);
        ZipEntry entry = new ZipEntry("org/apache/commons/daemon/embedded.txt");

        zos.putNextEntry(entry);
        zos.closeEntry();
        instance = new EmbeddedClassLoader(baos.toByteArray());
    }

    @Test
    public void getResourceAsStream() {
        assertNull(instance.getResourceAsStream("undefined.properties"));
        assertNotNull(instance.getResourceAsStream("org/apache/commons/daemon/embedded.txt"));
    }

}

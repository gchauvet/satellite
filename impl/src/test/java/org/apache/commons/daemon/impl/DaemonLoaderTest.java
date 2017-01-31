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

import java.io.BufferedInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import org.apache.commons.compress.archivers.ArchiveOutputStream;
import org.apache.commons.compress.archivers.ArchiveStreamFactory;
import org.apache.commons.compress.archivers.zip.ZipArchiveEntry;
import org.apache.commons.compress.utils.IOUtils;
import org.apache.commons.daemon.Daemon;
import org.apache.commons.daemon.DaemonContext;
import org.apache.commons.daemon.DaemonInitException;
import org.apache.commons.daemon.DaemonUserSignal;
import org.apache.commons.io.FileUtils;
import org.junit.AfterClass;
import org.junit.Before;
import org.junit.Test;
import static org.junit.Assert.*;
import org.junit.BeforeClass;

public final class DaemonLoaderTest {

    private DaemonLoader instance;
    private static File filename;

    @BeforeClass
    public static void setBefore() throws Exception {
        filename = File.createTempFile("archive", ".jar");
        filename.deleteOnExit();
        final FileOutputStream out = new FileOutputStream(filename);
        final File source = new File("target/test-classes");
        ArchiveOutputStream archive = new ArchiveStreamFactory().createArchiveOutputStream(ArchiveStreamFactory.ZIP, out);
        for (File file : FileUtils.listFiles(source, null, true)) {
            ZipArchiveEntry entry = new ZipArchiveEntry(source.toURI().relativize(file.toURI()).getPath());
            archive.putArchiveEntry(entry);
            BufferedInputStream input = new BufferedInputStream(new FileInputStream(file));
            IOUtils.copy(input, archive);
            input.close();
            archive.closeArchiveEntry();
        }
        archive.close();
    }

    @AfterClass
    public static void setAfter() {
        filename.delete();
    }

    @Before
    public void setUp() {
        instance = new DaemonLoader(DaemonLoader.class.getClassLoader());
        assertTrue(instance.load(filename.getPath(), null));
    }

    @Test
    public void check() {
        assertTrue(instance.check(FakeDaemonImpl.class.getName()));
        assertFalse(instance.check("org.apache.undefined"));
    }

    @Test
    public void signal() {
        assertTrue(instance.signal());
    }

    @Test
    public void start() {
        assertTrue(instance.start());
    }

    @Test
    public void stop() {
        assertTrue(instance.stop());
    }

    @Test
    public void destroy() {
        assertTrue(instance.destroy());
    }

    public static final class FakeDaemonImpl implements Daemon, DaemonUserSignal {

        public void init(DaemonContext context) throws DaemonInitException, Exception {
        }

        public void start() throws Exception {
        }

        public void stop() throws Exception {
        }

        public void destroy() {
        }

        public void signal() {
        }

    }

}

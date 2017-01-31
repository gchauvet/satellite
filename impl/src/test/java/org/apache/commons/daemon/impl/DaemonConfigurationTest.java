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

import java.io.File;
import java.net.URISyntaxException;
import org.junit.*;
import static org.junit.Assert.*;

public final class DaemonConfigurationTest {

    private DaemonConfiguration instance;

    @Before
    public void setUp() {
        instance = new DaemonConfiguration();
    }

    @Test
    public void loadDefaultFile() {
        instance.load(null);
    }

    @Test
    public void loadSpecificFile() throws URISyntaxException {
        instance.load(new File(getClass().getResource("empty.properties").toURI()));
    }

    @Test
    public void getProperty() throws Exception {
        instance.load(new File(getClass().getResource("test.properties").toURI()));
        assertNull(instance.getProperty(null));
        assertNull(instance.getProperty("undefined"));
        assertEquals("bar", instance.getProperty("foo"));
    }

    @Test
    public void testGetPropertyArray() throws Exception {
        instance.load(new File(getClass().getResource("test.properties").toURI()));
        assertTrue(instance.getPropertyArray(null).length == 0);
        assertTrue(instance.getPropertyArray("undefined").length == 0);
        assertArrayEquals(new String[]{"one", "two"}, instance.getPropertyArray("array"));
    }

}

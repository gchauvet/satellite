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
package org.apache.commons.daemon.maven;

import java.io.File;
import java.rmi.NotBoundException;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.rmi.server.UnicastRemoteObject;
import org.apache.commons.daemon.Daemon;
import org.apache.maven.plugin.Mojo;
import org.apache.maven.plugin.MojoExecutionException;
import org.apache.maven.plugin.testing.MojoRule;
import static org.junit.Assert.*;
import org.junit.*;
import org.junit.runner.RunWith;
import org.mockito.InjectMocks;
import org.mockito.Mock;
import static org.mockito.Mockito.*;
import org.mockito.runners.MockitoJUnitRunner;

@RunWith(MockitoJUnitRunner.class)
public final class StopDaemonMojoTest {

    @Rule
    public MojoRule rule = new MojoRule();
    @Mock
    private Daemon daemon;
    @InjectMocks
    private DaemonRemoteProxy proxy;
    private static Registry registry;

    @BeforeClass
    public static void setUpClass() throws Exception {
        registry = LocateRegistry.createRegistry(1099);
    }

    @Before
    public void setUp() throws Exception {
        registry.rebind(AbstractDaemonMojo.RMI_PATH, UnicastRemoteObject.exportObject(proxy, 0));
    }

    @After
    public void shutDown() throws Exception {
        try {
            registry.unbind(AbstractDaemonMojo.RMI_PATH);
            UnicastRemoteObject.unexportObject(proxy, true);
        } catch (NotBoundException ex) {
            // Normal case
        }
    }

    @Test
    public void stop() throws Exception {
        final File pom = new File(getClass().getResource("/stop.xml").toURI());
        assertTrue(pom.exists());
        final Mojo mojo = rule.lookupMojo("stop", pom);
        assertNotNull(mojo);
        mojo.execute();
        verify(daemon, times(1)).stop();
        verify(daemon, times(1)).destroy();
    }

    @Test(expected = MojoExecutionException.class)
    public void error() throws Exception {
        shutDown();
        final File pom = new File(getClass().getResource("/stop.xml").toURI());
        assertTrue(pom.exists());
        final Mojo mojo = rule.lookupMojo("stop", pom);
        assertNotNull(mojo);
        mojo.execute();
    }

}

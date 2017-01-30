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
import org.apache.maven.plugin.Mojo;
import org.apache.maven.plugin.MojoExecutionException;
import org.apache.maven.plugin.testing.MojoRule;
import static org.junit.Assert.*;
import org.junit.*;
import org.junit.runner.RunWith;
import org.mockito.runners.MockitoJUnitRunner;

@RunWith(MockitoJUnitRunner.class)
public final class StartDaemonMojoTest {

    @Rule
    public MojoRule rule = new MojoRule();

    @Test
    @Ignore // NPE because MavenProject not yet injected
    public void start() throws Exception {
        final File pom = new File(getClass().getResource("/start.xml").toURI());
        assertTrue(pom.exists());
        final Mojo mojo = rule.lookupMojo("start", pom);
        assertNotNull(mojo);
        mojo.execute();
    }

    @Test(expected = MojoExecutionException.class)
    public void error() throws Exception {
        final File pom = new File(getClass().getResource("/error.xml").toURI());
        assertTrue(pom.exists());
        final Mojo mojo = rule.lookupMojo("start", pom);
        assertNotNull(mojo);
        mojo.execute();
    }

}

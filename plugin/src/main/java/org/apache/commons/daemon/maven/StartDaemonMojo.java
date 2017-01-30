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
import java.net.URL;
import java.net.URLClassLoader;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.rmi.server.UnicastRemoteObject;
import java.util.HashSet;
import java.util.Set;
import org.apache.commons.daemon.Daemon;
import org.apache.commons.daemon.DaemonContext;
import org.apache.commons.daemon.DaemonController;
import org.apache.maven.plugin.MojoExecutionException;
import org.apache.maven.plugins.annotations.Mojo;
import org.apache.maven.plugins.annotations.Parameter;
import org.apache.maven.plugins.annotations.ResolutionScope;
import org.apache.maven.project.MavenProject;

@Mojo(
        name = "start",
        threadSafe = true,
        requiresDependencyResolution = ResolutionScope.COMPILE_PLUS_RUNTIME
)
public final class StartDaemonMojo extends AbstractDaemonMojo {

    @Parameter
    private String[] arguments;

    @Parameter(defaultValue = "${project}", readonly = true, required = true)
    private MavenProject project;

    private Daemon getDaemon(String clazz) throws Exception {
        final Set<URL> urls = new HashSet<URL>();
        for (String element : project.getCompileClasspathElements()) {
            urls.add(new File(element).toURI().toURL());
        }
        final URLClassLoader loader = URLClassLoader.newInstance(urls.toArray(new URL[0]), Thread.currentThread().getContextClassLoader());
        return (Daemon) Class.forName(clazz, true, loader).newInstance();
    }

    @Override
    public void execute() throws MojoExecutionException {
        try {
            final DaemonRemote instance = new DaemonRemoteProxy(getDaemon(daemon));
            final Registry registry = LocateRegistry.createRegistry(port);
            registry.rebind(RMI_PATH, UnicastRemoteObject.exportObject(instance, 0));
            instance.init(new DaemonContext() {
                @Override
                public DaemonController getController() {
                    throw new UnsupportedOperationException();
                }

                @Override
                public String[] getArguments() {
                    return arguments != null ? arguments : new String[0];
                }
            });
            instance.start();
        } catch (Exception ex) {
            throw new MojoExecutionException(ex.getMessage(), ex);
        }
    }
}

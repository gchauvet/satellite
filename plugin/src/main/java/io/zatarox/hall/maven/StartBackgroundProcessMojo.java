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
package io.zatarox.hall.maven;

import io.zatarox.hall.BackgroundContext;
import io.zatarox.hall.BackgroundController;
import io.zatarox.hall.BackgroundProcess;
import java.io.File;
import java.net.URL;
import java.net.URLClassLoader;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.rmi.server.UnicastRemoteObject;
import java.util.HashSet;
import java.util.Set;
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
public final class StartBackgroundProcessMojo extends AbstractBackgroundProcessMojo {

    @Parameter
    private String[] arguments;

    @Parameter(defaultValue = "${project}", readonly = true, required = true)
    private MavenProject project;

    private BackgroundProcess getDaemon(String clazz) throws Exception {
        final Set<URL> urls = new HashSet<URL>();
        for (String element : project.getCompileClasspathElements()) {
            urls.add(new File(element).toURI().toURL());
        }
        final URLClassLoader loader = URLClassLoader.newInstance(urls.toArray(new URL[0]), Thread.currentThread().getContextClassLoader());
        return (BackgroundProcess) Class.forName(clazz, true, loader).newInstance();
    }

    @Override
    public void execute() throws MojoExecutionException {
        try {
            final BackgroundProcessRemote instance = new BackgroundProcessRemoteProxy(getDaemon(entryPoint));
            final Registry registry = LocateRegistry.createRegistry(port);
            registry.rebind(RMI_PATH, UnicastRemoteObject.exportObject(instance, 0));
            instance.init(new BackgroundContext() {
                @Override
                public BackgroundController getController() {
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

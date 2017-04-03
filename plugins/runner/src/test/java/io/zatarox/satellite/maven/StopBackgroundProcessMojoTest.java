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
package io.zatarox.satellite.maven;

import io.zatarox.satellite.BackgroundProcess;
import java.io.File;
import java.rmi.NotBoundException;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.rmi.server.UnicastRemoteObject;
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
public final class StopBackgroundProcessMojoTest {

    @Rule
    public MojoRule rule = new MojoRule();
    @Mock
    private BackgroundProcess instance;
    @InjectMocks
    private BackgroundProcessRemoteProxy proxy;
    private static Registry registry;

    @BeforeClass
    public static void setUpClass() throws Exception {
        registry = LocateRegistry.createRegistry(1099);
    }

    @Before
    public void setUp() throws Exception {
        registry.rebind(AbstractBackgroundProcessMojo.RMI_PATH, UnicastRemoteObject.exportObject(proxy, 0));
    }

    @After
    public void shutDown() throws Exception {
        try {
            registry.unbind(AbstractBackgroundProcessMojo.RMI_PATH);
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
        verify(instance, times(1)).pause();
        verify(instance, times(1)).shutdown();
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

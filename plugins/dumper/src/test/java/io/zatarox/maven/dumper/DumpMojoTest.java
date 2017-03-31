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
package io.zatarox.maven.dumper;

import java.io.File;
import java.io.FileInputStream;
import java.util.HashMap;
import org.apache.commons.codec.digest.DigestUtils;
import org.apache.commons.lang3.SystemUtils;
import org.apache.maven.model.Build;

import org.apache.maven.plugin.testing.AbstractMojoTestCase;
import org.apache.maven.project.MavenProject;
import org.junit.Before;
import static org.mockito.Mockito.*;
import org.mockito.MockitoAnnotations;

public class DumpMojoTest extends AbstractMojoTestCase {

    @Before
    @Override
    public void setUp() throws Exception {
        super.setUp();
        MockitoAnnotations.initMocks(this);
    }

    public void testMojoLookup() throws Exception {
        File pluginXml = new File(getBasedir(), "src/test/resources/plugin-config.xml");
        DumpMojo mojo = (DumpMojo) lookupMojo("dump", pluginXml);
        assertNotNull(mojo);
    }

    public void testSources() throws Exception {
        final File pluginXml = new File(getBasedir(), "src/test/resources/plugin-config.xml");
        final DumpMojo mojo = (DumpMojo) lookupMojo("dump", pluginXml);
        assertNotNull(mojo);
        mojo.setPluginContext(new HashMap());

        final MavenProject project = mock(MavenProject.class);
        when(project.getBuild()).thenReturn(mock(Build.class));
        when(project.getBuild().getDirectory()).thenReturn("target");
        mojo.setProject(project);
        mojo.execute();

        boolean unix = SystemUtils.OS_NAME.contains("nix");
        assertEquals(unix ? "6e826326a65e54bf18f4498fec20378b" : "fa7600b6d98b87be6c031abdd5d4c518", DigestUtils.md5Hex(new FileInputStream(new File("target/generated-sources/c/dump.h"))));
        assertEquals(unix ? "a71bfe7718acb5461d2959377c8a8f4e" : "55335f89ac14423b30e34bc43c7a1ec1", DigestUtils.md5Hex(new FileInputStream(new File("target/generated-sources/c/dump.c"))));
    }
}

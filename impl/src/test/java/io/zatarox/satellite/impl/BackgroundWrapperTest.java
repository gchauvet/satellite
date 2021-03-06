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
package io.zatarox.satellite.impl;

import io.zatarox.satellite.*;
import java.io.File;
import java.io.FileOutputStream;
import java.util.jar.Attributes;
import java.util.jar.Manifest;
import org.apache.commons.compress.archivers.ArchiveOutputStream;
import org.apache.commons.compress.archivers.ArchiveStreamFactory;
import org.apache.commons.compress.archivers.zip.ZipArchiveEntry;
import org.junit.*;
import static org.junit.Assert.*;
import org.junit.runner.RunWith;
import org.powermock.core.classloader.annotations.PrepareForTest;
import org.powermock.modules.junit4.PowerMockRunner;

@RunWith(PowerMockRunner.class)
@PrepareForTest(BackgroundWrapper.class)
public final class BackgroundWrapperTest {

    private BackgroundWrapper instance;
    private static File file;
    private static boolean raiseException;

    @BeforeClass
    public static void setBefore() throws Exception {
        file = File.createTempFile("archive", ".jar");
        file.deleteOnExit();
        final FileOutputStream out = new FileOutputStream(file);
        ArchiveOutputStream archive = new ArchiveStreamFactory().createArchiveOutputStream(ArchiveStreamFactory.ZIP, out);
        ZipArchiveEntry entry = new ZipArchiveEntry("META-INF/MANIFEST.MF");
        archive.putArchiveEntry(entry);
        final Manifest manifest = new Manifest();
        manifest.getMainAttributes().put(Attributes.Name.MANIFEST_VERSION, "1.0");
        manifest.getMainAttributes().putValue("Background-Process-Class", FakeBackgroundProcessImpl.class.getName());
        manifest.write(archive);
        archive.closeArchiveEntry();
        archive.close();
    }

    @AfterClass
    public static void setAfter() {
        file.delete();
    }

    @Before
    public void setUp() {
        instance = new BackgroundWrapper(BackgroundWrapper.class.getClassLoader());
        raiseException = false;
        assertTrue(instance.load(file.getPath(), null));
    }

    @Test(expected = IllegalArgumentException.class)
    public void noClassloaderProvided() {
        instance = new BackgroundWrapper(null);
        fail();
    }

    @Test
    public void check() {
        assertTrue(instance.check(FakeBackgroundProcessImpl.class.getName()));
        assertFalse(instance.check("io.zatarox.satellite.undefined"));
    }

    @Test(expected = NullPointerException.class)
    public void checkNull() {
        instance.check(null);
        fail();
    }

    @Test
    public void resume() {
        assertTrue(instance.resume());
    }

    @Test
    public void pause() {
        assertTrue(instance.pause());
    }

    @Test
    public void destroy() {
        assertTrue(instance.shutdown());
    }

    @Test(expected = UnsupportedOperationException.class)
    public void exception() {
        raiseException = true;
        instance = new BackgroundWrapper(BackgroundWrapper.class.getClassLoader());
        assertFalse(instance.load(file.getPath(), null));
    }

    public static final class FakeBackgroundProcessImpl implements BackgroundProcess {

        @Override
        public void initialize(BackgroundContext context) throws BackgroundException, Exception {
            if (raiseException) {
                throw new BackgroundException("Test", new IllegalArgumentException("tester"));
            }
        }

        @Override
        public void resume() throws Exception {
        }

        @Override
        public void pause() throws Exception {
        }

        @Override
        public void shutdown() {
        }
    }

}

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
package javax.daemon;

import java.io.BufferedInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import org.apache.commons.compress.archivers.ArchiveOutputStream;
import org.apache.commons.compress.archivers.ArchiveStreamFactory;
import org.apache.commons.compress.archivers.zip.ZipArchiveEntry;
import org.apache.commons.compress.utils.IOUtils;
import org.apache.commons.io.FileUtils;
import org.junit.*;
import static org.junit.Assert.*;
import org.junit.runner.RunWith;
import org.powermock.core.classloader.annotations.PrepareForTest;
import org.powermock.modules.junit4.PowerMockRunner;

@RunWith(PowerMockRunner.class)
@PrepareForTest(BackgroundWrapper.class)
public final class BackgroundWrapperTest {

    private BackgroundWrapper instance;
    private static File filename;
    private static boolean raiseException;

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
        instance = new BackgroundWrapper();
        raiseException = false;
        assertTrue(instance.load(filename.getPath(), null));
    }

    @Test
    public void check() {
        assertTrue(instance.check(FakeBackgroundProcessImpl.class.getName()));
        assertFalse(instance.check("javax.daemon.undefined"));
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
        instance = new BackgroundWrapper();
        assertFalse(instance.load(filename.getPath(), null));
    }

    public static final class FakeBackgroundProcessImpl implements BackgroundProcess {

        @Override
        public void initialize(BackgroundContext context) throws BackgroundProcessException, Exception {
            if (raiseException) {
                throw new BackgroundProcessException("Test", new IllegalArgumentException("tester"));
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

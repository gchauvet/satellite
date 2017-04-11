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

import java.io.ByteArrayOutputStream;
import java.util.zip.ZipEntry;
import java.util.zip.ZipOutputStream;
import org.junit.Before;
import org.junit.Test;
import static org.junit.Assert.*;
import org.junit.runner.RunWith;
import org.powermock.core.classloader.annotations.PrepareForTest;
import org.powermock.modules.junit4.PowerMockRunner;

@RunWith(PowerMockRunner.class)
@PrepareForTest(EmbeddedClassLoader.class)
public final class EmbeddedClassLoaderTest {

    private EmbeddedClassLoader instance;

    @Before
    public void setUp() throws Exception {
        final ByteArrayOutputStream baos = new ByteArrayOutputStream();
        final ZipOutputStream zos = new ZipOutputStream(baos);
        ZipEntry entry = new ZipEntry("io/zatarox/satellite/impl/embedded.txt");
        zos.putNextEntry(entry);
        entry = new ZipEntry(this.getClass().getName().replaceAll(".", "/") + ".class");
        zos.putNextEntry(entry);
        
        zos.closeEntry();
        instance = new EmbeddedClassLoader(baos.toByteArray());
    }

    @Test
    public void getResourceAsStream() {
        assertNull(instance.getResourceAsStream("undefined.properties"));
        assertNotNull(instance.getResourceAsStream("io/zatarox/satellite/impl/embedded.txt"));
    }
    
    
    @Test
    public void findClass() throws ClassNotFoundException{
        Class<?> obj = instance.findClass(this.getClass().getName());
        assertNotSame(EmbeddedClassLoaderTest.class, obj);
        assertEquals(EmbeddedClassLoaderTest.class.getName(), obj.getName());
    }
    
    @Test(expected = ClassNotFoundException.class)
    public void findUndefinedClass() throws ClassNotFoundException{
        instance.findClass("io.zatarox.satellite.impl.undefined");
        fail();
    }
    
    @Test
    public void createBootstrap() throws Exception {
        final ByteArrayOutputStream baos = new ByteArrayOutputStream();
        final ZipOutputStream zos = new ZipOutputStream(baos);
        ZipEntry entry = new ZipEntry(BackgroundWrapper.class.getName().replaceAll(".", "/") + ".class");
        zos.putNextEntry(entry);
        
        zos.closeEntry();
        final Object wrapper = EmbeddedClassLoader.createBootstrap(baos.toByteArray());
        assertEquals(BackgroundWrapper.class.getName(), wrapper.getClass().getName());
    }

}

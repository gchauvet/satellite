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
package io.zatarox.hall.impl;

import java.io.*;
import java.lang.reflect.Constructor;
import java.util.jar.JarEntry;
import java.util.jar.JarInputStream;

public final class EmbeddedClassLoader extends ClassLoader {

    private final byte[] content;

    EmbeddedClassLoader(final byte[] content) {
        this.content = content;
    }

    @Override
    protected Class<?> findClass(String name) throws ClassNotFoundException {
        Class<?> result = null;
        try {
            final InputStream in = getResourceAsStream(name.replace('.', '/') + ".class");
            final ByteArrayOutputStream out = new ByteArrayOutputStream();
            final byte[] buffer = new byte[4096];
            int n;
            while ((n = in.read(buffer)) > 0) {
                out.write(buffer, 0, n);
            }
            final byte[] bytes = out.toByteArray();
            result = defineClass(name, bytes, 0, bytes.length);
        } catch (Throwable ex) {
            result = super.findClass(name);
        }
        return result;
    }

    @Override
    protected Class<?> loadClass(String name, boolean resolve) throws ClassNotFoundException {
        Class<?> result;
        try {
            result = findClass(name);
            if (result != null) {
                if (resolve) {
                    resolveClass(result);
                }
            }

        } catch (Throwable ex) {
            result = super.loadClass(name, resolve);
        }
        return result;
    }

    @Override
    public InputStream getResourceAsStream(String name) {
        InputStream result = super.getResourceAsStream(name);
        if (result == null) {
            try {
                final JarInputStream jis = new JarInputStream(new ByteArrayInputStream(content));
                JarEntry entry;
                while ((entry = jis.getNextJarEntry()) != null) {
                    if (entry.getName().equals(name)) {
                        result = jis;
                        break;
                    }
                }
            } catch (IOException e) {
            }
        }
        return result;
    }

    /**
     * Instanciate a DaemonLoader object
     *
     * @param content Bytecodes of embedded jar
     * @return a DaemonLoader as an Object
     * @throws Exception
     */
    static Object createBootstrap(final byte[] content) throws Exception {
        final ClassLoader main = new EmbeddedClassLoader(content);
        final Class<?> clazz = Class.forName("io.zatarox.hall.impl.BackgroundWrapper", true, main);
        final Constructor<?> constructor = clazz.getConstructor(ClassLoader.class);
        return constructor.newInstance(main);
    }

}

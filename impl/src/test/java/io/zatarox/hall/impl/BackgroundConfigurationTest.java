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

import java.io.File;
import java.net.URISyntaxException;
import org.junit.*;
import static org.junit.Assert.*;

public final class BackgroundConfigurationTest {

    private BackgroundConfiguration instance;

    @Before
    public void setUp() {
        instance = new BackgroundConfiguration();
    }

    @Test
    public void loadDefaultFile() {
        instance.load(null);
    }

    @Test
    public void loadSpecificFile() throws URISyntaxException {
        instance.load(new File(getClass().getResource("empty.properties").toURI()));
    }

    @Test
    public void getProperty() throws Exception {
        instance.load(new File(getClass().getResource("test.properties").toURI()));
        assertNull(instance.getProperty(null));
        assertNull(instance.getProperty("undefined"));
        assertEquals("bar", instance.getProperty("foo"));
    }

    @Test
    public void testGetPropertyArray() throws Exception {
        instance.load(new File(getClass().getResource("test.properties").toURI()));
        assertTrue(instance.getPropertyArray(null).length == 0);
        assertTrue(instance.getPropertyArray("undefined").length == 0);
        assertArrayEquals(new String[]{"one", "two"}, instance.getPropertyArray("array"));
    }

}

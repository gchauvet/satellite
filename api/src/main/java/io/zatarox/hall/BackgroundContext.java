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
package io.zatarox.hall;

/**
 * Defines a set of methods that a BackgroundProcess instance can use to communicate with
 * the Daemon container.
 */
public interface BackgroundContext {

    /**
     * Returns  <code>BackgroundController</code> object that can be used to control
     * the <code>Daemon</code> instance that this <code>BackgroundContext</code> is
     * passed to.
     */
    public BackgroundController getController();

    /**
     * Returns an array of <code>String</code> arguments supplied by the
     * environment. corresponding to the array of arguments given in the
     * <code>public static void main()</code> method used as an entry point to
     * most other java programs.
     */
    public String[] getArguments();

}

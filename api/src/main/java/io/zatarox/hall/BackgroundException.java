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
 * Throw this during init if you can't initialise yourself for some expected
 * reason. Using this exception will cause the exception's message to come out
 * on stdout, rather than a dirty great stacktrace.
 */
public final class BackgroundException extends Exception {

    private static final long serialVersionUID = 5665891535067213551L;

    // don't rely on Throwable#getCause (jdk1.4)
    private final Throwable cause;

    public BackgroundException(String message) {
        super(message);
        this.cause = null;
    }

    public BackgroundException(String message, Throwable cause) {
        super(message);
        this.cause = cause;
    }

    public String getMessageWithCause() {
        String extra = this.cause == null ? "" : ": " + this.cause.getMessage();
        return getMessage() + extra;
    }

}

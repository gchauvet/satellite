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
#include "jsvc.h"

/* Replace all occurrences of a string in another */
int replace(char *new, int len, char *old, char *mch, char *rpl)
{
    char *tmp;
    int count;
    int shift;
    int nlen;
    int olen;
    int mlen;
    int rlen;
    int x;

    /* The new buffer is NULL, fail */
    if (new == NULL)
        return -1;
    /* The length of the buffer is less than zero, fail */
    if (len < 0)
        return -2;
    /* The old buffer is NULL, fail */
    if (old == NULL)
        return -3;

    /* The string to be matched is NULL or empty, simply copy */
    if ((mch == NULL) || (strlen(mch) == 0)) {
        olen = strlen(old);
        if (len <= olen)
            return (olen + 1);
        strcpy(new, old);
        return 0;
    }

    /* The string to be replaced is NULL, assume it's an empty string */
    if (rpl == NULL)
        rpl = "";

    /* Evaluate some lengths */
    olen = strlen(old);
    mlen = strlen(mch);
    rlen = strlen(rpl);

    /* Calculate how many times the mch string appears in old */
    tmp = old;
    count = 0;
    while ((tmp = strstr(tmp, mch)) != NULL) {
        count++;
        tmp += mlen;
    }

    /* We have no matches, simply copy */
    if (count == 0) {
        olen = strlen(old);
        if (len <= olen)
            return (olen + 1);
        strcpy(new, old);
        return 0;
    }

    /* Calculate how big the buffer must be to hold the translation
     * and of how many bytes we need to shift the data
     */
    shift = rlen - mlen;
    nlen  = olen + (shift * count);
    /* printf("Count=%d Shift= %d OLen=%d NLen=%d\n",count,shift,olen,nlen); */

    /* Check if we have enough size in the buffer */
    if (nlen >= len)
        return (nlen + 1);

    /* Copy over the old buffer in the new one (save memory) */
    strcpy(new, old);

    /* Start replacing */
    tmp = new;
    while ((tmp = strstr(tmp, mch)) != NULL) {
        /* If shift is > 0 we need to move data from right to left */
        if (shift > 0) {
            for (x = (strlen(tmp) + shift); x > shift; x--) {
                /*
                   printf("src %c(%d) dst %c(%d)\n",
                   tmp[x-shift],tmp[x-shift],tmp[x],tmp[x]);
                 */
                tmp[x] = tmp[x - shift];
            }
            /* If shift is < 0 we need to move data from left to right */
        }
        else if (shift < 0) {
            for (x = mlen; x < strlen(tmp) - shift; x++) {
                /*
                   printf("src %c(%d) dst %c(%d)\n",
                   tmp[x],tmp[x],tmp[x+shift],tmp[x+shift]);
                 */
                tmp[x + shift] = tmp[x];
            }
        }
        /* If shift is = 0 we don't have to shift data */
        strncpy(tmp, rpl, rlen);
        tmp += rlen;
        /* printf("\"%s\"\n",tmp); */
    }
    return 0;
}


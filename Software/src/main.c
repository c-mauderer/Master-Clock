/*
 * Copyright (c) 2024 Christian Mauderer <oss@c-mauderer.de>
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
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <avr/io.h>

FUSES =
{
	/* Watchdog disabled */
	.WDTCFG = 0,
	/* Brown Out detection disabled */
	.BODCFG = 0,
	/* Default is 20MHz. This application wants 32kHz. So not relevant. */
	.OSCCFG = FUSE_OSCCFG_DEFAULT,
	/* Make sure that UPDI is active. Otherwise use defaults. */
	.SYSCFG0 = (FUSE_SYSCFG0_DEFAULT & ~FUSE_RSTPINCFG1) | FUSE_RSTPINCFG0,
	/* Default is 64ms. That's fine. */
	.SYSCFG1 = FUSE_SYSCFG1_DEFAULT,
	/* No difference between application and boot. 0 tells it's all boot. */
	.APPEND = 0,
	.BOOTEND = 0,
};

static void
io_init(void)
{
#warning FIXME: Add I/O initialization
}

static void
timer_init(void)
{
#warning FIXME: Add I/O initialization
}

int
main (void)
{
	io_init();
	timer_init();

	while (1) {
#warning FIXME: Sleep
	}

	return 0;
}

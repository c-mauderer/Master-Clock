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

#define F_CPU 32768ul

#include <avr/io.h>
#include <avr/cpufunc.h>
#include <avr/sleep.h>
#include <util/delay.h>

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

#define LED_MASK (1 << 2)
#define KEY_STEP_MASK (1 << 6)
#define KEY_PAUSE_MASK (1 << 7)

#define LED_ON do { PORTA.OUTCLR = LED_MASK; } while(0)
#define LED_OFF do { PORTA.OUTSET = LED_MASK; } while(0)
#define LED_TOGGLE do { PORTA.OUTTGL = LED_MASK; } while(0)

#define KEY_STEP_GET ((PORTA.IN & KEY_STEP_MASK) != 0)
#define KEY_PAUSE_GET ((PORTA.IN & KEY_PAUSE_MASK) != 0)

/*
 * Setup I/Os:
 *
 * - PA0: Reset/UPDI -> Other/Special: UPDI
 * - PA1: Minute Pulse Output -> TCA0:WO1 or GPIO out
 * - PA2: LED Output -> GPIO out
 * - PA3: 32kHz Input -> Other/Special: EXTCLK
 * - PA6: Key STEP -> GPIO in
 * - PA7: Key PAUSE -> GPIO in
 */
static void
io_init(void)
{
	PORTA.DIRSET = LED_MASK;
	LED_OFF;

	PORTA.DIRCLR = KEY_STEP_MASK | KEY_PAUSE_MASK;
}

/*
 * According to Data Sheet, the pin for EXTCLK is automatically configured if
 * any peripheral is requesting this clock. So this has only to switch to
 * external clock.
 */
static void
clock_init(void)
{
	/* Switch to external 32kHz clock */
	ccp_write_io((uint8_t *)&CLKCTRL.MCLKCTRLA, CLKCTRL_CLKSEL_EXTCLK_gc);

	/* Disable prescaler */
	ccp_write_io((uint8_t *)&CLKCTRL.MCLKCTRLB, 0);
}

/*
 * Enable power down mode for sleep. Only BOD, WDT and PIT are active.
 */
static void
standby_init(void)
{
	SLPCTRL.CTRLA = SLPCTRL_SMODE_PDOWN_gc;
}

static void
timer_init(void)
{
#warning FIXME: Add timer initialization
}

int
main (void)
{
	clock_init();
	io_init();
	timer_init();
	standby_init();

	for(int i = 0; i < 4; ++i) {
		LED_TOGGLE;
		_delay_ms(100);
	}
	LED_OFF;

	while (1) {
		sleep_mode();
	}

	return 0;
}

#include "halmcu/ll/clk.h"

#include "halmcu/assert.h"
#include "halmcu/bitop.h"
#include "halmcu/compiler.h"
#include "halmcu/asm/arm/cmsis.h"
#include "a33g.h"

#define PLLCON_UNLOCK_KEY			0x80750000U

#define MHZ					1000000U

#define VCO_MAX_MUL				256U
#define VCO_MAX_DIV				16U

#define PLLCLKSEL_POS				4U
#define PLLCLKDIV_POS				5U
#define HCLKDIV_POS				8U
#define PCLKDIV_POS				10U
#define MXOSCMNT_POS				8U
#define VCO_N1_POS				20U
#define VCO_N2_POS				16U
#define VCO_MODE_POS				28U
#define PREDIV_POS				8U
#define POSTDIV_POS				0U

#define PLLCLKSEL_MASK				(1U << PLLCLKSEL_POS)
#define PLLCLKDIV_MASK				(1U << PLLCLKDIV_POS)
#define HCLKDIV_MASK				(3U << HCLKDIV_POS)
#define PCLKDIV_MASK				(1U << PCLKDIV_POS)
#define VCO_N1_MASK				(0xFFU << VCO_N1_POS)
#define VCO_N2_MASK				(0xFU << VCO_N2_POS)
#define VCO_MODE_MASK				(0x1U << VCO_MODE_POS)
#define PREDIV_MASK				(0x7U << PREDIV_POS)
#define POSTDIV_MASK				(0xFU << POSTDIV_POS)

#define MXOSCSTS				1U
#define MXOSCMNT				(1U << MXOSCMNT_POS)
#define nBYPASS					(1U << 13)
#define PLLEN					(1U << 14)
#define PLLRST					(1U << 15)
#define PLLLOCKSTS				(1U << 12)
#define MCKMNT					(1U << 8)

static void unlock_pllcon(void)
{
	PMU->PLLCON = PLLCON_UNLOCK_KEY;
}

static uint32_t get_activation_bitmask_from_peripheral(periph_t peri)
{
	switch (peri) {
	case PERIPH_ADC:
		return 1U << 28;
	case PERIPH_PWM7: /* fall through */
	case PERIPH_PWM6: /* fall through */
	case PERIPH_PWM5: /* fall through */
	case PERIPH_PWM4:
		return 1U << 25;
	case PERIPH_PWM3: /* fall through */
	case PERIPH_PWM2: /* fall through */
	case PERIPH_PWM1: /* fall through */
	case PERIPH_PWM0:
		return 1U << 24;
	case PERIPH_UART3:
		return 1U << 23;
	case PERIPH_UART2:
		return 1U << 22;
	case PERIPH_UART1:
		return 1U << 21;
	case PERIPH_UART0:
		return 1U << 20;
	case PERIPH_I2C1:
		return 1U << 19;
	case PERIPH_I2C0 + 0:
		return 1U << 18;
	case PERIPH_SPI1:
		return 1U << 17;
	case PERIPH_SPI0 + 0:
		return 1U << 16;
	case PERIPH_GPIOA: /* fall through */
	case PERIPH_GPIOB: /* fall through */
	case PERIPH_GPIOC: /* fall through */
	case PERIPH_GPIOD: /* fall through */
	case PERIPH_GPIOE: /* fall through */
	case PERIPH_GPIOF:
		return 1U << 8;
	case PERIPH_TIMER9: /* fall through */
	case PERIPH_TIMER8: /* fall through */
	case PERIPH_TIMER7: /* fall through */
	case PERIPH_TIMER6:
		return 1U << 7;
	case PERIPH_TIMER5: /* fall through */
	case PERIPH_TIMER4: /* fall through */
	case PERIPH_TIMER3: /* fall through */
	case PERIPH_TIMER2:
		return 1U << 6;
	case PERIPH_TIMER1: /* fall through */
	case PERIPH_TIMER0:
		return 1U << 5;
	case PERIPH_FRT:
		return 1U << 4;
	case PERIPH_WDT:
		return 1U << 3;
	default:
		return 0;
	}
}

static uint32_t get_bitmask_from_clksrc(clk_source_t clk)
{
	switch (clk) {
	case CLK_LSI:
		return 1U << 6;
	case CLK_HSI:
		return 1U << 4;
	case CLK_LSE:
		return 1U << 2;
	case CLK_HSE:
		return 1U << 0;
	default:
		return 0;
	}
}

static bool clk_set_pll_source(clk_source_t clk)
{
	uint32_t val = PMU->BCCR & ~PLLCLKSEL_MASK;

	switch (clk) {
	case CLK_HSE:
		val |= PLLCLKSEL_MASK;
		break;
	case CLK_HSI:
		break;
	default:
		return false;
	}

	PMU->BCCR = val;
	return true;
}

static clk_source_t clk_get_pll_source(void)
{
	if (PMU->BCCR & PLLCLKSEL_MASK) {
		return CLK_HSE;
	}
	return CLK_HSI;
}

static uint32_t clk_get_pll_post_multifactor(void)
{
	return (PMU->PLLCON & VCO_N1_MASK) >> VCO_N1_POS;
}

static uint32_t clk_get_pll_post_divfactor(void)
{
	return (PMU->PLLCON & VCO_N2_MASK) >> VCO_N2_POS;
}

static uint32_t clk_get_pll_vco_mode(void)
{
	return (PMU->PLLCON & VCO_MODE_MASK) >> VCO_MODE_POS;
}

static uint32_t clk_get_pll_pre_divfactor(void)
{
	return (PMU->PLLCON & PREDIV_MASK) >> PREDIV_POS;
}

static uint32_t clk_get_pll_divfactor(void)
{
	return (PMU->PLLCON & POSTDIV_MASK) >> POSTDIV_POS;
}

static uint32_t clk_get_pclk_prescaler(void)
{
	return (PMU->BCCR & PCLKDIV_MASK) >> PCLKDIV_POS;
}

static uint32_t clk_get_hclk_prescaler(void)
{
	return (PMU->BCCR & HCLKDIV_MASK) >> HCLKDIV_POS;
}

static uint32_t clk_get_pll_prescaler(void)
{
	return (PMU->BCCR & PLLCLKDIV_MASK) >> PLLCLKDIV_POS;
}

static void clk_set_pll_prescaler(uint32_t div_factor)
{
	bitop_clean_set_with_mask(&PMU->BCCR, PLLCLKDIV_POS, 1U, div_factor);
}

static void clk_set_hclk_prescaler(uint32_t div_factor)
{
	bitop_clean_set_with_mask(&PMU->BCCR, HCLKDIV_POS, 3U, div_factor);
}

static void clk_set_pclk_prescaler(uint32_t div_factor)
{
	bitop_clean_set_with_mask(&PMU->BCCR, PCLKDIV_POS, 1U, div_factor);
}

static void clk_set_pll_post_multifactor(uint32_t mul_factor) // N1
{
	bitop_clean_set_with_mask(&PMU->PLLCON, VCO_N1_POS, 0xffU, mul_factor);
}

static void clk_set_pll_post_divfactor(uint32_t div_factor) // N2
{
	bitop_clean_set_with_mask(&PMU->PLLCON, VCO_N2_POS, 0xfU, div_factor);
}

static void clk_set_pll_pre_divfactor(uint32_t div_factor) // R
{
	bitop_clean_set_with_mask(&PMU->PLLCON, PREDIV_POS, 7U, div_factor);
}

static void clk_set_pll_divfactor(uint32_t div_factor) // P
{
	bitop_clean_set_with_mask(&PMU->PLLCON, POSTDIV_POS, 0xfU, div_factor);
}

static void clk_set_pll_vco_mode(uint32_t mode) // D
{
	bitop_clean_set_with_mask(&PMU->PLLCON, VCO_MODE_POS, 1U, mode);
}

static bool configure_pll(clk_source_t clkin)
{
	if (!clk_set_pll_source(clkin)) {
		return false;
	}

	clk_ll_enable_source(clkin);

	clk_set_hclk_prescaler(0);
	clk_set_pclk_prescaler(0);
	clk_set_pll_prescaler(0);

	return true;
}

static bool calculate_n1_n2(uint32_t target_hz, uint32_t *n1, uint32_t *n2)
{
	uint32_t out, mul, div;
	mul = div = 1;

	do {
		out = ((2 * mul) / div) * MHZ;
		if (out == target_hz) {
			*n1 = mul - 1;
			*n2 = div - 1;
			return true;
		} else if (out > target_hz) {
			div++;
		} else {
			mul++;
		}
	} while (mul <= VCO_MAX_MUL && div <= VCO_MAX_DIV);

	return false;
}

static void set_pll_factors(uint32_t src_hz, uint32_t target_hz)
{
	uint32_t r = src_hz / (2*MHZ) - 1;
	assert(r < 8); // 3-bit PREDIV
	assert(src_hz / (r+1) == 2*MHZ);

	uint32_t n1 = 0;
	uint32_t n2 = 0;
	bool ok = calculate_n1_n2(target_hz, &n1, &n2);
	unused(ok);
	assert(ok);
	assert(2*(n1+1) <= 200); // NOTE: 50 <= VCO <= 200MHz, 100 <= VCOx2 <= 250MHz
	assert(2*(n1+1)/(n2+1) <= 90); // NOTE: 8 <= out frequency <= 90MHz

	clk_set_pll_vco_mode(0); // D
	clk_set_pll_divfactor(0); // P
	clk_set_pll_pre_divfactor(r);
	clk_set_pll_post_multifactor(n1);
	clk_set_pll_post_divfactor(n2);
}

static clk_source_t clk_get_source_internal(void)
{
	uint32_t val = PMU->BCCR & 0x3;

	switch (val) {
	case 1:
		return CLK_LSE;
	case 2:
		return CLK_PLL;
	case 3:
		return CLK_PLL_BYPASS;
	default:
		return CLK_LSI;
	}
}

static uint32_t clk_get_pll_frequency(void)
{
	uint32_t clkin, n1, n2, d, r, p;

	switch (clk_get_source_internal()) {
	case CLK_LSI:
		return 1*MHZ;
	case CLK_PLL:
		if (clk_get_pll_source() == CLK_HSE) {
			clkin = F_HSE;
		} else {
			clkin = 16*MHZ;
		}
		clkin >>= clk_get_pll_prescaler();
		n1 = clk_get_pll_post_multifactor() + 1;
		n2 = clk_get_pll_post_divfactor() + 1;
		d = clk_get_pll_vco_mode() + 1;
		r = clk_get_pll_pre_divfactor() + 1;
		p = clk_get_pll_divfactor() + 1;
		return (clkin * n1) / (r * n2 * p) * d;
	case CLK_PLL_BYPASS:
		if (clk_get_pll_source() == CLK_HSE) {
			return F_HSE;
		}
		return 16*MHZ; /* HSI */
	case CLK_LSE:
	default:
		return 0;
	}
}

static clk_source_t clk_get_timer_source(periph_t timer)
{
	uint32_t pos = 4;

	switch (timer) {
	case PERIPH_TIMER0: /* fall through */
	case PERIPH_TIMER1:
		pos = 4;
		break;
	case PERIPH_TIMER2: /* fall through */
	case PERIPH_TIMER3: /* fall through */
	case PERIPH_TIMER4: /* fall through */
	case PERIPH_TIMER5:
		pos = 6;
		break;
	case PERIPH_TIMER6: /* fall through */
	case PERIPH_TIMER7: /* fall through */
	case PERIPH_TIMER8: /* fall through */
	case PERIPH_TIMER9:
		pos = 8;
		break;
	default:
		return CLK_PLL;
	}

	switch ((PMU->PCSR >> pos) & 0x3) {
	case 1:
		return CLK_HSI;
	case 2:
		return CLK_LSE;
	case 3:
		return CLK_LSI;
	case 0: /* fall through */
	default:
		return CLK_PLL;
	}
}

void clk_ll_enable_peripheral(periph_t peri)
{
	PMU->PCCR |= get_activation_bitmask_from_peripheral(peri);
}

void clk_ll_disable_peripheral(periph_t peri)
{
	PMU->PCCR &= ~get_activation_bitmask_from_peripheral(peri);
}

void clk_ll_enable_source(clk_source_t clk)
{
	uint32_t val = PMU->CCR;
	uint32_t mask = get_bitmask_from_clksrc(clk);
	val &= ~(mask * 3);
	val |= mask << 1;
	PMU->CCR = val;

	if (clk == CLK_HSE) {
		PMU->CMR |= MXOSCMNT; // activate MXOSC monitoring
		PMU->CMR |= MXOSCSTS; // clear
		while ((PMU->CMR & MXOSCSTS) == 0) {
			/* wait until getting xtal stable */
		}
	}
}

void clk_ll_disable_source(clk_source_t clk)
{
	uint32_t val = PMU->CCR;
	uint32_t mask = get_bitmask_from_clksrc(clk);
	val &= ~(mask * 3);
	PMU->CCR = val;
}

void clk_ll_set_source(clk_source_t clk)
{
	uint32_t mask = 3U;
	uint32_t val = PMU->BCCR;
	val &= ~mask;

	switch (clk) {
	case CLK_LSI:
		break;
	case CLK_LSE:
		val |= 1U;
		break;
	case CLK_PLL:
		val |= 2U;
		break;
	case CLK_PLL_BYPASS:
		val |= 3U;
		break;
	default:
		return;
	}

	PMU->BCCR = val;
}

clk_source_t clk_ll_get_source(void)
{
	return clk_get_source_internal();
}

void clk_ll_start_pll(void)
{
	PMU->PLLCON |= PLLEN | PLLRST | nBYPASS;
	PMU->MCMR |= MCKMNT;
}

void clk_ll_stop_pll(void)
{
	PMU->PLLCON &= ~(PLLEN | PLLRST | nBYPASS);
}

bool clk_ll_set_pll_frequency(clk_source_t clk, clk_source_t clkin, uint32_t hz)
{
	uint32_t src_hz = 16*MHZ; // CLK_HSI

	assert(hz % MHZ == 0);
	unlock_pllcon();

	switch (clk) {
	case CLK_PLL:
		if (!configure_pll(clkin)) {
			return false;
		}
		if (clkin == CLK_HSE) {
			// NOTE: according to UM_A33G52x: 4 <= xtal <= 8MHz
			assert(F_HSE >= 4*MHZ && F_HSE <= 8*MHZ);
			src_hz = F_HSE;
		}
		break;
	case CLK_LSI:
	case CLK_LSE:
	case CLK_PLL_BYPASS:
	default:
		return false;
	}

	set_pll_factors(src_hz, hz);

	return true;
}

uint32_t clk_ll_get_hclk_frequency(void)
{
	uint32_t div = clk_get_hclk_prescaler();
	if (div & 0x2) {
		div = (div & 1)? 4 : 2;
	} else {
		div = 1;
	}
	return clk_get_pll_frequency() / div;
}

uint32_t clk_ll_get_pclk_frequency(void)
{
	return clk_get_pll_frequency() / (clk_get_pclk_prescaler() + 1);
}

uint32_t clk_ll_get_frequency(clk_source_t clk)
{
	switch (clk) {
	case CLK_PLL:
		return clk_ll_get_hclk_frequency();
	case CLK_LSI:
		return 1*MHZ;
	case CLK_HSI:
		return 16*MHZ;
	case CLK_HSE:
		return F_HSE;
	case CLK_LSE:
	default:
		return 0;
	}
}

clk_source_t clk_ll_get_peripheral_clock_source(periph_t peri)
{
	return clk_get_timer_source(peri);
}

bool clk_ll_is_pll_locked(void)
{
	return !!(PMU->PLLCON & PLLLOCKSTS);
}

void clk_ll_reset(void)
{
	PMU->PCCR = 0x118;
	PMU->CCR = 0x80;
	PMU->CMR = 0;
	PMU->MCMR = 0;
	PMU->BCCR = 0;
	PMU->PCSR = 0;
	PMU->COR = 0;
	PMU->PLLCON = 0;
	PMU->IOSC16TRIM = 0x228B;
	PMU->EOSCCON = 1;
}

uint32_t clk_ll_get_peripheral_clock_source_frequency(periph_t peri)
{
	unused(peri);
	return clk_ll_get_pclk_frequency();
}

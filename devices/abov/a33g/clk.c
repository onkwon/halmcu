#include "abov/hal/clk.h"
#include "abov/asm/arm/cmsis.h"
#include "a33g.h"

#include <assert.h>

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

static inline void set_register_with_mask(volatile uint32_t *reg,
		uint32_t pos, uint32_t mask, uint32_t val)
{
	uint32_t t = *reg;
	t &= ~mask;
	t |= (val << pos) & mask;
	*reg = t;
}

static void unlock_pllcon(void)
{
	PMU->PLLCON = PLLCON_UNLOCK_KEY;
}

static uint32_t get_activation_bitmask_from_peripheral(peripheral_t peri)
{
	switch (peri) {
	case PERIPHERAL_ADC:
		return 1U << 28;
	case PERIPHERAL_PWM7:
	case PERIPHERAL_PWM6:
	case PERIPHERAL_PWM5:
	case PERIPHERAL_PWM4:
		return 1U << 25;
	case PERIPHERAL_PWM3:
	case PERIPHERAL_PWM2:
	case PERIPHERAL_PWM1:
	case PERIPHERAL_PWM0:
		return 1U << 24;
	case PERIPHERAL_UART3:
		return 1U << 23;
	case PERIPHERAL_UART2:
		return 1U << 22;
	case PERIPHERAL_UART1:
		return 1U << 21;
	case PERIPHERAL_UART0:
		return 1U << 20;
	case PERIPHERAL_I2C1:
		return 1U << 19;
	case PERIPHERAL_I2C0 + 0:
		return 1U << 18;
	case PERIPHERAL_SPI1:
		return 1U << 17;
	case PERIPHERAL_SPI0 + 0:
		return 1U << 16;
	case PERIPHERAL_CRC:
		return 1U << 14;
	case PERIPHERAL_GPIO:
		return 1U << 8;
	case PERIPHERAL_TIMER9:
	case PERIPHERAL_TIMER8:
	case PERIPHERAL_TIMER7:
	case PERIPHERAL_TIMER6:
		return 1U << 7;
	case PERIPHERAL_TIMER5:
	case PERIPHERAL_TIMER4:
	case PERIPHERAL_TIMER3:
	case PERIPHERAL_TIMER2:
		return 1U << 6;
	case PERIPHERAL_TIMER1:
	case PERIPHERAL_TIMER0:
		return 1U << 5;
	case PERIPHERAL_FRT:
		return 1U << 4;
	case PERIPHERAL_WDT:
		return 1U << 3;
	default:
		return 0;
	}
}

static uint32_t get_bitmask_from_clksrc(clk_source_t clk)
{
	switch (clk) {
	case CLK_INTERNAL_OSC:
		return 1U << 6;
	case CLK_INTERNAL_OSC_16MHZ:
		return 1U << 4;
	case CLK_EXTERNAL_OSC_SUB:
		return 1U << 2;
	case CLK_EXTERNAL_OSC:
		return 1U << 0;
	default:
		return 0;
	}
}

static bool clk_set_pll_source(clk_source_t clk)
{
	uint32_t val = PMU->BCCR & ~PLLCLKSEL_MASK;

	switch (clk) {
	case CLK_EXTERNAL_OSC:
		val |= PLLCLKSEL_MASK;
		break;
	case CLK_INTERNAL_OSC_16MHZ:
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
		return CLK_EXTERNAL_OSC;
	}
	return CLK_INTERNAL_OSC_16MHZ;
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
	set_register_with_mask(&PMU->BCCR, PLLCLKDIV_POS, PLLCLKDIV_MASK, div_factor);
}

static void clk_set_hclk_prescaler(uint32_t div_factor)
{
	set_register_with_mask(&PMU->BCCR, HCLKDIV_POS, HCLKDIV_MASK, div_factor);
}

static void clk_set_pclk_prescaler(uint32_t div_factor)
{
	set_register_with_mask(&PMU->BCCR, PCLKDIV_POS, PCLKDIV_MASK, div_factor);
}

static void clk_set_pll_post_multifactor(uint32_t mul_factor) // N1
{
	set_register_with_mask(&PMU->PLLCON, VCO_N1_POS, VCO_N1_MASK, mul_factor);
}

static void clk_set_pll_post_divfactor(uint32_t div_factor) // N2
{
	set_register_with_mask(&PMU->PLLCON, VCO_N2_POS, VCO_N2_MASK, div_factor);
}

static void clk_set_pll_pre_divfactor(uint32_t div_factor) // R
{
	set_register_with_mask(&PMU->PLLCON, PREDIV_POS, PREDIV_MASK, div_factor);
}

static void clk_set_pll_divfactor(uint32_t div_factor) // P
{
	set_register_with_mask(&PMU->PLLCON, POSTDIV_POS, POSTDIV_MASK, div_factor);
}

static void clk_set_pll_vco_mode(uint32_t mode) // D
{
	set_register_with_mask(&PMU->PLLCON, VCO_MODE_POS, VCO_MODE_MASK, mode);
}

static bool configure_pll(clk_source_t clkin)
{
	if (!clk_set_pll_source(clkin)) {
		return false;
	}

	clk_enable_source(clkin);

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

	uint32_t n1, n2;
	bool ok = calculate_n1_n2(target_hz, &n1, &n2);
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
		return CLK_EXTERNAL_OSC_SUB;
	case 2:
		return CLK_PLL;
	case 3:
		return CLK_PLL_BYPASS;
	default:
		return CLK_INTERNAL_OSC;
	}
}

static uint32_t clk_get_pll_frequency(void)
{
	uint32_t clkin, n1, n2, d, r, p;

	switch (clk_get_source_internal()) {
	case CLK_INTERNAL_OSC:
		return 1*MHZ;
	case CLK_PLL:
		if (clk_get_pll_source() == CLK_EXTERNAL_OSC) {
			clkin = XTAL;
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
	case CLK_EXTERNAL_OSC_SUB:
	default:
		return 0;
	}
}

void clk_enable_peripheral(peripheral_t peri)
{
	PMU->PCCR |= get_activation_bitmask_from_peripheral(peri);
}

void clk_disable_peripheral(peripheral_t peri)
{
	PMU->PCCR &= ~get_activation_bitmask_from_peripheral(peri);
}

void clk_enable_source(clk_source_t clk)
{
	uint32_t val = PMU->CCR;
	uint32_t mask = get_bitmask_from_clksrc(clk);
	val &= ~(mask * 3);
	val |= mask << 1;
	PMU->CCR = val;

	if (clk == CLK_EXTERNAL_OSC) {
		PMU->CMR |= MXOSCMNT; // activate MXOSC monitoring
		PMU->CMR |= MXOSCSTS; // clear
		while ((PMU->CMR & MXOSCSTS) == 0) {
			/* wait until getting xtal stable */
		}
	}
}

void clk_disable_source(clk_source_t clk)
{
	uint32_t val = PMU->CCR;
	uint32_t mask = get_bitmask_from_clksrc(clk);
	val &= ~(mask * 3);
	PMU->CCR = val;
}

void clk_set_source(clk_source_t clk)
{
	uint32_t mask = 3U;
	uint32_t val = PMU->BCCR;
	val &= ~mask;

	switch (clk) {
	case CLK_INTERNAL_OSC:
		break;
	case CLK_EXTERNAL_OSC_SUB:
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

clk_source_t clk_get_source(void)
{
	return clk_get_source_internal();
}

void clk_enable_pll(void)
{
	PMU->PLLCON |= PLLEN | PLLRST | nBYPASS;
}

void clk_disable_pll(void)
{
	PMU->PLLCON &= ~(PLLEN | PLLRST | nBYPASS);
}

bool clk_set_pll_frequency(clk_source_t clk, clk_source_t clkin, uint32_t hz)
{
	uint32_t src_hz = 16*MHZ; // CLK_INTERNAL_OSC_16MHZ

	assert(hz % MHZ == 0);
	unlock_pllcon();

	switch (clk) {
	case CLK_PLL:
		if (!configure_pll(clkin)) {
			return false;
		}
		if (clkin == CLK_EXTERNAL_OSC) {
			// NOTE: according to UM_A33G52x: 4 <= xtal <= 8MHz
			assert(XTAL >= 4*MHZ && XTAL <= 8*MHZ);
			src_hz = XTAL;
		}
		break;
	case CLK_INTERNAL_OSC:
	case CLK_EXTERNAL_OSC_SUB:
	case CLK_PLL_BYPASS:
	default:
		return false;
	}

	set_pll_factors(src_hz, hz);

	return true;
}

uint32_t clk_get_hclk_frequency(void)
{
	uint32_t div = clk_get_hclk_prescaler();
	if (div & 0x2) {
		div = (div & 1)? 4 : 2;
	} else {
		div = 1;
	}
	return clk_get_pll_frequency() / div;
}

uint32_t clk_get_pclk_frequency(void)
{
	return clk_get_pll_frequency() / (clk_get_pclk_prescaler() + 1);
}

bool clk_is_pll_locked(void)
{
	return !!(PMU->PLLCON & PLLLOCKSTS);
}
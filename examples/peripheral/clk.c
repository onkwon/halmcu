#include "halmcu/hal/clk.h"
#include "halmcu/compiler.h"

int main(void)
{
	clk_init(CLK_HSI, 16000000);

	uint32_t hclk = clk_get_hclk_frequency();
	uint32_t pclk = clk_get_pclk_frequency();
	unused(hclk);
	unused(pclk);

	while (1) {
		/* hang */
	}

	return 0;
}

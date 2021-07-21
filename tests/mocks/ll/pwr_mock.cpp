#include "CppUTestExt/MockSupport.h"

#include "halmcu/periph/pwr.h"

void pwr_enable_peripheral(periph_t peri)
{
	mock().actualCall(__func__).withParameter("peri", peri);
}

void pwr_disable_peripheral(periph_t peri)
{
	mock().actualCall(__func__).withParameter("peri", peri);
}

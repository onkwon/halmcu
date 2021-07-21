#include "CppUTestExt/MockSupport.h"

#include "halmcu/ll/pwr.h"

void pwr_ll_enable_peripheral(periph_t peri)
{
	mock().actualCall(__func__).withParameter("peri", peri);
}

void pwr_ll_disable_peripheral(periph_t peri)
{
	mock().actualCall(__func__).withParameter("peri", peri);
}

#include "CppUTestExt/MockSupport.h"

#include "abov/ll/pwr.h"

void pwr_enable_peripheral(peripheral_t peri)
{
	mock().actualCall(__func__).withParameter("peri", peri);
}

void pwr_disable_peripheral(peripheral_t peri)
{
	mock().actualCall(__func__).withParameter("peri", peri);
}

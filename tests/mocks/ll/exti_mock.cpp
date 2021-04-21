#include "CppUTestExt/MockSupport.h"

#include "../devices/st/stm32f1/exti.h"

void exti_set_source(peripheral_t port, uint32_t pin) {
	mock().actualCall(__func__)
		.withParameter("port", port).withParameter("pin", pin);
}
void exti_clear_rising_trigger(exti_t exti) {
	mock().actualCall(__func__).withParameter("exti", exti);
}
void exti_clear_falling_trigger(exti_t exti) {
	mock().actualCall(__func__).withParameter("exti", exti);
}
void exti_set_rising_trigger(exti_t exti) {
	mock().actualCall(__func__).withParameter("exti", exti);
}
void exti_set_falling_trigger(exti_t exti) {
	mock().actualCall(__func__).withParameter("exti", exti);
}
void exti_enable_irq(exti_t exti) {
	mock().actualCall(__func__).withParameter("exti", exti);
}
void exti_disable_irq(exti_t exti) {
	mock().actualCall(__func__).withParameter("exti", exti);
}
void exti_clear_event(exti_t exti) {
	mock().actualCall(__func__).withParameter("exti", exti);
}

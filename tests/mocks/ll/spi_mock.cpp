#include "CppUTestExt/MockSupport.h"

#include "halmcu/ll/spi.h"

void spi_ll_reset(periph_t spi) {
	mock().actualCall(__func__).withParameter("spi", spi);
}

void spi_ll_start(periph_t spi) {
	mock().actualCall(__func__).withParameter("spi", spi);
}
void spi_ll_stop(periph_t spi) {
	mock().actualCall(__func__).withParameter("spi", spi);
}
void spi_ll_set_mode(periph_t spi, spi_mode_t mode) {
	mock().actualCall(__func__)
		.withParameter("spi", spi)
		.withParameter("mode", mode);
}
void spi_ll_enable_irq(periph_t spi, spi_irq_t irqs) {
	mock().actualCall(__func__)
		.withParameter("spi", spi)
		.withParameter("irqs", irqs);
}
void spi_ll_disable_irq(periph_t spi, spi_irq_t irqs) {
	mock().actualCall(__func__)
		.withParameter("spi", spi)
		.withParameter("irqs", irqs);
}
void spi_ll_set_frequency(periph_t spi, uint32_t hz, uint32_t pclk) {
	mock().actualCall(__func__)
		.withParameter("spi", spi)
		.withParameter("hz", hz)
		.withParameter("pclk", pclk);
}
void spi_ll_set_bitorder(periph_t spi, bool lsb_first) {
	mock().actualCall(__func__)
		.withParameter("spi", spi)
		.withParameter("lsb_first", lsb_first);
}
void spi_ll_set_data_width(periph_t spi, uint32_t data_width) {
	mock().actualCall(__func__)
		.withParameter("spi", spi)
		.withParameter("data_width", data_width);
}
void spi_ll_set_clock_polarity(periph_t spi, int cpol) {
	mock().actualCall(__func__)
		.withParameter("spi", spi)
		.withParameter("cpol", cpol);
}
void spi_ll_set_clock_phase(periph_t spi, int cpha) {
	mock().actualCall(__func__)
		.withParameter("spi", spi)
		.withParameter("cpha", cpha);
}
bool spi_ll_is_busy(periph_t spi) {
	return mock().actualCall(__func__)
		.withParameter("spi", spi)
		.returnBoolValueOrDefault(false);
}
void spi_ll_write(periph_t spi, uint32_t value) {
	mock().actualCall(__func__)
		.withParameter("spi", spi)
		.withParameter("value", value);
}
uint32_t spi_ll_read(periph_t spi) {
	return mock().actualCall(__func__)
		.withParameter("spi", spi)
		.returnUnsignedIntValueOrDefault(0);
}

void spi_ll_clear_event(periph_t spi, spi_event_t events) {
	mock().actualCall(__func__)
		.withParameter("spi", spi)
		.withParameter("events", events);
}
spi_event_t spi_ll_get_event(periph_t spi) {
	return (spi_event_t)mock().actualCall(__func__)
		.withParameter("spi", spi)
		.returnUnsignedIntValueOrDefault(0);
}

void spi_ll_enable_chip_select(periph_t spi) {
	mock().actualCall(__func__).withParameter("spi", spi);
}
void spi_ll_disable_chip_select(periph_t spi) {
	mock().actualCall(__func__).withParameter("spi", spi);
}
void spi_ll_set_chip_select_mode(periph_t spi, bool manual) {
	mock().actualCall(__func__)
		.withParameter("spi", spi)
		.withParameter("manual", manual);
}

#include "console.h"
#include "printf.h"

#include "halmcu/irq.h"
#include "halmcu/asm/arm/systick.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

static void sample_task(void *pvParameters)
{
	(void)pvParameters;
	int i = 0;
	while (1) {
		printf("TASK #1 %d\n", i++);
		vTaskDelay(1000);
	}
}

static void sample_task2(void *pvParameters)
{
	(void)pvParameters;
	int i = 0;
	while (1) {
		printf("TASK #2 %d\n", i++);
		vTaskDelay(1000);
	}
}

void ISR_systick(void)
{
#if (INCLUDE_xTaskGetSchedulerState == 1)
	if (xTaskGetSchedulerState() == taskSCHEDULER_NOT_STARTED) {
		return;
	}
#endif
	extern void xPortSysTickHandler(void);
	xPortSysTickHandler();
}

void ISR_pendsv(void)
{
	extern void xPortPendSVHandler(void);
	xPortPendSVHandler();
}

void ISR_svc(void)
{
	extern void vPortSVCHandler(void);
	vPortSVCHandler();
}

int main(void)
{
	console_init();

	printf("Start FreeRTOS\n");

	systick_set_frequency(1000);
	systick_clear();
	systick_start();

	xTaskCreate(sample_task, "sample task", configMINIMAL_STACK_SIZE, NULL, 0, NULL);
	xTaskCreate(sample_task2, "sample task2", configMINIMAL_STACK_SIZE, NULL, 0, NULL);
	vTaskStartScheduler();

	while (1) {
	}

	return 0;
}

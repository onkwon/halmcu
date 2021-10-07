#include "FreeRTOS.h"
#include "task.h"
#include "printf.h"

#include "halmcu/irq.h"

void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName)
{
	/* This function will get called if a task overflows its stack.   If the
	parameters are corrupt then inspect pxCurrentTCB to find which was the
	offending task. */
	(void)pxTask;
	(void)pcTaskName;

	printf("Stack overflow!\n");

	while (1) {
	}
}

#if (configUSE_IDLE_HOOK == 1)
void vApplicationIdleHook(void)
{
	__asm__ __volatile__ ("wfi");
}
#endif

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

#include "FreeRTOS.h"
#include "task.h"
#include "printf.h"

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

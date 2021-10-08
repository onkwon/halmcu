#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#if defined(NDEBUG)
#define configOPTIMIZE_FOR_LATENCY		0
#endif

#define configUSE_PREEMPTION			1
#define configCPU_CLOCK_HZ			((unsigned long )8000000)
#define configTICK_RATE_HZ			((TickType_t )1000)
#define configTOTAL_HEAP_SIZE			((size_t)(8 * 1024))
#define configMINIMAL_STACK_SIZE		512
#define configMAX_TASK_NAME_LEN			16

#define configPRIO_BITS				3
#define configMAX_PRIORITIES			((1 << configPRIO_BITS) - 1)
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY	configMAX_PRIORITIES
#define configKERNEL_INTERRUPT_PRIORITY		\
	(configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS))
/* The highest interrupt priority that can be used by any interrupt service
routine that makes calls to interrupt safe FreeRTOS API functions. DO NOT CALL
INTERRUPT SAFE FREERTOS API FUNCTIONS FROM ANY INTERRUPT THAT HAS A HIGHER
PRIORITY THAN THIS! (higher priorities are lower numeric values. */
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY 2
/* !!!! configMAX_SYSCALL_INTERRUPT_PRIORITY must not be set to zero !!!!
See http://www.FreeRTOS.org/RTOS-Cortex-M3-M4.html. */
#define configMAX_SYSCALL_INTERRUPT_PRIORITY	\
	(configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS))

#define configUSE_TIME_SLICING			1
#define configUSE_16_BIT_TICKS			0
#define configUSE_IDLE_HOOK			0
#define configUSE_TICK_HOOK			0
#define configUSE_MUTEXES			1
#define configUSE_COUNTING_SEMAPHORES		1
#define configUSE_ALTERNATIVE_API		0
#define configUSE_RECURSIVE_MUTEXES		1
#define configQUEUE_REGISTRY_SIZE		0
#define configIDLE_SHOULD_YIELD			1

#define configUSE_TIMERS			0
#define configTIMER_TASK_PRIORITY		(configMAX_PRIORITIES - 1)
#define configTIMER_TASK_STACK_DEPTH		(configMINIMAL_STACK_SIZE * 2)
#define configTIMER_QUEUE_LENGTH		8

#define configUSE_CO_ROUTINES 			0
#define configMAX_CO_ROUTINE_PRIORITIES		2

#define configCHECK_FOR_STACK_OVERFLOW		2
#define configUSE_TRACE_FACILITY		1
#define configUSE_STATS_FORMATTING_FUNCTIONS	1
#define configGENERATE_RUN_TIME_STATS		0

#define INCLUDE_vTaskPrioritySet		1
#define INCLUDE_uxTaskPriorityGet		1
#define INCLUDE_vTaskDelete			1
#define INCLUDE_vTaskCleanUpResources		0
#define INCLUDE_vTaskSuspend			1
#define INCLUDE_vTaskDelayUntil			1
#define INCLUDE_vTaskDelay			1
#define INCLUDE_xTaskGetSchedulerState		1

#endif /* FREERTOS_CONFIG_H */

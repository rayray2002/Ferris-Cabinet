#ifndef ALARM_TASK_H
#define ALARM_TASK_H

#include "FreeRTOS.h"
#include "cybsp.h"
#include "cyhal.h"
#include "queue.h"
#include "task.h"
#include "timers.h"

/****************************************************************************
 * Enum
 ****************************************************************************/

typedef struct {
    uint8_t music;
} alarm_command_t;

/*******************************************************************************
 * Function Prototypes
 ********************************************************************************/
void alarm_task(void *arg);

/*******************************************************************************
 * Global Variables
 ********************************************************************************/

extern QueueHandle_t alarm_command_q;
extern TimerHandle_t alarm_timer;

#endif /* ALARM_TASK_H */
#include "alarm_task.h"

#include "FreeRTOS.h"
#ifndef DISABLE_RETARGET_IO
#include "cy_retarget_io.h"
#endif
#include "queue.h"
#include "task.h"
#include "timers.h"
#include "ui.h"

QueueHandle_t alarm_command_q;
TimerHandle_t alarm_timer;

void alarm_timer_callback(TimerHandle_t xTimer) {
    cyhal_gpio_write(P0_2, 1);
    vTaskDelay(100);
    cyhal_gpio_write(P0_2, 0);
    vTaskDelay(100);
}

void alarm_task(void *arg) {
    printf("Alarm task started\n");
    cy_rslt_t result;
    alarm_command_t alarm_command;

    result = cyhal_gpio_init(P0_2, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, 0);
    CY_ASSERT(result == CY_RSLT_SUCCESS);

    alarm_timer =
        xTimerCreate("Alarm Timer", pdMS_TO_TICKS(500), pdTRUE, NULL, alarm_timer_callback);

    for (;;) {
        if (pdTRUE == xQueueReceive(alarm_command_q, &alarm_command, portMAX_DELAY)) {
            {
                printf("Alarm task received command\n");
                printf("music: %d\n", alarm_command.music);

                if (alarm_command.music == 1) {
                    printf("Alarm task playing music\n");
                    xTimerStart(alarm_timer, 0);
                    draw_alarm_floating_screen();
                } else {
                    printf("Alarm task stopping music\n");
                    xTimerStop(alarm_timer, 0);
                }
            }
        }
    }
}

#include "interface_task.h"

#include "FreeRTOS.h"
#include "GUI.h"
#include "capsense_task.h"
#include "ui.h"
#ifndef DISABLE_RETARGET_IO
#include "cy_retarget_io.h"
#endif
#include "alarm_task.h"
#include "cycfg.h"
#include "publisher_task.h"
#include "queue.h"
#include "task.h"

/* The pins above are defined by the CY8CKIT-028-TFT library. If the display is being used on
 * different hardware the mappings will be different. */
const mtb_st7789v_pins_t tft_pins = {.db08 = CY8CKIT_028_TFT_PIN_DISPLAY_DB8,
                                     .db09 = CY8CKIT_028_TFT_PIN_DISPLAY_DB9,
                                     .db10 = CY8CKIT_028_TFT_PIN_DISPLAY_DB10,
                                     .db11 = CY8CKIT_028_TFT_PIN_DISPLAY_DB11,
                                     .db12 = CY8CKIT_028_TFT_PIN_DISPLAY_DB12,
                                     .db13 = CY8CKIT_028_TFT_PIN_DISPLAY_DB13,
                                     .db14 = CY8CKIT_028_TFT_PIN_DISPLAY_DB14,
                                     .db15 = CY8CKIT_028_TFT_PIN_DISPLAY_DB15,
                                     .nrd = CY8CKIT_028_TFT_PIN_DISPLAY_NRD,
                                     .nwr = CY8CKIT_028_TFT_PIN_DISPLAY_NWR,
                                     .dc = CY8CKIT_028_TFT_PIN_DISPLAY_DC,
                                     .rst = CY8CKIT_028_TFT_PIN_DISPLAY_RST};

// uint8_t ready = 0;
screen_state_t screen_state = SCREEN_0_MAIN;
alarm_setting_state_t alarm_setting_state = IDLE;
desk_state_t desk_state = DESK_0;

alarm_t alarm = {0, 0, 1};
uint8_t num_of_desks = 7;
desk_config_t desks_config[8] = {
    {"CHARGER", "all of the chargers", 0},
    {"UMBRELLA", "Your best mate on rainy days", 1},
    {"HOODIE", "Comfortable outfit for undergraduates", 2},
    {"PLANT", "Green building", 3},
    {"T-SHIRT", "Best for summer time", 4},
    {"OUTDOOR", "Giroro likes to go out", 5},
    {"CLOAT", "For Cold Weather", 6},
    {"desk8", "description8", 7},
};

main_screen_info_t main_screen_info = {.day_of_week = 1,
                                       .month = 1,
                                       .date = 1,
                                       .hour = 0,
                                       .minute = 0,
                                       .max_temp = 0,
                                       .current_temp = 0,
                                       .min_temp = 0,
                                       .rain_probability = 0,
                                       .moisture = 0};

void task_interface(void* param) {
    BaseType_t rtos_api_result;
    capsense_event_data_t capsense_event_data;

    /* Suppress warning for unused parameter */
    (void)param;

    /* Initialize the display controller */
    cy_rslt_t result;
    result = mtb_st7789v_init8(&tft_pins);
    CY_ASSERT(result == CY_RSLT_SUCCESS);

    GUI_Init();
    draw_infineon_screen();

    /* Repeatedly running part of the task */
    for (;;) {
        /* Block until a command has been received over queue */
        rtos_api_result = xQueueReceive(capsense_event_data_q, &capsense_event_data, portMAX_DELAY);

        /* Command has been received from queue */
        if (rtos_api_result == pdTRUE) {
            if (xTimerIsTimerActive(alarm_timer) == pdTRUE) {
                printf("[Interface Task] Alarm timer is active, stopping it\r\n");
                // alarm_command_t alarm_command;
                // alarm_command.music = 0;
                // xQueueSend(alarm_command_q, &alarm_command, 0);
                xTimerStop(alarm_timer, 100);
            } else {
                switch (capsense_event_data.event) {
                    /* Button 0 pressed */
                    case CAPSENSE_BUTTON0_PRESSED: {
                        /* Handle button 0 pressed here */
                        printf("[Interface Task] Button 0 pressed\r\n");

                        switch (screen_state) {
                            case SCREEN_2_ALARM: {
                                printf("[Interface Task] Alarm setting state: %d\r\n",
                                       alarm_setting_state);
                                switch (alarm_setting_state) {
                                    case IDLE:
                                        alarm_setting_state = HOUR;
                                        break;
                                    case HOUR:
                                        alarm_setting_state = MINUTE;
                                        break;
                                    case MINUTE:
                                        alarm_setting_state = DESK;
                                        break;
                                    default:
                                        alarm_setting_state = IDLE;
                                        /* Set alarm */
                                        publisher_data_t publisher_q_data;
                                        publisher_q_data.topic = "/client/alarm";
                                        publisher_q_data.cmd = PUBLISH_MQTT_MSG;

                                        char buf[100];
                                        sprintf(buf, "{\"hour\":%d,\"minute\":%d,\"desk\":%d}",
                                                alarm.hour, alarm.minute, alarm.desk);
                                        publisher_q_data.data = buf;
                                        xQueueSendToBack(publisher_task_q, &publisher_q_data, 0);
                                        break;
                                }
                                break;
                            }
                            case SCREEN_3_DESK: {
                                screen_state = SCREEN_3_DESK_FOCUS;
                                break;
                            }
                            default: {
                                /* Send action */
                                publisher_data_t publisher_q_data;
                                publisher_q_data.topic = "/client/action";
                                publisher_q_data.cmd = PUBLISH_MQTT_MSG;

                                char buf[2];
                                switch (screen_state) {
                                    case SCREEN_0_MAIN:
                                        buf[0] = '0';
                                        break;
                                    case SCREEN_1_WEATHER:
                                        buf[0] = '9';
                                        break;
                                    default:
                                        buf[0] = desk_state + '1';
                                        break;
                                }
                                buf[1] = '\0';
                                publisher_q_data.data = buf;
                                xQueueSendToBack(publisher_task_q, &publisher_q_data, 0);
                                break;
                            }
                        }
                        break;
                    }
                    /* Button 1 pressed */
                    case CAPSENSE_BUTTON1_PRESSED: {
                        /* Handle button 1 pressed here */
                        printf("[Interface Task] Button 1 pressed\r\n");

                        if (alarm_setting_state != IDLE) {
                            alarm_setting_state = IDLE;
                        } else if (screen_state == SCREEN_3_DESK_FOCUS) {
                            screen_state = SCREEN_3_DESK;
                        } else {
                            screen_state = SCREEN_0_MAIN;
                        }
                        break;
                    }
                    // /* Slider touched */
                    case CAPSENSE_SLIDER_RIGHT: {
                        /* Handle slider touched here */
                        printf("[Interface Task] Slider right\r\n");

                        switch (screen_state) {
                            case SCREEN_3_DESK_FOCUS: {
                                desk_state = (desk_state + 1) % num_of_desks;
                                break;
                            }
                            default: {
                                if (alarm_setting_state == HOUR) {
                                    alarm.hour = (alarm.hour + 1) % 24;
                                } else if (alarm_setting_state == MINUTE) {
                                    alarm.minute = (alarm.minute + 1) % 60;
                                } else if (alarm_setting_state == DESK) {
                                    alarm.desk = (alarm.desk + 1) % num_of_desks;
                                } else {
                                    screen_state =
                                        (screen_state_t)((screen_state + 1) % TOTAL_PAGES);
                                }
                            }
                        }
                        break;
                    }
                    case CAPSENSE_SLIDER_LEFT: {
                        /* Handle slider touched here */
                        printf("[Interface Task] Slider left\r\n");

                        switch (screen_state) {
                            case SCREEN_3_DESK_FOCUS: {
                                desk_state = (desk_state + num_of_desks - 1) % num_of_desks;
                                break;
                            }
                            default: {
                                if (alarm_setting_state == HOUR) {
                                    alarm.hour = (alarm.hour + 23) % 24;
                                } else if (alarm_setting_state == MINUTE) {
                                    alarm.minute = (alarm.minute + 59) % 60;
                                } else if (alarm_setting_state == DESK) {
                                    alarm.desk = (alarm.desk + num_of_desks - 1) % num_of_desks;
                                } else {
                                    screen_state =
                                        (screen_state_t)((screen_state + TOTAL_PAGES - 1) %
                                                         TOTAL_PAGES);
                                }
                            }
                        }
                        break;
                    }
                    /* Invalid command */
                    default: {
                        /* Handle invalid command here */
                        // if (ready < 2) {
                        //     draw_infineon_screen();
                        // } else {
                        //     draw_logo_screen();
                        // }
                        break;
                    }
                }
            }
            printf("[Interface Task] Screen state: %d, num_of_desks: %d\r\n", screen_state,
                   num_of_desks);
            switch (screen_state) {
                case SCREEN_0_MAIN:
                    draw_homepage_screen();
                    break;

                case SCREEN_1_WEATHER:
                    draw_weather_screen();
                    break;

                case SCREEN_2_ALARM:
                    draw_alarm_screen();
                    break;

                default:
                    draw_desk_screen(desk_state + 1, &desks_config[desk_state]);
                    break;
            }
        }

        /* Task has timed out and received no data during an interval of
         * portMAXDELAY ticks.
         */
        else {
            /* Handle timeout here */
        }
    }
}
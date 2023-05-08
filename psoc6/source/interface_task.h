/******************************************************************************
 * File Name: led_task.h
 *
 * Description: This file is the public interface of led_task.c source file
 *
 * Related Document: README.md
 *
 *******************************************************************************
 * Copyright 2019-2022, Cypress Semiconductor Corporation (an Infineon company) or
 * an affiliate of Cypress Semiconductor Corporation.  All rights reserved.
 *
 * This software, including source code, documentation and related
 * materials ("Software") is owned by Cypress Semiconductor Corporation
 * or one of its affiliates ("Cypress") and is protected by and subject to
 * worldwide patent protection (United States and foreign),
 * United States copyright laws and international treaty provisions.
 * Therefore, you may use this Software only as provided in the license
 * agreement accompanying the software package from which you
 * obtained this Software ("EULA").
 * If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
 * non-transferable license to copy, modify, and compile the Software
 * source code solely for use in connection with Cypress's
 * integrated circuit products.  Any reproduction, modification, translation,
 * compilation, or representation of this Software except as specified
 * above is prohibited without the express written permission of Cypress.
 *
 * Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
 * reserves the right to make changes to the Software without notice. Cypress
 * does not assume any liability arising out of the application or use of the
 * Software or any product or circuit described in the Software. Cypress does
 * not authorize its products for use in any products where a malfunction or
 * failure of the Cypress product may reasonably be expected to result in
 * significant property damage, injury or death ("High Risk Product"). By
 * including Cypress's product in a High Risk Product, the manufacturer
 * of such system or application assumes all risk of such use and in doing
 * so agrees to indemnify Cypress against all liability.
 *******************************************************************************/

/*******************************************************************************
 * Include guard
 ******************************************************************************/
#ifndef SOURCE_INTERFACE_TASK_H_
#define SOURCE_INTERFACE_TASK_H_

/*******************************************************************************
 * Header file includes
 ******************************************************************************/
#include "FreeRTOS.h"
#include "GUI.h"
#include "cy8ckit_028_tft_pins.h"
#include "mtb_st7789v.h"
#include "queue.h"
#include "task.h"

/*******************************************************************************
 * Global constants
 *******************************************************************************/

#define TOTAL_PAGES (4)

/*******************************************************************************
 * Data structure and enumeration
 ******************************************************************************/
typedef enum {
    SCREEN_0_MAIN,
    SCREEN_1_WEATHER,
    SCREEN_2_ALARM,
    SCREEN_3_DESK,
    SCREEN_3_DESK_FOCUS,
} screen_state_t;

typedef enum {
    DESK_0,
    DESK_1,
    DESK_2,
    DESK_3,
    DESK_4,
    DESK_5,
    DESK_6,
    DESK_7,
} desk_state_t;

typedef enum {
    IDLE,
    HOUR,
    MINUTE,
    DESK,
} alarm_setting_state_t;

typedef struct {
    uint8_t hour;
    uint8_t minute;
    uint8_t desk;
} alarm_t;

typedef struct {
    char name[20];
    char description[100];
    uint8_t icon;
} desk_config_t;

typedef struct {
    uint8_t day_of_week;
    uint8_t month;
    uint8_t date;

    uint8_t hour;
    uint8_t minute;

    float max_temp;
    float current_temp;
    float min_temp;
    uint8_t moisture;
    uint8_t rain_probability;
} main_screen_info_t;

/*******************************************************************************
 * Global variable
 ******************************************************************************/
// extern uint8_t ready;

extern screen_state_t screen_state;
extern uint8_t num_of_desks;
extern desk_config_t desks_config[8];

extern main_screen_info_t main_screen_info;
extern alarm_setting_state_t alarm_setting_state;
extern alarm_t alarm;

/*******************************************************************************
 * Function prototype
 ******************************************************************************/
void task_interface(void* param);
void draw_main_screen();

#endif /* SOURCE_INTERFACE_TASK_H_ */

/* [] END OF FILE  */

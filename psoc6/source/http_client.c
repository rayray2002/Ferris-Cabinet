/******************************************************************************
 * File Name:   http_client.c
 *
 * Description: This file contains task and functions related to HTTP client
 * operation.
 *
 *******************************************************************************
 * (c) 2019-2020, Cypress Semiconductor Corporation. All rights reserved.
 *******************************************************************************
 * This software, including source code, documentation and related materials
 * ("Software"), is owned by Cypress Semiconductor Corporation or one of its
 * subsidiaries ("Cypress") and is protected by and subject to worldwide patent
 * protection (United States and foreign), United States copyright laws and
 * international treaty provisions. Therefore, you may use this Software only
 * as provided in the license agreement accompanying the software package from
 * which you obtained this Software ("EULA").
 *
 * If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
 * non-transferable license to copy, modify, and compile the Software source
 * code solely for use in connection with Cypress's integrated circuit products.
 * Any reproduction, modification, translation, compilation, or representation
 * of this Software except as specified above is prohibited without the express
 * written permission of Cypress.
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
 * including Cypress's product in a High Risk Product, the manufacturer of such
 * system or application assumes all risk of such use and in doing so agrees to
 * indemnify Cypress against all liability.
 *******************************************************************************/

/* Header file includes. */
#ifndef DISABLE_RETARGET_IO
#include "cy_retarget_io.h"
#endif
#include "cybsp.h"
#include "cyhal.h"

/* FreeRTOS header file. */
#include <FreeRTOS.h>
#include <cJSON.h>
#include <semphr.h>
#include <task.h>

#include "timers.h"

/* Standard C header file. */
#include <string.h>

/* Cypress secure socket header file. */
#include "cy_secure_sockets.h"

/* Wi-Fi connection manager header files. */
#include "cy_wcm.h"
#include "cy_wcm_error.h"

/* TCP client task header file. */
#include "http_client.h"
#include "interface_task.h"
#include "ui.h"

/* HTTP Client Library*/
#include "cy_http_client_api.h"

/*******************************************************************************
 * Macros
 ********************************************************************************/
#define BUFFERSIZE (2048 * 1)
#define SENDRECEIVETIMEOUT (5000)

/*******************************************************************************
 * Function Prototypes
 ********************************************************************************/
cy_rslt_t connect_to_wifi_ap(void);
void disconnect_callback(void *arg);

/*******************************************************************************
 * Global Variables
 ********************************************************************************/
bool connected;
TaskHandle_t http_client_task_handle;
TimerHandle_t update_time_timer_handle;

void parse_time_response(const char *response) {
    printf("parse_time_response\r\n");
    cJSON *root = cJSON_Parse(response);
    if (root == NULL) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            fprintf(stderr, "Error before: %s\n", error_ptr);
        }
    }

    char *string = cJSON_Print(root);
    printf("%s\r\n", string);

    cJSON *hour = cJSON_GetObjectItemCaseSensitive(root, "hour");
    cJSON *minute = cJSON_GetObjectItemCaseSensitive(root, "minute");
    cJSON *date = cJSON_GetObjectItemCaseSensitive(root, "date");
    cJSON *month = cJSON_GetObjectItemCaseSensitive(root, "month");
    cJSON *day_of_week = cJSON_GetObjectItemCaseSensitive(root, "day_of_week");

    main_screen_info.hour = atoi(hour->valuestring);
    main_screen_info.minute = atoi(minute->valuestring);
    main_screen_info.date = atoi(date->valuestring);
    main_screen_info.month = atoi(month->valuestring);
    main_screen_info.day_of_week = atoi(day_of_week->valuestring);

    alarm.hour = main_screen_info.hour;
    alarm.minute = main_screen_info.minute;

    printf("hour: %d\r\n", main_screen_info.hour);
    printf("minute: %d\r\n", main_screen_info.minute);
    printf("date: %d\r\n", main_screen_info.date);
    printf("month: %d\r\n", main_screen_info.month);
    printf("day_of_week: %d\r\n", main_screen_info.day_of_week);
}

void parse_weather_response(const char *response) {
    printf("parse_weather_response\r\n");
    cJSON *root = cJSON_Parse(response);
    if (root == NULL) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            fprintf(stderr, "Error before: %s\n", error_ptr);
        }
    }

    char *string = cJSON_Print(root);
    printf("%s\r\n", string);

    cJSON *current_temp = cJSON_GetObjectItemCaseSensitive(root, "current_temp");
    cJSON *max_temp = cJSON_GetObjectItemCaseSensitive(root, "max_temp");
    cJSON *min_temp = cJSON_GetObjectItemCaseSensitive(root, "min_temp");
    cJSON *rain_probability = cJSON_GetObjectItemCaseSensitive(root, "rain_probability");
    cJSON *moisture = cJSON_GetObjectItemCaseSensitive(root, "moisture");

    main_screen_info.current_temp = (float)current_temp->valuedouble;
    main_screen_info.max_temp = (float)max_temp->valuedouble;
    main_screen_info.min_temp = (float)min_temp->valuedouble;
    main_screen_info.rain_probability = (int)rain_probability->valuedouble;
    main_screen_info.moisture = (int)moisture->valuedouble;

    printf("current_temp: %f\r\n", main_screen_info.current_temp);
    printf("max_temp: %f\r\n", main_screen_info.max_temp);
    printf("min_temp: %f\r\n", main_screen_info.min_temp);
    printf("rain_probability: %d\r\n", main_screen_info.rain_probability);
    printf("moisture: %d\r\n", main_screen_info.moisture);
}

static void update_time(TimerHandle_t xTimer) {
    main_screen_info.minute += 1;
    if (main_screen_info.minute >= 60) {
        main_screen_info.minute = 0;
        main_screen_info.hour += 1;
        if (main_screen_info.hour >= 24) {
            main_screen_info.hour = 0;
        }
    }
    printf("[update_time_task] %02d:%02d\r\n", main_screen_info.hour, main_screen_info.minute);
    if (screen_state == SCREEN_0_MAIN) {
        draw_homepage_screen();
    }
}

/*******************************************************************************
 * Function Name: http_client_task
 *******************************************************************************
 * Summary:
 *  Task used to establish a connection to a remote TCP server and
 *  control the LED state (ON/OFF) based on the command received from TCP server.
 *
 * Parameters:
 *  void *args : Task parameter defined during task creation (unused).
 *
 * Return:
 *  void
 *
 *******************************************************************************/
void http_client_task(void *arg) {
    cy_rslt_t result;

    // result = connect_to_wifi_ap();
    // CY_ASSERT(result == CY_RSLT_SUCCESS);

    result = cy_http_client_init();
    if (result != CY_RSLT_SUCCESS) {
        printf("HTTP Client Library Initialization Failed!\n");
        CY_ASSERT(0);
    }

    // Server Info
    cy_awsport_server_info_t serverInfo;
    (void)memset(&serverInfo, 0, sizeof(serverInfo));
    serverInfo.host_name = SERVERHOSTNAME;
    serverInfo.port = SERVERPORT;

    // Disconnection Callback
    cy_http_disconnect_callback_t disconnectCallback = (void *)disconnect_callback;

    // Client Handle
    cy_http_client_t clientHandle;

    // Create the HTTP Client
    result = cy_http_client_create(NULL, &serverInfo, disconnectCallback, NULL, &clientHandle);
    if (result != CY_RSLT_SUCCESS) {
        printf("HTTP Client Creation Failed!\n");
        CY_ASSERT(0);
    }

    // Connect to the HTTP Server
    result = cy_http_client_connect(clientHandle, SENDRECEIVETIMEOUT, SENDRECEIVETIMEOUT);
    if (result != CY_RSLT_SUCCESS) {
        printf("HTTP Client Connection Failed!\n");
        CY_ASSERT(0);
    } else {
        printf("\nConnected to HTTP Server Successfully\n\n");
        connected = true;
    }

    // GET /api/ip HTTP/1.1
    // Host: worldtimeapi.org
    // User-Agent: curl/7.87.0
    // Accept: */*

    // Create Request
    uint8_t buffer[BUFFERSIZE];
    cy_http_client_request_header_t request;
    request.buffer = buffer;
    request.buffer_len = BUFFERSIZE;
    request.method = CY_HTTP_CLIENT_METHOD_GET;
    request.range_start = -1;
    request.range_end = -1;
    request.resource_path = "/time";

    // Create Header
    cy_http_client_header_t header;
    header.field = "Host";
    header.field_len = strlen("Host");
    header.value = SERVERHOSTNAME;
    header.value_len = strlen(SERVERHOSTNAME);
    uint32_t num_header = 1;

    result = cy_http_client_write_header(clientHandle, &request, &header, num_header);
    if (result != CY_RSLT_SUCCESS) {
        printf("HTTP Client Header Write Failed!\n");
        CY_ASSERT(0);
    }

    // Var to hold the servers responses
    cy_http_client_response_t response;

    // Send get request to /html resource
    if (connected) {
        result = cy_http_client_send(clientHandle, &request, NULL, 0, &response);
        if (result != CY_RSLT_SUCCESS) {
            printf("HTTP Client Send Failed!\n");
            CY_ASSERT(0);
        }
    } else {
        // Connect to the HTTP Server
        result = cy_http_client_connect(clientHandle, SENDRECEIVETIMEOUT, SENDRECEIVETIMEOUT);
        if (result != CY_RSLT_SUCCESS) {
            printf("HTTP Client Connection Failed!\n");
            CY_ASSERT(0);
        } else {
            printf("\nConnected to HTTP Server Successfully\n\n");
            connected = true;
        }
        // Send get request to /html resource
        result = cy_http_client_send(clientHandle, &request, NULL, 0, &response);
        if (result != CY_RSLT_SUCCESS) {
            printf("HTTP Client Send Failed!\n");
            CY_ASSERT(0);
        }
    }

    // Print response message
    printf("Response received from httpbin.org/html:\n");

    printf("Status Code: %d\n", response.status_code);
    for (int i = 0; i < response.body_len; i++) {
        printf("%c", response.body[i]);
    }
    printf("END\n");

    // Parse JSON
    parse_time_response((char *)response.body);

    vTaskDelay(1000);

    // Create Request
    // uint8_t buffer[BUFFERSIZE];
    // cy_http_client_request_header_t request;
    request.buffer = buffer;
    request.buffer_len = BUFFERSIZE;
    request.method = CY_HTTP_CLIENT_METHOD_GET;
    request.range_start = -1;
    request.range_end = -1;
    request.resource_path = "/weather";

    // Create Header
    // cy_http_client_header_t header;
    header.field = "Host";
    header.field_len = strlen("Host");
    header.value = SERVERHOSTNAME;
    header.value_len = strlen(SERVERHOSTNAME);
    num_header = 1;

    result = cy_http_client_write_header(clientHandle, &request, &header, num_header);
    if (result != CY_RSLT_SUCCESS) {
        printf("HTTP Client Header Write Failed!\n");
        CY_ASSERT(0);
    }

    // Var to hold the servers responses
    // cy_http_client_response_t response;

    // Send get request to /html resource
    if (connected) {
        result = cy_http_client_send(clientHandle, &request, NULL, 0, &response);
        if (result != CY_RSLT_SUCCESS) {
            printf("HTTP Client Send Failed!\n");
            CY_ASSERT(0);
        }
    } else {
        // Connect to the HTTP Server
        result = cy_http_client_connect(clientHandle, SENDRECEIVETIMEOUT, SENDRECEIVETIMEOUT);
        if (result != CY_RSLT_SUCCESS) {
            printf("HTTP Client Connection Failed!\n");
            CY_ASSERT(0);
        } else {
            printf("\nConnected to HTTP Server Successfully\n\n");
            connected = true;
        }
        // Send get request to /html resource
        result = cy_http_client_send(clientHandle, &request, NULL, 0, &response);
        if (result != CY_RSLT_SUCCESS) {
            printf("HTTP Client Send Failed!\n");
            CY_ASSERT(0);
        }
    }

    // Print response message
    printf("Response received from httpbin.org/html:\n");

    printf("Status Code: %d\n", response.status_code);
    for (int i = 0; i < response.body_len; i++) {
        printf("%c", response.body[i]);
    }
    printf("END\n");

    // Parse JSON
    parse_weather_response((char *)response.body);

    update_time_timer_handle =
        xTimerCreate("update_time_timer", 60 * 1000, pdTRUE, NULL, update_time);
    xTimerStart(update_time_timer_handle, 0);
    draw_homepage_screen();

    while (1) {
        vTaskDelay(1);
    }
}

/*******************************************************************************
 * Function Name: connect_to_wifi_ap()
 *******************************************************************************
 * Summary:
 *  Connects to Wi-Fi AP using the user-configured credentials, retries up to a
 *  configured number of times until the connection succeeds.
 *
 *******************************************************************************/
// cy_rslt_t connect_to_wifi_ap(void)
// {
//     cy_rslt_t result;

//     /* Variables used by Wi-Fi connection manager.*/
//     cy_wcm_connect_params_t wifi_conn_param;

//     cy_wcm_config_t wifi_config = { .interface = CY_WCM_INTERFACE_TYPE_STA };

//     cy_wcm_ip_address_t ip_address;

//      /* Initialize Wi-Fi connection manager. */
//     result = cy_wcm_init(&wifi_config);

//     if (result != CY_RSLT_SUCCESS)
//     {
//         printf("Wi-Fi Connection Manager initialization failed!\n");
//         return result;
//     }
//     printf("Wi-Fi Connection Manager initialized.\r\n");

//      /* Set the Wi-Fi SSID, password and security type. */
//     memset(&wifi_conn_param, 0, sizeof(cy_wcm_connect_params_t));
//     memcpy(wifi_conn_param.ap_credentials.SSID, WIFI_SSID, sizeof(WIFI_SSID));
//     memcpy(wifi_conn_param.ap_credentials.password, WIFI_PASSWORD, sizeof(WIFI_PASSWORD));
//     wifi_conn_param.ap_credentials.security = WIFI_SECURITY_TYPE;

//     /* Join the Wi-Fi AP. */
//     for(uint32_t conn_retries = 0; conn_retries < MAX_WIFI_CONN_RETRIES; conn_retries++ )
//     {
//         result = cy_wcm_connect_ap(&wifi_conn_param, &ip_address);

//         if(result == CY_RSLT_SUCCESS)
//         {
//             printf("Successfully connected to Wi-Fi network '%s'.\n",
//                                 wifi_conn_param.ap_credentials.SSID);
//             printf("IP Address Assigned: %d.%d.%d.%d\n", (uint8)ip_address.ip.v4,
//                     (uint8)(ip_address.ip.v4 >> 8), (uint8)(ip_address.ip.v4 >> 16),
//                     (uint8)(ip_address.ip.v4 >> 24));
//             return result;
//         }

//         printf("Connection to Wi-Fi network failed with error code %d."
//                "Retrying in %d ms...\n", (int)result, WIFI_CONN_RETRY_INTERVAL_MSEC);

//         vTaskDelay(pdMS_TO_TICKS(WIFI_CONN_RETRY_INTERVAL_MSEC));
//     }

//     /* Stop retrying after maximum retry attempts. */
//     printf("Exceeded maximum Wi-Fi connection attempts\n");

//     return result;
// }

/*******************************************************************************
 * Function Name: disconnect_callback
 *******************************************************************************
 * Summary:
 *  Invoked when the server disconnects
 *
 * Parameters:
 *  void *arg : unused
 *
 * Return:
 *  void
 *
 *******************************************************************************/
void disconnect_callback(void *arg) {
    printf("Disconnected from HTTP Server\n");
    connected = false;
}

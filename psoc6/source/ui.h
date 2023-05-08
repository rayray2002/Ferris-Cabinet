#ifndef UI_H
#define UI_H
#include <stdio.h>

#include "GUI.h"
#include "interface_task.h"

extern GUI_CONST_STORAGE GUI_BITMAP bmrainy;
extern GUI_CONST_STORAGE GUI_BITMAP bmcloudy;
extern GUI_CONST_STORAGE GUI_BITMAP bmsunny;
extern GUI_CONST_STORAGE GUI_BITMAP bmwater;
extern GUI_CONST_STORAGE GUI_BITMAP bmwatersmall;
extern GUI_CONST_STORAGE GUI_BITMAP bmcloudsun;
extern GUI_CONST_STORAGE GUI_BITMAP bmleft;
extern GUI_CONST_STORAGE GUI_BITMAP bmright;
extern GUI_CONST_STORAGE GUI_BITMAP bmlogo;
extern GUI_CONST_STORAGE GUI_BITMAP bminfineon;

#define GUI_SKYBLUE GUI_MAKE_COLOR(0x01e6aa02)
#define GUI_MYRED GUI_MAKE_COLOR(0x0101008E)

typedef enum {
    sunny,
    cloudy,
    cloudsun,
    rainy,
} weather_t;

weather_t guess_weather(int rainprob, int moisture);

void draw_heading(char* name);
void draw_logo_screen();
void draw_infineon_screen();
void draw_alarm_floating_screen();
void draw_weather_icon(weather_t w, int x, int y);
void draw_homepage_screen();
void draw_weather_screen();
void draw_alarm_screen();
void draw_desk_screen(const uint8_t desk_id, const desk_config_t* config);

// void CO2_Screen(int concentration);

#endif

#include "ui.h"

void draw_heading(char* name) {
    char time[6];
    sprintf(time, "%02d:%02d", main_screen_info.hour, main_screen_info.minute);

    GUI_SetColor(GUI_WHITE);
    GUI_SetFont(GUI_FONT_13_1);
    GUI_DispStringAt(name, 10, 10);
    GUI_DispStringAt(time, 280, 10);
}

weather_t guess_weather(int rainprob, int moisture) {
    if (moisture > 50) {
        if (rainprob > 85) {
            return rainy;
        } else {
            return cloudy;
        }
    } else {
        if (rainprob > 40) {
            return cloudsun;
        } else {
            return sunny;
        }
    }
}

void draw_weather_icon(weather_t w, int x, int y) {
    switch (w) {
        case sunny:
            GUI_DrawBitmap(&bmsunny, x, y);
            break;
        case cloudy:
            GUI_DrawBitmap(&bmcloudy, x, y);
            break;
        case cloudsun:
            GUI_DrawBitmap(&bmcloudsun, x, y);
            break;
        case rainy:
            GUI_DrawBitmap(&bmrainy, x, y);
            break;
    }
}

void draw_alarm_floating_screen() {
    GUI_SetColor(GUI_GRAY);
    GUI_FillRect(50, 50, 270, 190);
    GUI_SetColor(GUI_BLACK);
    GUI_SetBkColor(GUI_GRAY);
    GUI_SetFont(GUI_FONT_32_1);
    GUI_SetTextAlign(GUI_ALIGN_HCENTER);
    GUI_DispStringAt("Alarm", 160, 60);
}

void draw_logo_screen() {
    GUI_SetBkColor(GUI_BLACK);
    GUI_Clear();

    GUI_DrawBitmap(&bmlogo, 60, 20);

    // GUI_SetTextAlign(GUI_ALIGN_HCENTER);
    // GUI_SetColor(GUI_WHITE);
    // GUI_SetFont(GUI_FONT_16_1);
    // GUI_DispStringAt("Fetching data...", 160, 10);
}

void draw_infineon_screen() {
    GUI_SetBkColor(GUI_WHITE);
    GUI_Clear();

    GUI_DrawBitmap(&bminfineon, 10, 52);

    // GUI_SetTextAlign(GUI_ALIGN_HCENTER);
    // GUI_SetColor(GUI_BLACK);
    // GUI_SetFont(GUI_FONT_16_1);
    // GUI_DispStringAt("Connecting WiFi...", 160, 10);
}

void draw_homepage_screen() {
    main_screen_info_t info = main_screen_info;

    char time[6];
    sprintf(time, "%02d:%02d", info.hour, info.minute);

    int temperature = info.current_temp;

    char date[11];
    sprintf(date, "%02d%02d", info.month, info.date);

    char* day_of_week_to_string[10] = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};
    char* weekday = day_of_week_to_string[info.day_of_week];

    char prob[4];
    int rainProb = info.rain_probability;
    sprintf(prob, "%d%%", info.rain_probability);

    int moisture = info.moisture;

    GUI_SetBkColor(GUI_BLACK);
    GUI_Clear();

    int yline = 20;
    int xstart = 10;
    GUI_SetFont(GUI_FONT_32_1);
    GUI_SetColor(GUI_WHITE);
    GUI_DispStringAt(weekday, xstart, yline);
    GUI_DispStringAt(date, xstart, yline + 35);

    xstart += 70;
    draw_weather_icon(guess_weather(rainProb, moisture), xstart, yline);

    xstart += 80;
    GUI_SetFont(GUI_FONT_D32);
    GUI_DispDecAt(temperature, xstart, yline + 15, 2);
    xstart += 50;
    GUI_SetFont(GUI_FONT_16B_1);
    GUI_DispStringAt("o", xstart + 5, yline + 10);
    GUI_SetFont(GUI_FONT_24_1);
    GUI_DispStringAt("C", xstart + 15, yline + 15);

    xstart += 40;
    GUI_DrawBitmap(&bmwatersmall, xstart + 5, yline);
    GUI_SetColor(GUI_SKYBLUE);
    GUI_SetFont(GUI_FONT_24_1);
    GUI_DispStringAt(prob, xstart, yline + 40);
    GUI_SetColor(GUI_WHITE);
    GUI_SetFont(GUI_FONT_D80);
    GUI_DispStringAt(time, 10, 120);
}

// void Alarm_Screen(char* time, int hour, int minute, char* description, int shelf_index,
//                   char setting) {
void draw_alarm_screen() {
    int hour = alarm.hour;
    int minute = alarm.minute;
    int desk = alarm.desk;

    char* description = "";

    GUI_SetBkColor(GUI_BLACK);
    GUI_Clear();

    // setting: s - shelf, h - hour, m - minute
    draw_heading("Alarm");
    int yline = 70;
    int xstart = 30;
    GUI_SetColor(GUI_GRAY);
    switch (alarm_setting_state) {
        case HOUR:
            GUI_DrawRoundedFrame(25, 58, 130, 145, 5, 4);
            break;
        case MINUTE:
            GUI_DrawRoundedFrame(165, 58, 270, 145, 5, 4);
            break;
        default:
            break;
    }

    GUI_SetColor(GUI_WHITE);
    GUI_SetFont(GUI_FONT_D64);
    GUI_DispDecAt(hour, xstart, yline, 2);
    GUI_SetFont(GUI_FONT_D64);
    GUI_DispStringAt(":", 138, yline - 5);
    GUI_DispDecAt(minute, 170, yline, 2);

    yline += 100;
    GUI_SetFont(GUI_FONT_16_1);
    GUI_DispStringAt(description, xstart + 5, yline + 5);
    GUI_SetColor(GUI_MYRED);
    if (alarm_setting_state == DESK) {
        GUI_DrawRoundedFrame(250, yline, 290, yline + 40, 10, 4);
        GUI_SetBkColor(GUI_BLACK);
    } else {
        GUI_FillRoundedRect(250, yline, 290, yline + 40, 10);
        GUI_SetBkColor(GUI_MYRED);
    }
    GUI_SetColor(GUI_WHITE);
    GUI_SetFont(GUI_FONT_24B_1);
    GUI_DispDecAt(desk, 265, yline + 8, 1);
}

// void draw_desk_screen(char* time, char* name, char* description, int shelf_index) {
void draw_desk_screen(const uint8_t desk_id, const desk_config_t* config) {
    uint8_t shelf_index = desk_id;
    char* name = config->name;
    char* description = config->description;

    GUI_SetBkColor(GUI_BLACK);
    GUI_Clear();

    int yline = 96;
    int xstart = 62;
    draw_heading("Cabinet");

    if (screen_state == SCREEN_3_DESK_FOCUS) {
        /*only appear in choosing state*/
        GUI_DrawBitmap(&bmleft, 10, 120);
        GUI_DrawBitmap(&bmright, 286, 120);
        GUI_SetColor(GUI_GRAY);
        GUI_DrawRoundedFrame(10, 190, 70, 230, 5, 1);
        GUI_DispStringAt("Select", 25, 200);
        GUI_DrawRoundedFrame(250, 190, 310, 230, 5, 1);
        GUI_DispStringAt("Back", 265, 200);
        /*only appear in choosing state*/
    }

    GUI_SetColor(GUI_GRAY);
    GUI_FillRect(xstart - 22, yline - 16, xstart + 58, yline + 64);
    GUI_SetFont(GUI_FONT_D48);
    GUI_SetBkColor(GUI_GRAY);
    GUI_SetColor(GUI_WHITE);
    GUI_DispDecAt(shelf_index, xstart, yline, 1);
    xstart += 70;
    GUI_SetFont(GUI_FONT_32B_1);
    GUI_SetBkColor(GUI_BLACK);
    GUI_DispStringAt(name, xstart, yline - 10);

    yline = yline + 45;
    GUI_SetFont(GUI_FONT_16_1);
    GUI_DispStringAt(description, xstart, yline);
}

// void draw_weather_screen(char* time, char weather, int lowTemp, int highTemp, int rainProb) {
void draw_weather_screen() {
    int lowTemp = main_screen_info.min_temp;
    int highTemp = main_screen_info.max_temp;
    int rainProb = main_screen_info.rain_probability;
    int moisture = main_screen_info.moisture;

    char prob[4];
    sprintf(prob, "%d%%", rainProb);

    GUI_SetBkColor(GUI_BLACK);
    GUI_Clear();

    int yline = 40;
    draw_heading("Weather");
    draw_weather_icon(guess_weather(rainProb, moisture), 20, yline);
    GUI_SetColor(GUI_SKYBLUE);
    GUI_SetFont(GUI_FONT_D32);
    GUI_DrawBitmap(&bmwater, 140, yline);

    GUI_SetTextAlign(GUI_TA_RIGHT);
    if (rainProb < 10) {
        GUI_DispDecAt(rainProb, 260, yline + 20, 1);
    } else {
        GUI_DispDecAt(rainProb, 260, yline + 20, 2);
    }
    GUI_SetFont(GUI_FONT_32B_1);
    GUI_SetTextAlign(GUI_TA_LEFT);
    GUI_DispStringAt("%", 270, yline + 20);

    yline = 140;
    GUI_SetColor(GUI_WHITE);
    GUI_SetFont(GUI_FONT_D64);
    GUI_DispDecAt(lowTemp, 20, yline, 2);
    GUI_SetFont(GUI_FONT_32B_1);
    GUI_DispStringAt("_", 130, yline + 5);
    GUI_SetFont(GUI_FONT_D64);
    GUI_DispDecAt(highTemp, 160, yline, 2);
    GUI_SetFont(GUI_FONT_16B_1);
    GUI_DispStringAt("o", 260, yline);
    GUI_SetFont(GUI_FONT_32_1);
    GUI_DispStringAt("C", 270, yline);

    // GUI_SetFont(GUI_FONT_32B_1);
    // GUI_DispStringAt("℃", 235, yline);
}

// void CO2_Screen(int concentration){
//     GUI_SetFont(GUI_FONT_D80);
//     if(x)
//     GUI_DispDecAt(concentration, 20, 80, x);
//     GUI_SetFont(GUI_FONT_32B_1);
//     GUI_DispStringAt("ppm", 250, 130);
// }

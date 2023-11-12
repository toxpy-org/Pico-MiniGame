/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include "LCD_2in.h"
#include "GUI_Paint.h"
#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "./loop.c"
#include "./display.c"

#include <malloc.h>

uint32_t getTotalHeap(void)
{
    extern char __StackLimit, __bss_end__;
    return &__StackLimit - &__bss_end__;
}

uint32_t getFreeHeap(void)
{
    struct mallinfo m = mallinfo();
    return getTotalHeap() - m.uordblks;
}

int main()
{
    stdio_init_all();
    adc_init();

    if (cyw43_arch_init())
    {
        printf("Wi-Fi init failed");
        return -1;
    }

    if (Setup_Display() != 0)
    {
        return -1;
    }

    configure_pins();

    int led_toggle = 0;

    time_t led_start = time(NULL);

    UWORD *Frame = Loop_Init();

    struct RuntimeData data;
    Init_Enemies(&data);

    Spawn_Enemy(&data);

    data.size = 5;
    data.position.x = ((int) (LCD_2IN_WIDTH / 2)) - ((int) (data.size / 2));
    data.position.y = ((int) (LCD_2IN_HEIGHT / 2)) - ((int) (data.size / 2));

    data.speedOpt.normal = 1;
    data.speedOpt.fast = 6;
    data.speedOpt.usedFast = 0;
    data.speed = data.speedOpt.normal;

    data.energy = 0;
    data.health = 100;
    data.boost = 0;
    data.points = 0;

    data.gameOver = false;
    data.usingEnergy = false;

    data.last = timeInMilliseconds();

    int skipF = 0;

    bool paused = true;

    Paint_DrawString_EN(10, ((int) (LCD_2IN_HEIGHT / 2)) - 7, "Press [Any] To Start!", &Font16, BLACK, WHITE);
    LCD_2IN_Display((UBYTE *)Frame);

    while (true)
    {       
        if (data.gameOver) {
            cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
            break;
        };

        if (
            Get_BottomLeftPin() ||
            Get_BottomRightPin() ||
            Get_TopLeftPin() ||
            Get_TopRightPin() ||
            Get_JoyStickClick()) {
            paused = false;
        }
        if (paused) continue;

        Loop(&data);
        
        if (skipF > 6) {
            led_toggle = led_toggle == 0 ? 1 : 0;
            cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, led_toggle);
            
            skipF = 0;
            Loop_Render(Frame);

            // led_toggle = led_toggle == 0 ? 1 : 0;
            // cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, led_toggle);
        }
        skipF++;

        // time_t led_end = time(NULL);

        // if (led_end - led_start >= 0.25)
        // {
        //     led_toggle = led_toggle == 0 ? 1 : 0;
        //     cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, led_toggle);
        //     led_start = time(NULL);
        // }

        if (
            Get_BottomLeftPin() ||
            Get_BottomRightPin() ||
            Get_TopLeftPin() ||
            Get_TopRightPin() ||
            Get_JoyStickClick())
        {
            // printf("Total Memory: %lu bytes\n", getTotalHeap());
            // printf("Used Memory: %lu bytes\n", getFreeHeap());

            // printf("JX: %ld\n", Get_JoyStickX());
            // printf("JY: %ld\n", Get_JoyStickY());

            gpio_put(16, 1);
        }
        else
        {
            gpio_put(16, 0);
        }
    }

    GameOver(&data);
    Loop_Render(Frame);

    return 0;
}
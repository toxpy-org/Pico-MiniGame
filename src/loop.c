#include "./rtdata.c"
#include "./controls.c"
#include "./enemies.c"

long long timeInMilliseconds(void)
{
    struct timeval tv;

    gettimeofday(&tv, NULL);
    return (((long long)tv.tv_sec) * 1000) + (tv.tv_usec / 1000);
}

UWORD *Loop_Init()
{
    UDOUBLE Imagesize = LCD_2IN_HEIGHT * LCD_2IN_WIDTH * 2;
    UWORD *FrameImage;

    if ((FrameImage = (UWORD *)malloc(Imagesize)) == NULL)
    {
        // not enough ram in heap to run loop
        printf("not enough ram!");
        exit(0);
    }

    Paint_NewImage((UBYTE *)FrameImage, LCD_2IN.WIDTH, LCD_2IN.HEIGHT, 90, WHITE);
    Paint_SetScale(65);
    Paint_SetRotate(ROTATE_270);
    Paint_Clear(WHITE);

    return FrameImage;
}

void Loop_Cleanup(UWORD *FrameImage)
{
    free(FrameImage);
}

void Loop_Render(UWORD *FrameImage)
{
    LCD_2IN_Display((UBYTE *)FrameImage);
    Paint_Clear(BLACK);
}

void Loop_Controls(RTdata *data);

void Loop(RTdata *data)
{
    Loop_Controls(data);
    Enemies_Loop(data);

    float t = (float) ((timeInMilliseconds() - data->last) / 1000.0);
    data->last = timeInMilliseconds();
    
    int length_tt = snprintf(NULL, 0, "%f", t);
    char *str_tt = malloc(length_tt - 2);
    snprintf(str_tt, length_tt - 2, "%f", t);

    Paint_DrawString_EN(5, 5, str_tt, &Font12, WHITE, BLACK);
    Paint_DrawString_EN(42, 5, "s", &Font12, WHITE, BLACK);

    free(str_tt);

    // Paint_DrawRectangle(
    //     data->position.x, data->position.y,
    //     data->position.x + data->size, data->position.y + data->size,
    //     BLUE,
    //     DOT_PIXEL_1X1, DRAW_FILL_EMPTY);



    Paint_DrawCircle(
        data->position.x, data->position.y,
        data->size,
        data->usingEnergy ? MAGENTA : BLUE, 
        DOT_PIXEL_1X1, DRAW_FILL_EMPTY
    );


    // ENERGY BAR
    Paint_DrawRectangle(
        5, 30 + ( ( 1 - ((float) data->energy / 1000.0) ) * 50),
        10, 80,
        MAGENTA,
        DOT_PIXEL_1X1, DRAW_FILL_EMPTY);

    Paint_DrawRectangle(
        15, 30 + ( ( 1 - ((float) data->health / 100.0) ) * 50),
        20, 80,
        RED,
        DOT_PIXEL_1X1, DRAW_FILL_EMPTY);

    Paint_DrawRectangle(
        25, 30 + ( ( 1 - ((float) data->boost / 10) ) * 50),
        30, 80,
        BLUE,
        DOT_PIXEL_1X1, DRAW_FILL_EMPTY);

    if (data->health <= 0) {
        data->gameOver = true;
    }

    
    int length_pp = snprintf(NULL, 0, "%d", data->points);
    char *str_pp = malloc(length_pp + 1);
    snprintf(str_pp, length_pp + 1, "%d", data->points);

    Paint_DrawString_EN(180, 5, str_pp, &Font12, WHITE, BLACK);
    Paint_DrawString_EN(120, 5, "Score: ", &Font12, WHITE, BLACK);

    free(str_pp);
}

void GameOver(RTdata *data) {
    Paint_Clear(WHITE);
    
    int length_pp = snprintf(NULL, 0, "%d", data->points);
    char *str_pp = malloc(length_pp + 1);
    snprintf(str_pp, length_pp + 1, "%d", data->points);

    Paint_DrawString_EN(180, 5, str_pp, &Font12, BLACK, WHITE);
    Paint_DrawString_EN(120, 5, "Score: ", &Font12, BLACK, WHITE);

    free(str_pp);

    Paint_DrawString_EN(10, ((int) (LCD_2IN_HEIGHT / 2)) - 15, "GAME OVER!", &Font20, BLACK, WHITE);
    Paint_DrawString_EN(10, ((int) (LCD_2IN_HEIGHT / 2)) + 5, "Restart to play again!", &Font16, BLACK, WHITE);
}


bool Move(RTdata *data) {
    bool m = false;
    
    if (Get_JoyStickY() > 3700)
    {
        data->position.x -= data->speed;
        m = true;
    }
    if (Get_JoyStickY() < 1500)
    {
        data->position.x += data->speed;
        m = true;
    }
    if (Get_JoyStickX() > 3700)
    {
        data->position.y += data->speed;
        m = true;
    }
    if (Get_JoyStickX() < 1500)
    {
        data->position.y -= data->speed;
        m = true;
    }

    return m;
}

void Loop_Controls(RTdata *data)
{   
    int m = Move(data);

    if (Get_TopLeftPin())
    {
        if (data->boost > 0) 
        {
            data->boost--;
            data->speed = data->speedOpt.fast;
        } 
        else 
        {
            data->speed = data->speedOpt.normal;
        }
    }
    else
    {
        if (m != true) {
            if (data->speedOpt.usedFast > 10 && data->boost < 10) {
                data->speedOpt.usedFast = 0;
                data->boost++;
            }
        } else {
            if (data->speedOpt.usedFast > 25 && data->boost < 10) {
                data->speedOpt.usedFast = 0;
                data->boost++;
            }
        }

        if (data->boost < 10) {
            data->speedOpt.usedFast++;
        }

        data->speed = data->speedOpt.normal;
    }

    if (Get_TopRightPin()) {
        data->usingEnergy = true;

        if (data->energy <= 0) {
            data->usingEnergy = false;
        }

        if (data->usingEnergy) {
            data->energy -= 25;
        }
    } else {
        data->usingEnergy = false;
        if (data->energy < 0) data->energy = 0;
        if (data->energy < 1000) data->energy += 2;
    }

    if (data->position.x < 0)
    {
        data->position.x = 0;
    }

    if (data->position.y < 0)
    {
        data->position.y = 0;
    }

    if (data->position.x > (320 - data->size))
    {
        data->position.x = (320 - data->size);
    }

    if (data->position.y > (240 - data->size))
    {
        data->position.y = (240 - data->size);
    }
}
void configure_pins()
{
    adc_gpio_init(26); // y
    adc_gpio_init(27); // x

    // JOY STICK PRESS
    gpio_init(6);
    gpio_set_dir(6, GPIO_IN);
    gpio_pull_up(6);

    // MAIN ENERGY PROVIDER FOR IN PINS
    // Just because of the way I wire them is better to do
    // Do it this way
    gpio_init(0);
    gpio_set_dir(0, GPIO_OUT);

    // POWER ON | PIN (0)
    gpio_put(0, 1);

    // Left Bottom Button
    gpio_init(1);
    gpio_set_dir(1, GPIO_IN);

    // Right Bottom Button
    gpio_init(15);
    gpio_set_dir(15, GPIO_IN);

    //
    // LED OUTPUT
    gpio_init(16);
    gpio_set_dir(16, GPIO_OUT);
    //

    // Right Top Button
    gpio_init(14);
    gpio_set_dir(14, GPIO_IN);

    // Left Top Button
    gpio_init(5);
    gpio_set_dir(5, GPIO_IN);
}

bool Get_BottomLeftPin() { return gpio_get(1) == 1; }
bool Get_BottomRightPin() { return gpio_get(15) == 1; }
bool Get_TopLeftPin() { return gpio_get(5) == 1; }
bool Get_TopRightPin() { return gpio_get(14) == 1; }
bool Get_JoyStickClick() { return gpio_get(6) == 0; }
int Get_JoyStickY()
{
    adc_select_input(0);
    return adc_read();
}
int Get_JoyStickX()
{
    adc_select_input(1);
    return adc_read();
}

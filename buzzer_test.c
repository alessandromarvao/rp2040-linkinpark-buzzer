/**
 * Exemplo de acionamento do buzzer utilizando sinal PWM no GPIO 21 da Raspberry Pico / BitDogLab
 * 06/12/2024
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"

// Configuração do pino do buzzer
#define BUZZER_PIN 21

// Notas musicais para a música tema de Star Wars
const uint in_the_end_notes_buzzer_1[] = {
    330, 262, 262, 415, 392, 392, 392, 392, 415};

// Notas musicais para a música tema de Star Wars
const uint in_the_end_notes_buzzer_2[] = {
    1661, 1568, 1976, 1661, 1568, 1319};

// Duração das notas em milissegundos
const uint note_duration_buzzer_1[] = {
    536, 536, 536, 536, 536, 536, 536, 268, 268
};

// Duração das notas em milissegundos
const uint note_duration_buzzer_2[] = {
    268, 2144, 2144, 268, 2144, 2144
};

// Inicializa o PWM no pino do buzzer
void pwm_init_buzzer(uint pin)
{
    gpio_set_function(pin, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(pin);
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, 4.0f); // Ajusta divisor de clock
    pwm_init(slice_num, &config, true);
    pwm_set_gpio_level(pin, 0); // Desliga o PWM inicialmente
}

// Toca uma nota com a frequência e duração especificadas
void play_tone(uint pin, uint frequency, uint duration_ms)
{
    uint slice_num = pwm_gpio_to_slice_num(pin);
    uint32_t clock_freq = clock_get_hz(clk_sys);
    uint32_t top = clock_freq / frequency - 1;

    pwm_set_wrap(slice_num, top);
    pwm_set_gpio_level(pin, top / 2); // 50% de duty cycle

    sleep_ms(duration_ms);

    pwm_set_gpio_level(pin, 0); // Desliga o som após a duração
    sleep_ms(50);               // Pausa entre notas
}

// Função principal para tocar a música
void play_linkin_park(uint pin)
{
    for (int i = 0; i < sizeof(in_the_end_notes_buzzer_1) / sizeof(in_the_end_notes_buzzer_1[0]); i++)
    {
        play_tone(pin, star_wars_notes[i], note_duration[i]);
    }
}

int main()
{
    stdio_init_all();
    pwm_init_buzzer(BUZZER_PIN);
    while (1)
    {
        play_linkin_park(BUZZER_PIN);
    }

    return 0;
}
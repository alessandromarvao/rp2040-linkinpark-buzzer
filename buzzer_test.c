#include <stdio.h>                  // Biblioteca padrão de entrada e saída
#include "pico/stdlib.h"             // Biblioteca padrão do Raspberry Pi Pico
#include "hardware/pwm.h"            // Biblioteca para controle do PWM
#include "hardware/clocks.h"         // Biblioteca para controle de clocks

// Configuração do pino do buzzer
#define BUZZER_PIN 21                // Define o pino 21 como o pino de controle do buzzer

// Notas musicais para a música tema de Star Wars (Frequências das notas musicais em Hz)
const uint star_wars_notes[] = {
    330, 262, 262, 415, 392, 392, 392, 392, 415
};

// Duração das notas em milissegundos
const uint note_duration[] = {
    536, 536, 536, 536, 536, 536, 536, 268, 268
};

// Inicializa o PWM no pino do buzzer
void pwm_init_buzzer(uint pin) {
    // Define o pino como função PWM
    gpio_set_function(pin, GPIO_FUNC_PWM);
    // Obtém o número do slice do pino
    uint slice_num = pwm_gpio_to_slice_num(pin);
    // Cria uma configuração padrão de PWM
    pwm_config config = pwm_get_default_config();
    // Ajusta o divisor de clock para controlar a frequência
    pwm_config_set_clkdiv(&config, 4.0f);
    // Inicializa o PWM com a configuração
    pwm_init(slice_num, &config, true);
    // Inicialmente, desliga o PWM (nível 0)
    pwm_set_gpio_level(pin, 0);
}

// Toca uma nota com a frequência e duração especificadas
void play_tone(uint pin, uint frequency, uint duration_ms) {
    // Obtém o número do slice do pino
    uint slice_num = pwm_gpio_to_slice_num(pin);
    // Obtém a frequência do clock do sistema
    uint32_t clock_freq = clock_get_hz(clk_sys);
    // Calcula o valor do top para a frequência desejada
    uint32_t top = clock_freq / frequency - 1;

    // Define o valor de wrap para a frequência
    pwm_set_wrap(slice_num, top);
    // Define a largura do pulso (duty cycle) para 50%
    pwm_set_gpio_level(pin, top / 2);

    // Aguarda o tempo da nota
    sleep_ms(duration_ms);

    // Desliga o som após a duração da nota
    pwm_set_gpio_level(pin, 0);
    // Pausa entre as notas
    sleep_ms(50);
}

// Função principal para tocar a música
void play_song(uint pin) {
    // Loop através das notas e durações da música
    for (int i = 0; i < sizeof(star_wars_notes) / sizeof(star_wars_notes[0]); i++) {
        play_tone(pin, star_wars_notes[i], note_duration[i]);
    }
}

int main() {
    // Inicializa a comunicação serial
    stdio_init_all();
    // Inicializa o buzzer no pino definido
    pwm_init_buzzer(BUZZER_PIN);

    while (1) {
        // Toca a música do Star Wars repetidamente
        play_song(BUZZER_PIN);
    }

    return 0;
}

#include "com_github_lazygamer1111_components_output_ESC.h"

#include <iostream>
#include "test.pio.h"

PIO pio = pio0;

JNIEXPORT jint JNICALL Java_com_github_lazygamer1111_components_output_ESC_init_1SM(JNIEnv * env, jobject obj, jint pin, jint timeForZero) {
    stdio_init_all();

    int sm = pio_claim_unused_sm(pio, true);
    uint offset = pio_add_program(pio, &DSHOT_program);
    printf("Initialized on sm=%d, pin=%d, timeForZero=%d\n", sm, pin, timeForZero);

    pio_sm_config c = DSHOT_program_get_default_config(offset);
    // Map the state machine's OUT pin group to one pin, namely the `pin`
    // parameter to this function.
    sm_config_set_out_pins(&c, pin, 1);
    // Set this pin's GPIO function (connect PIO to the pad)
    pio_gpio_init(pio, pin);
    // Set the pin direction to output at the PIO
    pio_sm_set_consecutive_pindirs(pio, sm, pin, 1, true);

    
    // Get system clock
    int sysClock = clock_get_hz(clk_sys);
    // Calculate the division factor of the PIO processor
    float clockDiv = (sysClock) / (1.0E9f/(timeForZero/3)); // divider = clock / (1ns in seconds/(time for a zero to be registered / (cycles for a zero to be sent = 1 + delay = 3)))

    sm_config_set_clkdiv(&c, clockDiv);
    // Load our configuration, and jump to the start of the program
    pio_sm_init(pio, sm, offset, &c);
    
    // Set the state machine running
    pio_sm_set_enabled(pio, sm, true);

    return sm;
}

JNIEXPORT void JNICALL Java_com_github_lazygamer1111_components_output_ESC_put(JNIEnv * env, jobject obj, jint sm, jshort integer) {
    pio_sm_put_blocking(pio, sm, integer);
}

JNIEXPORT jshort JNICALL Java_com_github_lazygamer1111_components_output_ESC_pop(JNIEnv * env, jobject obj, jint sm) {
    return pio_sm_get_blocking(pio, sm);
}
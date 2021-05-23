#include <libopencm3/lm4f/gpio.h>
#include <libopencm3/lm4f/systemcontrol.h>
#include "adc.h"


inline enum lm4f_clken gpio_rcc_from(uint32_t port) {
    switch (port) {
    case GPIOA: return RCC_GPIOA;
    case GPIOB: return RCC_GPIOB;
    case GPIOC: return RCC_GPIOC;
    case GPIOD: return RCC_GPIOD;
    case GPIOE: return RCC_GPIOE;
    }
    return RCC_ADC0;  // error
}

/**
 * - see LM4F data sheet .
 */
void adc0_init(uint32_t port, uint8_t pins) {
    /// ### part 1. the module initialization. (according to 13.4.1)
    /// 1. Enable the ADC clock by RCGCADC
    periph_clock_enable(RCC_ADC0);
    /// 2. Enable the GPIO clock for analog input by RCGGPIO
    // enum lm4f_clken rcc = gpio_rcc_from(port);
    periph_clock_enable(RCC_GPIOE);
    /// 3. Set the GPIO `AFSEL` bits
    GPIO_AFSEL(port) |= pins;
    /// 4. Clear the `DEN` bit in `GPIOEN` register to be analog inputs.
    /// 5. Set `1` to the `GPIOAMSEL` to disbale the analog isolate circuit.
    gpio_mode_setup(port, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, pins);
        // TODO(shimoda): check to need to more or some...?
    /// 6. (optional) set sample sequencer priorities.
    // for now, not set => priority is 0 to 3.
}


/**
 * - see LM4F data sheet .
 */
uint32_t adc0_get(uint32_t mux) {

    /// ### part 2. the sequencer initialization. (according to 13.4.2)
    /// 1. Disable the sequencer by a register.
    ADCACTSS(ADC0) &= ~0xF;
    /// 2. set the trigger events by a register.
    // ADCEMUX(port) |= 0;  // processor
    /// 3. select input sources in samples.
    ADCSSMUX0(ADC0) |= mux;  // all input as AIN0 and AIN1
    /// 4. set the `END` bit to control bits.
    ADCSSCTL0(ADC0) = 0x20;  // only 1 sample to get.
    /// 5. setup interrupts.
    // for now, not set => no interrupts.
    /// 6. finally enable the sequencer logic by a register.
    ADCACTSS(ADC0) |= 1;  // enable seq #1

    ADCPSSI(ADC0) |= 1;  // enable seq #1

    for (int i = 0; i < 100000; i++) {
        if (ADCACTSS(ADC0) & 0x10000) {
            continue;
        }
        break;
    }
    return ADCSSFIFO0(ADC0) & 0xFFF;
}

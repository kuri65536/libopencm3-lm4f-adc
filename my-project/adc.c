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
void adc0_init(uint32_t port, uint8_t pin) {
    /// ### part 1. the module initialization. (according to 13.4.1)
    /// 1. Enable the ADC clock by RCGCADC
    periph_clock_enable(RCC_ADC0);
    /// 2. Enable the GPIO clock for analog input by RCGGPIO
    enum lm4f_clken rcc = gpio_rcc_from(port);
    periph_clock_enable(rcc);
    /// 3. Set the GPIO `AFSEL` bits
    GPIO_AFSEL(port) |= pin;  // do not use gpio_set_af by side effects.
    /// 4. Clear the `DEN` bit in `GPIOEN` register to be analog inputs.
    /// 5. Set `1` to the `GPIOAMSEL` to disbale the analog isolate circuit.
    gpio_mode_setup(port, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, pin);
        // TODO(shimoda): check to need to more or some...?
    /// 6. (optional) set sample sequencer priorities.
    // for now, not set => priority is 0 to 3.

    /// ### part 2. the sequencer initialization. (according to 13.4.2)
    /// 1. Disable the sequencer by a register.
    ADCACTSS(port) |= 1;
    /// 2. set the trigger events by a register.
    ADCEMUX(port) |= 1;
    /// 3. select input sources in samples.
    ADCSSMUX0(port) |= 1;
    /// 4. set the `END` bit to control bits.
    ADCSSCTL0(port) |= 1;
    /// 5. setup interrupts.
    // for now, not set => no interrupts.
    /// 6. finally enable the sequencer logic by a register.
    ADCACTSS(port) |= 1;
}


#include <libopencm3/lm4f/gpio.h>
#include <libopencm3/lm4f/rcc.h>
#include <libopencm3/lm4f/uart.h>

#include "adc.h"

uint16_t dec2ascii(uint8_t src);


#define DEBUG_UART 1


uint16_t dec2ascii(uint8_t src) {
    src = src % 10;
    return '0' + src;
}


int main(void) {
    gpio_enable_ahb_aperture();
    rcc_sysclk_config(OSCSRC_MOSC, XTAL_16M, 5);
    periph_clock_enable(RCC_GPIOF);
    gpio_mode_setup(GPIOF, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,
                    GPIO1 | GPIO2 | GPIO3);
    gpio_set_output_config(GPIOF, GPIO_OTYPE_PP, GPIO_DRIVE_2MA,
                           GPIO1 | GPIO2 | GPIO3);

    #if DEBUG_UART
    periph_clock_enable(RCC_GPIOC);
    gpio_mode_setup(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO5);
    gpio_set_af(GPIOC, 2, GPIO4 | GPIO5);

    periph_clock_enable(RCC_UART1);
    uart_clock_from_sysclk(UART1);
    uart_disable(UART1);
    uart_set_baudrate(UART1, 115200);
    uart_set_databits(UART1, 8);
    uart_set_stopbits(UART1, 1);
    uart_set_parity(UART1, UART_PARITY_NONE);
    // uart_set_mode(UART0, 1);
    uart_set_flow_control(UART1, UART_FLOWCTL_NONE);
    uart_enable(UART1);

    #endif

    #if 0
    adc_init(GPIOA, GPIO0);
    #endif

    while (1) {
        #if 0
        uint32_t val = adc_get();
        #else
        static uint32_t val = 0;
        val++;
        #endif

        #if DEBUG_UART
        uint8_t ch = dec2ascii(val / 10000);
        uart_send_blocking(UART1, ch);
        ch = dec2ascii(val / 1000);
        uart_send_blocking(UART1, ch);
        ch = dec2ascii(val / 100);
        uart_send_blocking(UART1, ch);
        ch = dec2ascii(val / 10);
        uart_send_blocking(UART1, ch);
        ch = dec2ascii(val);
        uart_send_blocking(UART1, ch);
        uart_send_blocking(UART1, (uint16_t)'\r');
        uart_send_blocking(UART1, (uint16_t)'\n');
        #endif
        for (int i = 0; i < 800000; i++) {
            __asm__("nop");
        }
        if (val & 1) {
            gpio_set(GPIOF, GPIO1 | GPIO2 | GPIO3);
        } else {
            gpio_clear(GPIOF, GPIO1 | GPIO2 | GPIO3);
        }
    }
    return 0;
}

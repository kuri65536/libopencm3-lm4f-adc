#include <libopencm3/lm4f/gpio.h>
#include <libopencm3/lm4f/rcc.h>
#include <libopencm3/lm4f/uart.h>

#include "adc.h"

uint16_t dec2ascii(uint8_t src);


#define DEBUG_UART 1
#define DEBUG_LED 1


uint16_t dec2ascii(uint8_t src) {
    src = src % 10;
    return '0' + src;
}


void out_digit(uint32_t val) {
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
}


int main(void) {
    gpio_enable_ahb_aperture();
    rcc_sysclk_config(OSCSRC_MOSC, XTAL_16M, 5);

    #if DEBUG_LED
    periph_clock_enable(RCC_GPIOF);
    gpio_mode_setup(GPIOF, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,
                    GPIO1 | GPIO2 | GPIO3);
    gpio_set_output_config(GPIOF, GPIO_OTYPE_PP, GPIO_DRIVE_2MA,
                           GPIO1 | GPIO2 | GPIO3);
    #endif

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

    adc0_init(GPIOE, GPIO2 | GPIO3);

    while (1) {
        static int loop_num = 0;
        uint32_t val = adc0_get(loop_num & 1 ? 0: 0x11111111);

        #if DEBUG_UART
        out_digit(loop_num++);
        uart_send_blocking(UART1, (uint16_t)'-');
        out_digit(val);
        uart_send_blocking(UART1, (uint16_t)'\r');
        uart_send_blocking(UART1, (uint16_t)'\n');
        #endif
        for (int i = 0; i < 800000; i++) {
            __asm__("nop");
        }

        #if DEBUG_LED
        if (val & 1) {
            gpio_set(GPIOF, GPIO1 | GPIO2 | GPIO3);
        } else {
            gpio_clear(GPIOF, GPIO1 | GPIO2 | GPIO3);
        }
        #endif
    }
    return 0;
}

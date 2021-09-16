#include <asf.h>
#include <board.h>
#include <gpio.h>
#include <sysclk.h>
#include "busy_delay.h"

#define CONFIG_USART_IF (AVR32_USART2)

// defines for BRTT interface
#define TEST_A      AVR32_PIN_PA31
#define RESPONSE_A  AVR32_PIN_PA30
#define TEST_B      AVR32_PIN_PA29
#define RESPONSE_B  AVR32_PIN_PA28
#define TEST_C      AVR32_PIN_PA27
#define RESPONSE_C  AVR32_PIN_PB00

volatile int interrupt_test_a = 0;
volatile int interrupt_test_b = 0;
volatile int interrupt_test_c = 0;


void respond(uint32_t pin){
	gpio_set_pin_low(pin);
	busy_delay_us(5);
	gpio_set_pin_high(pin);
}
__attribute__((__interrupt__)) static void interrupt_J3(void);

void init(){
    sysclk_init();
    board_init();
    busy_delay_init(BOARD_OSC0_HZ);
    
	gpio_configure_pin(TEST_A, GPIO_DIR_INPUT);
	gpio_configure_pin(TEST_B, GPIO_DIR_INPUT);
	gpio_configure_pin(TEST_C, GPIO_DIR_INPUT);
	gpio_configure_pin(RESPONSE_A, GPIO_DIR_OUTPUT | GPIO_INIT_HIGH);
	gpio_configure_pin(RESPONSE_B, GPIO_DIR_OUTPUT | GPIO_INIT_HIGH);
	gpio_configure_pin(RESPONSE_C, GPIO_DIR_OUTPUT | GPIO_INIT_HIGH);
	
    cpu_irq_disable();
    INTC_init_interrupts();
    INTC_register_interrupt(&interrupt_J3, AVR32_GPIO_IRQ_3, AVR32_INTC_INT1);
	gpio_enable_pin_interrupt(TEST_A, GPIO_FALLING_EDGE);
	gpio_enable_pin_interrupt(TEST_B, GPIO_FALLING_EDGE);
	gpio_enable_pin_interrupt(TEST_C, GPIO_FALLING_EDGE);
    cpu_irq_enable();
    
    stdio_usb_init(&CONFIG_USART_IF);

    #if defined(__GNUC__) && defined(__AVR32__)
        setbuf(stdout, NULL);
        setbuf(stdin,  NULL);
    #endif
}

__attribute__((__interrupt__)) static void interrupt_J3(void){ 
	if(gpio_get_pin_interrupt_flag(TEST_A)){
		gpio_clear_pin_interrupt_flag(TEST_A);
		interrupt_test_a++;
	}
	if(gpio_get_pin_interrupt_flag(TEST_B)){
		gpio_clear_pin_interrupt_flag(TEST_B); 
		interrupt_test_b++;
	}
	if(gpio_get_pin_interrupt_flag(TEST_C)){
		gpio_clear_pin_interrupt_flag(TEST_C);
		interrupt_test_c++;
	}
}


int main (void){
    init();
	while(1){
		if(interrupt_test_a){
			respond(RESPONSE_A);
			interrupt_test_a--;
		}
		if(interrupt_test_b){
			//busy_delay_us(100);
			respond(RESPONSE_B);
			interrupt_test_b--;
		}
		if(interrupt_test_c){
			respond(RESPONSE_C);
			interrupt_test_c--;
		}	
	}
	
	
	
	

}

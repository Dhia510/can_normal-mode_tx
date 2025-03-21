#include "stm32f4xx.h"
#include "can_driver.h"
#include "uart.h"
#include "bsp.h"
#include "fpu.h"
#include "adc.h"
#include "timebase.h"
#include <stdio.h>


#define  GPIOAEN		(1U<<0)
#define  PIN5			(1U<<5)
#define  LED_PIN		PIN5


uint8_t rx_data[5];

uint32_t tx_mailbox[3];

can_rx_header_typedef rx_header;
can_tx_header_typedef tx_header;

#define RX_DATA_STD_ID  0x244

uint8_t count = 0;
uint8_t message_buff[20];

void CAN1_RX0_IRQHandler(void)
{
	if((CAN1->RF0R & CAN_RF0R_FMP0) != 0U)
	{
		can_get_rx_message(CAN_RX_FIFO0, &rx_header, rx_data);

		sprintf((char *)message_buff, "New Data : %s",rx_data);
		printf("%s\n\r",message_buff);
		led_toggle();
		count++;
	}
}
int main()
{
	fpu_enable();
	timebase_init();
	debug_uart_init();
	led_init();
	can_gpio_init();
	can_params_init(CAN_MODE_NORMAL);
	can_filter_config(RX_DATA_STD_ID);
	can_start();


   printf("Receiver ready....\n\r");


	while(1)
	{

		tx_header.dlc = 10;
		tx_header.ext_id = 0;
		tx_header.ide = CAN_ID_STD;
		tx_header.rtr =  0;
		tx_header.std_id =  0x544;
		tx_header.transmit_global_time = 0;

		uint8_t tx_msg_pack[10] = "NODE2:Hi";

		can_add_tx_message(&tx_header, &tx_msg_pack[0],tx_mailbox);

		delay(1);

	}
}

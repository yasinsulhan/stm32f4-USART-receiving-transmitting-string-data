#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

char rx_buffer[20];
char control_buffer[5];
char tx_buffer[50];

int i = 0;

void delayy(uint32_t time)
{
	while(time--);
}


void GPIO_Config()
{

	RCC->AHB1ENR |= (1 << 3);											// Enable clock for GPIOD and Configure PD12 in output mode +

	GPIOD->MODER |= (1 << 24) | (1 << 26)  |  (1 << 28) | (1 << 30);	// Port D -> Pin 12 | Pin 13 | Pin 14 | Pin 15 Output selected +

	GPIOD->OSPEEDR |= (2 << 24) | (2 << 26)  |  (2 << 28) | (2 << 30);	// High Speed
	//GPIOD->PUPDR |= (2 << 24) | (2 << 26)  |  (2 << 28) | (2 << 30);	// Pull Down Mode
}


void RCC_Config()
{
	RCC->CR |= 0x00010000;	// HSEON enable
	while(!(RCC->CR & 0x00020000));	// HSEON Ready Flag wait
	RCC->CR |= 0x00080000;	// CSS Enable
	RCC->PLLCFGR |= 0x01000000;	// PLL on
	RCC->PLLCFGR |= 0x00000004;	// PLL M = 4
	RCC->PLLCFGR |= 0x00005A00;	// Pll N = 168
	RCC->PLLCFGR |= 0x00000000;	// PLL p = 2
	RCC->CFGR |= 0x00000000;	// AHB Prescaler = 1
	RCC->CFGR |= 0x00080000;	// APB2 Prescaler = 2
	RCC->CFGR |= 0x00001400;	// APB1 Prescaler = 4
	RCC->CIR |= 0x00800000;		// CSS Flag clear
}

void Uart2Config (void)
{
		/******* STEPS FOLLOWED ********

	1. Enable the UART CLOCK and GPIO CLOCK
	2. Configure the UART PINs for ALternate Functions
	3. Enable the USART by writing the UE bit in USART_CR1 register to 1.
	4. Program the M bit in USART_CR1 to define the word length.
	5. Select the desired baud rate using the USART_BRR register.
	6. Enable the Transmitter/Receiver by Setting the TE and RE bits in USART_CR1 Register

	********************************/

	// 1. Enable the UART CLOCK and GPIO CLOCK
	RCC->APB1ENR |= (1<<17);  // Enable UART2 CLOCK
	RCC->AHB1ENR |= (1<<0); // Enable GPIOA CLOCK

	// 2. Configure the UART PINs for ALternate Functions
	GPIOA->MODER |= (2<<4);  // Bits (5:4)= 1:0 --> Alternate Function for Pin PA2
	GPIOA->MODER |= (2<<6);  // Bits (7:6)= 1:0 --> Alternate Function for Pin PA3

	GPIOA->OSPEEDR |= (3<<4) | (3<<6);  // Bits (5:4)= 1:1 and Bits (7:6)= 1:1 --> High Speed for PIN PA2 and PA3

	GPIOA->AFR[0] |= (7<<8);  // Bytes (11:10:9:8) = 0:1:1:1  --> AF7 Alternate function for USART2 at Pin PA2
	GPIOA->AFR[0] |= (7<<12); // Bytes (15:14:13:12) = 0:1:1:1  --> AF7 Alternate function for USART2 at Pin PA3

	// 3. Enable the USART by writing the UE bit in USART_CR1 register to 1.
	USART2->CR1 = 0x00;  // clear all
	USART2->CR1 |= (1<<13);  // UE = 1... Enable USART

	// 4. Program the M bit in USART_CR1 to define the word length.
	USART2->CR1 &= ~(1<<12);  // M =0; 8 bit word length

	// 5. Select the desired baud rate using the USART_BRR register.
	//USART2->BRR = (7<<0) | (24<<4);   // Baud rate of 115200, PCLK1 at 45MHz
	USART2->BRR = 0x1112;			// Baud Rate 9600

	// 6. Enable the Transmitter/Receiver by Setting the TE and RE bits in USART_CR1 Register
	USART2->CR1 |= (1<<2); // RE=1.. Enable the Receiver
	USART2->CR1 |= (1<<3);  // TE=1.. Enable Transmitter
}


void UART2_SendChar (uint8_t c)
{

	USART2->DR = c; 			      	 // load the data into DR register
	while (!(USART2->SR & (1<<6)));  	// Wait for TC to SET.. This indicates that the data has been transmitted
}

void UART2_SendString (char *string)
{
	while (*string) UART2_SendChar (*string++);
}


void assign_the_data(char *buffer)
{
    char* ptr;
    ptr = &buffer[0];

    for(int i=0; i<20; i++)
    {
    	while (!(USART2->SR & (1<<5)));
        *(ptr+i) = USART2->DR;

        if(*(ptr+i) == '\n')
        	return;
    }
}


int main (void)
{
	RCC_Config();
	Uart2Config ();
	GPIO_Config();


	while (1)
	{
		assign_the_data(rx_buffer);


		if(rx_buffer[0] == 'l' && rx_buffer[1] == 'e' && rx_buffer[2] == 'd' && rx_buffer[3] == '1' && rx_buffer[4] == ' ' &&
			rx_buffer[5] == 'o' && rx_buffer[6] == 'n')
		{
			GPIOD->ODR |= (1 << 12);
			UART2_SendString("led1 is on.\n");
		}

		else if(rx_buffer[0] == 'l' && rx_buffer[1] == 'e' && rx_buffer[2] == 'd' && rx_buffer[3] == '1' && rx_buffer[4] == ' ' &&
			rx_buffer[5] == 'o' && rx_buffer[6] == 'f' && rx_buffer[7] == 'f')
		{
			GPIOD->ODR &= ~(1 << 12);
			UART2_SendString("led1 is off.\n");
		}

		if(rx_buffer[0] == 'l' && rx_buffer[1] == 'e' && rx_buffer[2] == 'd' && rx_buffer[3] == '2' && rx_buffer[4] == ' ' &&
			rx_buffer[5] == 'o' && rx_buffer[6] == 'n')
		{
			GPIOD->ODR |= (1 << 13);
			UART2_SendString("led2 is on.\n");
		}

		else if(rx_buffer[0] == 'l' && rx_buffer[1] == 'e' && rx_buffer[2] == 'd' && rx_buffer[3] == '2' && rx_buffer[4] == ' ' &&
			rx_buffer[5] == 'o' && rx_buffer[6] == 'f' && rx_buffer[7] == 'f')
		{
			GPIOD->ODR &= ~(1 << 13);
			UART2_SendString("led2 is off.\n");
		}

		if(rx_buffer[0] == 'l' && rx_buffer[1] == 'e' && rx_buffer[2] == 'd' && rx_buffer[3] == '3' && rx_buffer[4] == ' ' &&
			rx_buffer[5] == 'o' && rx_buffer[6] == 'n')
		{
			GPIOD->ODR |= (1 << 14);
			UART2_SendString("led3 is on.\n");
		}

		else if(rx_buffer[0] == 'l' && rx_buffer[1] == 'e' && rx_buffer[2] == 'd' && rx_buffer[3] == '3' && rx_buffer[4] == ' ' &&
			rx_buffer[5] == 'o' && rx_buffer[6] == 'f' && rx_buffer[7] == 'f')
		{
			GPIOD->ODR &= ~(1 << 14);
			UART2_SendString("led3 is off.\n");
		}

		if(rx_buffer[0] == 'l' && rx_buffer[1] == 'e' && rx_buffer[2] == 'd' && rx_buffer[3] == '4' && rx_buffer[4] == ' ' &&
			rx_buffer[5] == 'o' && rx_buffer[6] == 'n')
		{
			GPIOD->ODR |= (1 << 15);
			UART2_SendString("led4 is on.\n");
		}

		else if(rx_buffer[0] == 'l' && rx_buffer[1] == 'e' && rx_buffer[2] == 'd' && rx_buffer[3] == '4' && rx_buffer[4] == ' ' &&
			rx_buffer[5] == 'o' && rx_buffer[6] == 'f' && rx_buffer[7] == 'f')
		{
			GPIOD->ODR &= ~(1 << 15);
			UART2_SendString("led4 is off.\n");
		}

		delayy(1000);

	}
}

void EVAL_AUDIO_TransferComplete_CallBack(uint32_t pBuffer, uint32_t Size){
  /* TODO, implement your code here */
  return;
}


uint16_t EVAL_AUDIO_GetSampleCallBack(void){
  /* TODO, implement your code here */
  return -1;
}

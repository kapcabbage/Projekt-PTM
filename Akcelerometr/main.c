#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_conf.h"
#include "stm32f4_discovery_lis302dl.h"
#include "stm32f4xx_usart.h"

int8_t acc_x;
int8_t acc_y;
int8_t acc_z;
int stop = 1;
uint8_t globalInt;

void InitButton() {
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_InitTypeDef GPIO_InitDef;
	//Pin 0
	GPIO_InitDef.GPIO_Pin = GPIO_Pin_0;
	//Mode output
	GPIO_InitDef.GPIO_Mode = GPIO_Mode_IN;
	//Output type push-pull
	GPIO_InitDef.GPIO_OType = GPIO_OType_PP;
	//With pull down resistor
	GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_DOWN;
	//50MHz pin speed
	GPIO_InitDef.GPIO_Speed = GPIO_Speed_50MHz;

	//Initialize pin on GPIOA port
	GPIO_Init(GPIOA, &GPIO_InitDef);
}

void InitUsart(){
	// wlaczenie taktowania wybranego portu

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	// wlaczenie taktowania wybranego uk³adu USART
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	// konfiguracja linii Tx
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	// konfiguracja linii Rx
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	USART_InitTypeDef USART_InitStructure;
	// predkosc transmisji (mozliwe standardowe opcje: 9600, 19200, 38400, 57600, 115200, ...)
	USART_InitStructure.USART_BaudRate = 9600;
	// d³ugoœæ s³owa (USART_WordLength_8b lub USART_WordLength_9b)
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	// liczba bitów stopu (USART_StopBits_1, USART_StopBits_0_5, USART_StopBits_2, USART_StopBits_1_5)
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	// sprawdzanie parzystoœci (USART_Parity_No, USART_Parity_Even,	USART_Parity_Odd)
	USART_InitStructure.USART_Parity = USART_Parity_No;
	// sprzêtowa kontrola przep³ywu (USART_HardwareFlowControl_None, USART_HardwareFlowControl_RTS, USART_HardwareFlowControl_CTS, USART_HardwareFlowControl_RTS_CTS)
	USART_InitStructure.USART_HardwareFlowControl =	USART_HardwareFlowControl_None;
	// tryb nadawania/odbierania (USART_Mode_Rx, USART_Mode_Rx )
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	// konfiguracja
	USART_Init(USART3, &USART_InitStructure);

	// wlaczenie ukladu USART
	USART_Cmd(USART3, ENABLE);}

void InitDiody(){
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
		GPIO_InitTypeDef  Diody;
		Diody.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15;
		Diody.GPIO_Mode = GPIO_Mode_OUT;
		Diody.GPIO_OType = GPIO_OType_PP;
		Diody.GPIO_Speed = GPIO_Speed_100MHz;
		Diody.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_Init(GPIOD, &Diody);

}
void InitAkcelerometr(){
	LIS302DL_InitTypeDef  LIS302DL_InitStruct;
		// Set configuration of LIS302DL
		LIS302DL_InitStruct.Power_Mode = LIS302DL_LOWPOWERMODE_ACTIVE;
		LIS302DL_InitStruct.Output_DataRate = LIS302DL_DATARATE_100;
		LIS302DL_InitStruct.Axes_Enable = LIS302DL_X_ENABLE | LIS302DL_Y_ENABLE | LIS302DL_Z_ENABLE;
		LIS302DL_InitStruct.Full_Scale = LIS302DL_FULLSCALE_2_3;
		LIS302DL_InitStruct.Self_Test = LIS302DL_SELFTEST_NORMAL;
		LIS302DL_Init(&LIS302DL_InitStruct);
}
int main(void)
{
	SystemInit();
	SystemCoreClockUpdate();
	InitDiody();
	InitAkcelerometr();
	InitUsart();
	InitButton();


    while(1)
    {/*USART_SendData(USART3, 'A');
	// czekaj az dane zostana wyslane
	while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET) {}
	while (USART_GetFlagStatus(USART3, USART_FLAG_RXNE) == RESET) {}
	globalInt = USART_ReceiveData(USART3);*/



    	LIS302DL_Read(&acc_x, LIS302DL_OUT_X_ADDR, 1);
    	LIS302DL_Read(&acc_y, LIS302DL_OUT_Y_ADDR, 1);
    	LIS302DL_Read(&acc_z, LIS302DL_OUT_Z_ADDR, 1);
    	if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)== 0 && stop == 1 )
    	{
    		stop = 0;
    		USART_SendData(USART3, 'z');

    		   while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET) {}
    	}
    	else if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)!= 0 && stop == 0)
    	{
    		stop = 1;
    		USART_SendData(USART3, 'z');

    		   while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET) {}
    	}
    	if(acc_x>15){
    			GPIO_SetBits(GPIOD, GPIO_Pin_15);
    			USART_SendData(USART3, 'a');
    					// czekaj az dane zostana wyslane
    					while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET) {}
    			}
    	else if(acc_x<-15) {
    			GPIO_SetBits(GPIOD, GPIO_Pin_13);
    			USART_SendData(USART3, 'd');
    			    					// czekaj az dane zostana wyslane
    			    					while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET) {}
    			}
    	else if(acc_y>15) {
    	    	GPIO_SetBits(GPIOD, GPIO_Pin_14);
    	    	USART_SendData(USART3, 's');
    	    	    					// czekaj az dane zostana wyslane
    	    	    					while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET) {}
    	    	}
    	else if(acc_y<-15) {
    	    	GPIO_SetBits(GPIOD, GPIO_Pin_12);
    	    	USART_SendData(USART3, 'w');
				// czekaj az dane zostana wyslane
				while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET) {}
    	    	}/*
    	else if(acc_z>45) {
    	    	GPIO_SetBits(GPIOD,GPIO_Pin_12 | GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15);
    	       	}
    	else if(acc_z<-45) {
    	      	GPIO_SetBits(GPIOD,GPIO_Pin_12 | GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15);
    	       	}*/
    	else{
    			GPIO_ResetBits(GPIOD, GPIO_Pin_12 | GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15);
    	}
    }
}

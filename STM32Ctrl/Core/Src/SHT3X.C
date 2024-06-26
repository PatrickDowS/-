
#include "stm32l4xx_hal.h"
#include "SHT3X.h"
#include "main.h"

#define SHT3XAddr 0x44
#define write 0
#define read  1

uint16_t buff[6];
float Temperature = 0;
float Humidity = 0;

void SHT3XI2CDelay()
{
	uint8_t i;
	for(i = 0; i < 2; i++)
	{
		__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
	}
}

void SHT3X_IIC_Start()
{
	HAL_GPIO_WritePin(SHT30_SDA_GPIO_Port, SHT30_SDA_Pin, GPIO_PIN_SET); //????SH_SDA?SH_SCL?????
	HAL_GPIO_WritePin(SHT30_SCK_GPIO_Port, SHT30_SCK_Pin, GPIO_PIN_SET);
	SHT3XI2CDelay();
	HAL_GPIO_WritePin(SHT30_SDA_GPIO_Port, SHT30_SDA_Pin, GPIO_PIN_RESET);//???SH_SDA
	SHT3XI2CDelay();
	HAL_GPIO_WritePin(SHT30_SCK_GPIO_Port, SHT30_SCK_Pin, GPIO_PIN_RESET); //???SH_SCL
	SHT3XI2CDelay();
}
/* ???????? */
void SHT3X_IIC_Stop()
{
	HAL_GPIO_WritePin(SHT30_SCK_GPIO_Port, SHT30_SCK_Pin, GPIO_PIN_RESET); //????SH_SDA?SH_SCL?????
	HAL_GPIO_WritePin(SHT30_SDA_GPIO_Port, SHT30_SDA_Pin, GPIO_PIN_RESET);
	SHT3XI2CDelay();
	HAL_GPIO_WritePin(SHT30_SCK_GPIO_Port, SHT30_SCK_Pin, GPIO_PIN_SET); //???SH_SCL
	SHT3XI2CDelay();
	HAL_GPIO_WritePin(SHT30_SDA_GPIO_Port, SHT30_SDA_Pin, GPIO_PIN_SET); //???SH_SDA
	SHT3XI2CDelay();
}
/* I2C?????,dat-?????,???-??????? */
uint8_t SHT3X_IIC_SendData(uint8_t dat)
{
	uint8_t ack; //?????????
	uint8_t mask;  //????????????????

	for (mask = 0x80; mask != 0; mask >>= 1) //??????????
	{
		if ((mask & dat) == 0) //???????SH_SDA?
		{
			HAL_GPIO_WritePin(SHT30_SDA_GPIO_Port, SHT30_SDA_Pin, GPIO_PIN_RESET);
		}
		else
		{
			HAL_GPIO_WritePin(SHT30_SDA_GPIO_Port, SHT30_SDA_Pin, GPIO_PIN_SET);
		}
		SHT3XI2CDelay();
		HAL_GPIO_WritePin(SHT30_SCK_GPIO_Port, SHT30_SCK_Pin, GPIO_PIN_SET);          //??SH_SCL
		SHT3XI2CDelay();
		HAL_GPIO_WritePin(SHT30_SCK_GPIO_Port, SHT30_SCK_Pin, GPIO_PIN_RESET);          //???SH_SCL,???????
	}
	SHT3XI2CDelay();
	HAL_GPIO_WritePin(SHT30_SDA_GPIO_Port, SHT30_SDA_Pin, GPIO_PIN_SET);   //8???????,????SH_SDA,???????
	SHT3XI2CDelay();
	HAL_GPIO_WritePin(SHT30_SCK_GPIO_Port, SHT30_SCK_Pin, GPIO_PIN_SET);   //??SH_SCL
	
	GPIO_PinState SDOState = HAL_GPIO_ReadPin(SHT30_SDA_GPIO_Port, SHT30_SDA_Pin);
		if(SDOState == GPIO_PIN_RESET)
		{
			dat &= ~mask;  //?0?,dat??????
		}
		else
		{
			dat |= mask;  //?1?,dat?????1
		}
		
	SHT3XI2CDelay();
	HAL_GPIO_WritePin(SHT30_SCK_GPIO_Port, SHT30_SCK_Pin, GPIO_PIN_RESET);   //???SH_SCL?????,??????
	SHT3XI2CDelay();

	return ack; //?????????????:
	//0=??????????,1=??????????
}
/* I2C?????,????????,???-????? */
uint8_t SHT3X_IIC_ReadNACK()
{
	uint8_t mask;
	uint8_t dat;

	HAL_GPIO_WritePin(SHT30_SDA_GPIO_Port, SHT30_SDA_Pin, GPIO_PIN_SET);  //????????SH_SDA
	for (mask = 0x80; mask != 0; mask >>= 1) //??????????
	{
		SHT3XI2CDelay();
		HAL_GPIO_WritePin(SHT30_SCK_GPIO_Port, SHT30_SCK_Pin, GPIO_PIN_SET);      //??SH_SCL
		
		GPIO_PinState SDOState = HAL_GPIO_ReadPin(SHT30_SDA_GPIO_Port, SHT30_SDA_Pin);
		if(SDOState == GPIO_PIN_RESET)
		{
			dat &= ~mask;  //?0?,dat??????
		}
		else
		{
			dat |= mask;  //?1?,dat?????1
		}
		
		SHT3XI2CDelay();
		HAL_GPIO_WritePin(SHT30_SCK_GPIO_Port, SHT30_SCK_Pin, GPIO_PIN_RESET);      //???SH_SCL,??????????
	}
	HAL_GPIO_WritePin(SHT30_SDA_GPIO_Port, SHT30_SDA_Pin, GPIO_PIN_SET);   //8???????,??SH_SDA,???????
	SHT3XI2CDelay();
	HAL_GPIO_WritePin(SHT30_SCK_GPIO_Port, SHT30_SCK_Pin, GPIO_PIN_SET);   //??SH_SCL
	SHT3XI2CDelay();
	HAL_GPIO_WritePin(SHT30_SCK_GPIO_Port, SHT30_SCK_Pin, GPIO_PIN_RESET);   //???SH_SCL??????,??????
	SHT3XI2CDelay();

	return dat;
}
/* I2C?????,???????,???-????? */
uint8_t SHT3X_IIC_ReadACK()
{
	uint8_t mask;
	uint8_t dat;

	HAL_GPIO_WritePin(SHT30_SDA_GPIO_Port, SHT30_SDA_Pin, GPIO_PIN_SET);  //????????SH_SDA
	for (mask = 0x80; mask != 0; mask >>= 1) //??????????
	{
		SHT3XI2CDelay();
		HAL_GPIO_WritePin(SHT30_SCK_GPIO_Port, SHT30_SCK_Pin, GPIO_PIN_SET);      //??SH_SCL
		
		GPIO_PinState SDOState = HAL_GPIO_ReadPin(SHT30_SDA_GPIO_Port, SHT30_SDA_Pin);
		if(SDOState == GPIO_PIN_RESET)
		{
			dat &= ~mask;  //?0?,dat??????
		}
		else
		{
			dat |= mask;  //?1?,dat?????1
		}
		
		SHT3XI2CDelay();
		HAL_GPIO_WritePin(SHT30_SCK_GPIO_Port, SHT30_SCK_Pin, GPIO_PIN_RESET);      //???SH_SCL,??????????
	}
	HAL_GPIO_WritePin(SHT30_SDA_GPIO_Port, SHT30_SDA_Pin, GPIO_PIN_RESET);  //8???????,??SH_SDA,??????
	SHT3XI2CDelay();
	HAL_GPIO_WritePin(SHT30_SCK_GPIO_Port, SHT30_SCK_Pin, GPIO_PIN_SET);   //??SH_SCL
	SHT3XI2CDelay();
	HAL_GPIO_WritePin(SHT30_SCK_GPIO_Port, SHT30_SCK_Pin, GPIO_PIN_RESET);   //???SH_SCL?????,??????
	SHT3XI2CDelay();

	return dat;
}

void Init_SHT3X()  
{
	SHT3X_IIC_Stop();
}

/*******************************************************************
 ???????               
????: SHT30_read_result(u8 addr);
??: ???????????????
********************************************************************/ 
void SHT30_read_result(float *pTemperature, float *pHumidity)
{
	uint16_t tem,hum;
	
	SHT3X_IIC_Start();
	SHT3X_IIC_SendData(SHT3XAddr << 1 | write);//?7?I2C?????0?????,1????
	SHT3X_IIC_SendData(0x2C);
	SHT3X_IIC_SendData(0x06);
	//SHT3X_IIC_Stop();
	HAL_Delay(20);
	
	SHT3X_IIC_Start();
	SHT3X_IIC_SendData(SHT3XAddr << 1 | read);//?7?I2C?????0?????,1????

	buff[0]=SHT3X_IIC_ReadACK();
	buff[1]=SHT3X_IIC_ReadACK();
	buff[2]=SHT3X_IIC_ReadACK();
	buff[3]=SHT3X_IIC_ReadACK();
	buff[4]=SHT3X_IIC_ReadACK();
	buff[5]=SHT3X_IIC_ReadNACK();
	SHT3X_IIC_Stop();
	
	tem = ((buff[0]<<8) | buff[1]);//????
	hum = ((buff[3]<<8) | buff[4]);//????
	
	/*??????*/
	Temperature= (175.0*(float)tem/65535.0-45.0) ;// T = -45 + 175 * tem / (2^16-1)
	Humidity= (100.0*(float)hum/65535.0);// RH = hum*100 / (2^16-1)
	(*pTemperature) = Temperature;
	(*pHumidity) = Humidity;
}

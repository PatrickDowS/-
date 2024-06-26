#ifndef	__STC32G_SHT3X_H
#define	__STC32G_SHT3X_H


//sbit SH_SDA = P1^3;
//sbit SH_SCL = P1^7;

void Init_SHT3X();
void SHT30_read_result(float *pTemperature, float *pHumidity);

#endif
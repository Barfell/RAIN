#ifndef __LTC2402_H
#define __LTC2402_H

#define LTC2402_ReferenceResistance     19600.0
#define LTC2402_ReferenceVoltage        4.99

#define PT_ZeroTemRes    100
#define PT_A 	3.9083e-3
#define PT_B    -5.775e-7
#define PT_C    -4.183e-12
#define PT_Min	5
#define PT_Max	200
#define	NTC_A	1.4051e-3
#define NTC_B	2.369e-4
#define NTC_C	1.019e-7
#define NTC_Min	250
#define	NTC_Max 100000

void LTC2402Init(void);
unsigned char LTC2402_ReadByte(void);
unsigned char LTC2402_TEST_EOC(void);
double LTC2402_GetResistance(unsigned char channel);
double GetNTCTemperature(double NTCRes);

#endif


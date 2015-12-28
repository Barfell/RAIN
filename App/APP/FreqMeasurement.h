#ifndef __FEQMEAUREMENT_H
#define __FEQMEAUREMENT_H

void Time3_Config(void);
void Time2_Config(void); 
void Time4_Config(void);  
double CalDisplacement(double Freq,double T);
extern double OnFreq;
extern unsigned char measureflag;
extern unsigned char PulseOnFlag;
void FreqModuleInit(void);
double Measure(void);
void TranPulse(unsigned int freq,unsigned char num);
double GetFreq(unsigned char ch);
void OutPutFreqSet(unsigned int freq);

#endif







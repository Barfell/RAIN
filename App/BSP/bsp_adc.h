#ifndef _BSP_ADC_H_
#define _BSP_ADC_H_
void voltage_adc_init(void);
unsigned int get_adc_value(void);
double get_dev_voltage(unsigned int value);
#endif


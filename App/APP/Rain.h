#ifndef _RAIN_H_
#define _RAIN_H_

#define RAINNUMBERS_ADDR  0x080c0000  //雨量个数 地址，占4个字节      sector10
#define RAINDATA_ADDR     0x080e0000  //雨量的时间数据开始存放的地址  sector11

void rain_gpio(void);

#endif


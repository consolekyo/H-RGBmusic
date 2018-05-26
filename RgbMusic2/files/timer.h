#ifndef TIMER_H
#define TIMER_H 0



#ifndef TIMER_THIS  //在非timer.c文件include本文件，可以调用以下变量
extern unsigned char T0RH;
extern unsigned char T0RL;

void ConfigTimer0(unsigned int ms);
void Config_T0_100us(unsigned int time);
#endif

#endif


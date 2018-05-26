#include "60s2.h"
void pwm_init(void) //PWM初始化函数，初始占空比为0
{
    CMOD=0x02; //0000 0010 空闲时不计数，不产生中断，时钟源为fosc/2，因此输出占用率为fosc/512
    CL=0x00;
    CH=0x00;
    CCAPM0=0x42; //0100 0010， 8位 PWM
    CCAP0L=0x00;
    CCAP0H=0x00; 
    //PWM实现方法，因为是８位，CL基础计数器从0xFF递减到０x00，溢出后将CCAP0H
    //的值加载到CCAP0L，若CL递减过程中，若大于CCAP0L，则输出高电平，否则为低电平
    CCAPM1=0x42;
    CCAP1H=0x00;
    CCAP1L=0x00;
    CR=1; //开启PCA计数器
}
/*****************************************************/
void pwm_set(unsigned char x, unsigned char y) //占空比设置函数
{
    CCAP0H=x; //设置比较值,'占空比'=CCAP0H/255
    CCAP0L=x; //CL溢出导致L自动装载H的值
    CCAP1H=y;
    CCAP1L=y;
}
/**************************************************/

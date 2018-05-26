    ///
#include "60s2.h"
#include "main.h"
#include "mainconfig.h"
#include "pwm.h"
	///数据文件
#include "pcmdata.c"


sbit DAC_out = P2^1;


    ///本文件内函数声明
void initwav();	    //播放模块初始化
void wavmain();	    //播放循环	


    ///函数正文
void initwav(){
    
    pwm_init(); //config PWM reg
    
    //配置P2口高阻，防止烧PWM口
    P2M1 = 0xff;
    P2M0 = 0x00;
    //P1.5高阻
    P1M1 |= 0x01<<5;
    P1M0 &= 0x01<<5;
}

void wavmain(){
    u16 tmp = 0;
    u16 i = 0;
    
        while(tmp <= pcmlth){
            
            pwm_set(pcmdata[tmp], 255-pcmdata[tmp]); 
            for(i = 50; i; i--);// 延时100 样本不跳 为正常速度
            
            tmp++;
            //tmp += 100;
        }
        
} 

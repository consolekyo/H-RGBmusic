    ///
#include "60s2.h"
#include "main.h"
#include "mainconfig.h"
#include "pwm.h"
	///数据文件
#include "pcmdata.c"

    ///全局变量

u8 tonebuf[8][2] = {0}; //播放配置缓存区
                        //tonebuf[x][y]: 第x个音; 倍速播放:每隔[y=0]个,取([y=1]+1)个样本
u16 tonept[8] = {0};    //样本‘数指针’ 正在播放;
u8 tonebufpt = 0;  //缓存区输入 数指针 0~7




    ///本文件内函数声明
void initwav();	    //播放模块初始化
void wavmain();	    //播放循环	
void Toneconfig(u8 tone);   //配置播放信息
void t0wav();       //T0中断-播放模块 
void Pwmplay();     //准备输出PWM
u8 PCMprocess();    //混音处理PWM振幅
u8 PCMread( u8 i);  //播放单个音


    ///重载函数（C++功能）
void plus8(u8* var){    //加到8归零
    (*var)++;
    (*var) &= 0x07;
}


    ///函数正文
void initwav(){
    u8 i;
    pwm_init(); //config PWM reg
    tonebufpt = 0;
    for(i=0;i<8;i++)tonept[i] = pcmlth;
}

void wavmain(){
    if(colnum){
        //接受颜色变量到buf
        Toneconfig(colnum);   //配置播放信息
        plus8(&tonebufpt);      
        
        colnum = 0; //清0信号变量
    }
} 

void Toneconfig(u8 tone){   //根据音调配置播放信息
    static u8 toneinfo[8][2]={ //配置信息(播放时长)对照表
        0,0,    //[tone=0]全取，最低音
        1,4,
        1,3,
        1,2,
        1,1,
        1,0,    //[tone=5]隔1个取1个
        2,0,
        3,0     
    };
    tonebuf[tonebufpt][0] = toneinfo[tone][0];
    tonebuf[tonebufpt][1] = toneinfo[tone][1];
    tonept[tonebufpt] = 0;  //正在播放指针清0
}

void t0wav(){   //定时器(12分频后)接收脉冲0.921MHZ，音频采样率11.025 or 8 KHZ
                //定时器 1ms <=> 1KHZ  ,  100us <=> 10KHZ 
                /**
                static u8 cpwm = 0;
                cpwm++;
                if(cpwm>=83){
                    cpwm = 0;
                    Pwmplay();
                }
                **/
    Pwmplay();
}    

void Pwmplay(){
    u8 pcm;
    pcm = PCMprocess();
    pwm_set( pcm, pcm);    //PWM输出占空比
}

u8 PCMprocess(){
    u8 i, tmp, num=0; //计数，传值，有效需混音的buf数量
    u16 rcv = 0;    //临时 求和  

    for(i=0;i<8;i++){
        tmp = PCMread( i);
        rcv += tmp;
        if( tmp){num++;}
    }
    if( num){rcv /= num;}    //平均数混音法
    return rcv;
}

u8 PCMread( u8 i){  //播放单个音
    static u8 btrbuf[8] = {0};    //连续取样计数标记boot read buffer
    if( btrbuf[i] != 0){
        tonept[i]++;    //下一个样本
        btrbuf[i]--;
    }else{
        tonept[i]++;
        tonept[i] += tonebuf[i][0];//额外跳过一些样本
        btrbuf[i] =  tonebuf[i][1];//重载 连续取样计数
    }
    if( tonept[i] >= pcmlth){
        return 0;   //返0表此音已播放完
    }else{
        return pcmdata[tonept[i]];
    }
}
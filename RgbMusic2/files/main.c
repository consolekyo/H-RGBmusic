    ///头文件
#include "60s2.h"
//#include <reg52.h>
#include "mainconfig.h"
#include "pwm.h"
#include "tcs.h"

#include "pcmdata.c"


u8 tonecol = 0;
u8 MAINMODE = 0;
u8 SSTOP = 0;


#if 1
    u16 colprop[8][2] = {
        2000,100,
        1500,133,
        1000,200,
        900,222,
        800,250,
        700,285,
        600,300,
        400,500//音调最高
        
    };  //颜色x对应音调的 (1)dtime = bz翻转延迟 (2)ptime = 播放时长，p*d=播放真实时长(我取的是200000)
#endif

    
    
sbit BZ = P1^5; //BETA   
sbit EN1 = P2^5;//S1S0
sbit EN2 = P2^6;

void MCUinit();
void tellcol();
void wavmain(u16 dtime, u16 ptime);
void Int0Init();
void wPWMplay(u8 speed);
void PWMout();

    
//主函数
void main(){
    
    MCUinit();
    
    while(1){   
        
        switch(MAINMODE){
            case 0:
                wavmain(colprop[tonecol][0], colprop[tonecol][1]);      //蜂鸣器响(读取tonecol)
                break;
            case 1:
                PWMout();
                break;
        }
        
        while(SSTOP == 0);   //BETA:卡住
        SSTOP = 0;
        
    }

}



void MCUinit(){
    /*
        //高阻
    //P2
    P2M1 = 0xff;
    P2M0 = 0x00;
    
    //P1.5
    P1M1 |= 0x01<<5;
    P1M0 &= 0x01<<5;
    */
    
    EN1 = 1;    //enable wires
    EN2 = 1;
    
    AUXR &= ~(0<<7) ;//T0 12分频
    
        //按键
    Int0Init();
    
        //PWM模块
    pwm_init();
        //TCS模块
    TMOD=0x51;//??T0?????1??10??
    LCMInit();//LCD is WAITING so NO lcd stops the MCU!!!
    baipingheng();//?????????
        //初值
    tonecol = 7;//BETA
    
}


void EXitrpt0() interrupt 0
{//按键控制 扫描颜色
    celiang();
    tellcol();
    
    SSTOP = 1;//设置卡住
}
void EXitrpt1() interrupt 2
{
    MAINMODE++;
    MAINMODE &= 0x01;   //  <2
}
void PWMout(){// wav plays
    static u8 spdconv[8] = {
        200,150,120,100,
        80,70,60,50
    };
    wPWMplay(spdconv[tonecol]);
}

void wPWMplay(u8 speed){
    u16 tmp = 0;
    u16 i = 0;
    
        tmp = 0;
        while(tmp <= pcmlth){
            
            pwm_set(pcmdata[tmp], 255-pcmdata[tmp]); 
            for(i = speed; i; i--);//DELAYTIME
            
            tmp++;
            //tmp += 100;
        }    

} 




void Int0Init()
{
	
	IT0=1;//???????(???)
	EX0=1;//??INT0??????	
	IT1=1;//
	EX1=1;//	
	EA=1;//?????	
}



void tellcol(){
    u8 i;
    u8 CMPADD = 5; //不大于30，否则黑色减掉后溢出
#if 0
    static u8 cmpdat[8][3]={
        15,15,20,
        23,29,55,
        24,37,36,
        25,43,71,
        57,25,35,
        70,29,50,
        54,46,37,
        62,60,83
    };
#endif
    
#if 0
    static u8 cmpdat[8][3]={
        30,30,30,
        44,65,103,
        55,95,67,
        52,108,140,
        139,55,57,
        185,67,93,
        138,115,67,
        170,170,180
    };
#endif
    static u8 cmpdat[8][3]={
        6,6,8,
        14,18,38,
        14,24,22,
        14,27,46,
        36,14,19,
        42,15,28,
        36,30,24,
        36,36,50
    };
    
    
    
    //颜色关系对应表，
    //rb,gb,bb; RGB值
    //填入 全局变量 u8 tonecol 即可
    for(i=0;i<8;i++){
        if( (rb <= (cmpdat[i][0]+CMPADD)) && (rb >= (cmpdat[i][0]-CMPADD)) 
          &&(gb <= (cmpdat[i][1]+CMPADD)) && (gb >= (cmpdat[i][1]-CMPADD))
          &&(bb <= (cmpdat[i][2]+CMPADD)) && (bb >= (cmpdat[i][2]-CMPADD))
           )
        {
           tonecol = i;
        }
    }
    
    
    //显示在LCD上
       DisplayOneChar(0, 0,'R');//??????RGB??????
       DisplayOneChar(0, 1, rb/100+0x30); //??????
       DisplayOneChar(0, 2, rb/10%10+0x30);//??????
       DisplayOneChar(0, 3, rb%10+0x30);//??????
       DisplayOneChar(0, 5,'G');//??????RGB??????
	   
       DisplayOneChar(0, 6, gb/100+0x30); //??????
       DisplayOneChar(0, 7, gb/10%10+0x30);
       DisplayOneChar(0, 8, gb%10+0x30);
       DisplayOneChar(0, 10,'B');//??????RGB??????
       DisplayOneChar(0, 11, bb/100+0x30);
       DisplayOneChar(0, 12, bb/10%10+0x30);
       DisplayOneChar(0, 13, bb%10+0x30);
    
    
#if 0
       //*****?LCD1602?????16????RGB*******************
       DisplayOneChar(1, 1, tab1[rb/16]); 
       DisplayOneChar(1, 2, tab1[rb%16]);
       DisplayOneChar(1, 3, 'H');
       DisplayOneChar(1, 6, tab1[gb/16]); 
       DisplayOneChar(1, 7, tab1[rb%16]);
       DisplayOneChar(1, 8, 'H');
       DisplayOneChar(1, 11,tab1[bb/16]);
       DisplayOneChar(1, 12,tab1[bb%16]);
       DisplayOneChar(1, 13,'H');
#endif

    
}




void wavmain(u16 dtime, u16 ptime){
    u16 i,j;
    
    for(j=ptime;j;j--){     //连续脉冲
        BZ = ~BZ;
        for(i=dtime;i;i--){;}   //软件延时
    }  
    
}

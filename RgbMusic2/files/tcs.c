//#include <reg52.h>
#include "60s2.h"
#define uchar unsigned char
#define uint  unsigned int
//==============LCD1602�ӿ����ӷ���=====================
/*-----------------------------------------------------
       |DB0-----P0.0 | DB4-----P0.4 | RW-------P2.3    |
       |DB1-----P0.1 | DB5-----P0.5 | RS-------P2.4    |
       |DB2-----P0.2 | DB6-----P0.6 | E--------P2.2    |
       |DB3-----P0.3 | DB7-----P0.7 | 
    ---------------------------------------------------*/
//================================================*/              
#define LCM_Data     P0    //LCD1602���ݽӿ�
#define Busy         0x80   //���ڼ��LCM״̬���е�Busy��ʶ
sbit    LCM_RW     = P2^5;  //��д��������ˣ�LCD1602�ĵ����
sbit    LCM_RS     = P2^6;  //�Ĵ���ѡ������ˣ�LCD1602�ĵ��Ľ�
sbit    LCM_E      = P2^7;  //ʹ���ź������,LCD1602�ĵ�6��

//=================��ɫ����ģ������=====================
/*-----------------------------------------------------
       |EO-----GND
       |S0-----VCC | S2-----P2.0 | OUT-------P3.5 
       |S1-----VCC | S3-----P2.1 | 
  ---------------------------------------------------*/
sbit    tcs230_s2=P2^0;//TCS230 S2�ӵ�Ƭ��P2.0
sbit    tcs230_s3=P2^1;//TCS230 S3�ӵ�Ƭ��P2.1
sbit    tcs230_en=P3^0; //TCS230 EN(E0)��GND
//**************��������***************************************
void    WriteDataLCM		(uchar WDLCM);//LCDģ��д����
void    WriteCommandLCM	(uchar WCLCM,BuysC); //LCDģ��дָ��
uchar   ReadStatusLCM(void);//��LCDģ���æ��
void    DisplayOneChar(uchar X,uchar Y,uchar ASCII);//�ڵ�X+1�еĵ�Y+1λ����ʾһ���ַ�
void    LCMInit(void);//LCD��ʼ
void    DelayMs(uint Ms);//1MS��׼��ʱ����
void    baipingheng();//��ƽ���ӳ���
void    celiang();//ʵ����ɫ����
uint    ryz,gyz,byz;//�ֱ����ɫ���� ��ɫ���� ��ɫ����
uint    rb,gb,bb;//RGBֵ
uchar   tab1[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

//******************************************************
//��RGB
void   celiang()
{
     //*********��Rֵ************************************
     TH0=0x00;  //TH0=(65536-10000)/256;
  	 TL0=0x00;  //TL0=(65536-10000)/256;
     TH1=0;
     TL1=0;
     tcs230_s2=0;
     tcs230_s3=0;//ѡ���ɫ�˹���
     tcs230_en=0;
     TR0=1;//10���뿪ʼ��ʱ
     TR1=1;//��ʼ����
     while(TF0==0);//�ȴ���ʱ�����
     TF0=0;//�����ʱ��0�����־
     TR0=0;//�رն�ʱ0
     TR1=0;
     rb=(unsigned long)(TH1*256+TL1)*255/ryz;
     if(rb>255)rb=255;//�ж�RGBֵ�Ƿ�Ϸ�
     //********=255***��Bֵ**************************************
     TH0=0x00;  //TH0=(65536-10000)/256;
  	 TL0=0x00;  //TL0=(65536-10000)/256;
     TH1=0;
     TL1=0;
     tcs230_s2=0;
     tcs230_s3=1;//ѡ����ɫ�˹���
     TR0=1;//10���뿪ʼ��ʱ
     TR1=1;//��ʼ����
     while(TF0==0);//�ȴ���ʱ�����
     TF0=0;//�����ʱ��0�����־
     TR0=0;//�رն�ʱ0
     TR1=0;
     bb=(unsigned long)(TH1*256+TL1)*255/byz;
     if(bb>255)bb=255;//�ж�RGBֵ�Ƿ�Ϸ�     
     //***********��Gֵ**************************************   
     TH0=0x00;  //TH0=(65536-10000)/256;
  	 TL0=0x00;  //TL0=(65536-10000)/256;
     TH1=0;
     TL1=0;
     tcs230_s2=1;
     tcs230_s3=1;//ѡ����ɫ�˹���
     TR0=1;//10���뿪ʼ��ʱ
     TR1=1;//��ʼ����
     while(TF0==0);//�ȴ���ʱ�����
     TF0=0;//�����ʱ��0�����־
     TR0=0;//�رն�ʱ0
     TR1=0;
     tcs230_en=1;
     gb=(unsigned long)(TH1*256+TL1)*255/gyz;
     if(gb>255)gb=255;//�ж�RGBֵ�Ƿ�Ϸ�  
}
//******************************************************
//��ƽ���ӳ���
void    baipingheng()
{
     //**************��ȡ��ɫ����***********************
     TH0=0x00;  //TH0=(65536-10000)/256;
  	 TL0=0x00;  //TL0=(65536-10000)/256;
     TH1=0;
     TL1=0;
     tcs230_s2=0;
     tcs230_s3=0;//ѡ���ɫ�˹���
     tcs230_en=0;
     TR0=1;//10���뿪ʼ��ʱ
     TR1=1;//��ʼ����
     while(TF0==0);//�ȴ���ʱ�����
     TF0=0;//�����ʱ��0�����־
     TR0=0;//�رն�ʱ0
     TR1=0;
     ryz=255/(TH1*256+TL1);//��ʵ����ı�������Ӧ��Ϊ255/(TH1*256+TL1)
     //**************��ȡ��ɫ����***********************
     TH0=0x00;  //TH0=(65536-10000)/256;
  	 TL0=0x00;  //TL0=(65536-10000)/256;
     TH1=0;
     TL1=0;
     tcs230_s2=0;
     tcs230_s3=1;//ѡ����ɫ�˹���
     TR0=1;//10���뿪ʼ��ʱ
     TR1=1;//��ʼ����
     while(TF0==0);//�ȴ���ʱ�����
     TF0=0;//�����ʱ��0�����־
     TR0=0;//�رն�ʱ0
     TR1=0;
     byz=255/(TH1*256+TL1);//��ʵ����ı�������Ӧ��Ϊ255/(TH1*256+TL1)
     //**************���̺�ɫ����***********************
     TH0=0x00;  //TH0=(65536-10000)/256;
  	 TL0=0x00;  //TL0=(65536-10000)/256;
     TH1=0;
     TL1=0;
     tcs230_s2=1;
     tcs230_s3=1;//ѡ����ɫ�˹���
     TR0=1;//10���뿪ʼ��ʱ
     TR1=1;//��ʼ����
     while(TF0==0);//�ȴ���ʱ�����
     TF0=0;//�����ʱ��0�����־
     TR0=0;//�رն�ʱ0
     TR1=0;
     tcs230_en=1;
     gyz=255/(TH1*256+TL1);//��ʵ����ı�������Ӧ��Ϊ255/(TH1*256+TL1)
}
/*======================================================================
 LCM��ʼ��
======================================================================*/
void LCMInit(void) 
{
 LCM_Data = 0;
 WriteCommandLCM(0x38,0); //������ʾģʽ���ã������æ�ź�
 DelayMs(5);
 WriteCommandLCM(0x38,0);
 DelayMs(5);
 WriteCommandLCM(0x38,0);
 DelayMs(5);
 WriteCommandLCM(0x38,1); //��ʾģʽ����,��ʼҪ��ÿ�μ��æ�ź�
 WriteCommandLCM(0x08,1); //�ر���ʾ
 WriteCommandLCM(0x01,1); //��ʾ����
 WriteCommandLCM(0x06,1); // ��ʾ����ƶ�����
 WriteCommandLCM(0x0C,1); // ��ʾ�����������
 DelayMs(100);
}
//==============================LCD1602��ʾ�ӳ���================================================
// д���ݺ���: E =������ RS=1 RW=0
//======================================================================*/
void WriteDataLCM(uchar WDLCM)
{
 ReadStatusLCM(); //���æ
 LCM_Data = WDLCM;
 LCM_RS = 1;
 LCM_RW = 0;
 LCM_E = 0; //�������ٶ�̫�߿���������С����ʱ
 LCM_E = 0; //��ʱ
 LCM_E = 1;
}
/*====================================================================
  дָ���: E=������ RS=0 RW=0
======================================================================*/
void WriteCommandLCM(uchar WCLCM,BuysC) //BuysCΪ0ʱ����æ���
{
 if (BuysC) ReadStatusLCM(); //������Ҫ���æ
 LCM_Data = WCLCM;
 LCM_RS = 0;
 LCM_RW = 0;
 LCM_E = 0;
 LCM_E = 0;
 LCM_E = 1;
}
/*====================================================================
  ������д����֮ǰ������LCD������״̬:E=1 RS=0 RW=1;
  DB7: 0 LCD���������У�1 LCD������æ��
  ��״̬
======================================================================*/
uchar ReadStatusLCM(void)
{
 LCM_Data = 0xFF;
 LCM_RS = 0;
 LCM_RW = 1;
 LCM_E = 0;
 LCM_E = 0;
 LCM_E = 1;
 while (LCM_Data & Busy); //���æ�ź�  
 return(LCM_Data);
}
/*======================================================================
�� ��:     ��1602 ָ��λ����ʾһ���ַ�:��һ��λ��0~15,�ڶ���16~31
˵ ��:     �� X ��,�� y ��  ע��:�ַ������ܳ���16���ַ�
======================================================================*/
void DisplayOneChar( uchar X, uchar Y, uchar ASCII)
{
  X &= 0x1;
  Y &= 0xF; //����Y���ܴ���15��X���ܴ���1
  if (X) Y |= 0x40; //��Ҫ��ʾ�ڶ���ʱ��ַ��+0x40;
  Y |= 0x80; // ���ָ����
  WriteCommandLCM(Y, 0); //���ﲻ���æ�źţ����͵�ַ��
  WriteDataLCM(ASCII);
}
/*====================================================================
  �趨��ʱʱ��:x*1ms
====================================================================*/
void DelayMs(uint Ms)
{
  uint i,TempCyc;
  for(i=0;i<Ms;i++)
  {
    TempCyc = 250;
    while(TempCyc--);
  }
}
	 
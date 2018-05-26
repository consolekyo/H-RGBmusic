#ifndef PWM_H
#define PWM_H

void pwm_init(void); //PWM初始化函数，初始占空比为0
void pwm_set(unsigned char x, unsigned char y); 
        //占空比设置函数,  x:P1^3 y:P1^4, 占 x/256

#endif

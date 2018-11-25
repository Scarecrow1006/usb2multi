#ifndef __PWM_H__
#define __PWM_H__

void initPWM(void);

void initClocks(void);
void setupPLL0(void);
void feedSeq(void);
void connectPLL0(void);
extern void PWM_func(unsigned char value);

#endif
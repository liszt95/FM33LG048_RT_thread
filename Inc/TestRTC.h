#ifndef __TESTRTC_H__
#define __TESTRTC_H__

#include "main.h"

void Test_RTCB(void);
void RTCB_Init(void);
uint8_t RTC_GetRTCB(FL_RTCB_InitTypeDef *InitStructer);
uint8_t RTC_SetRTCB(FL_RTCB_InitTypeDef *InitStructer);



#endif

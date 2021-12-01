#include "user_init.h"
#include "TestRTC.h"

void RTCB_Init(void)
{
    FL_RTCB_InitTypeDef      TempTime;

    TempTime.year   = 0x20;
    TempTime.month  = 0x02;
    TempTime.day    = 0x21;
    TempTime.week   = 0x04;
    TempTime.hour   = 0x09;
    TempTime.minute = 0x00;
    TempTime.second = 0x00;
    FL_RTCB_Init(RTCB, &TempTime);
    FL_RTCB_Enable(RTCB);     //RTCB使能
    NVIC_DisableIRQ(RTCx_IRQn);                //NVIC中断控制器配置
}

uint8_t RTC_GetRTCB(FL_RTCB_InitTypeDef *InitStructer)
{
    uint8_t n, i;
    uint8_t Result = 1;

    FL_RTCB_InitTypeDef TempTime1, TempTime2;

    for(n = 0 ; n < 3; n++)
    {
        FL_RTCB_GetTime(RTCB, &TempTime1); //读一次时间
        FL_RTCB_GetTime(RTCB, &TempTime2); //再读一次时间

        for(i = 0; i < 7; i++) //两者一致, 表示读取成功
        {
            if(((uint32_t *)(&TempTime1))[i] != ((uint32_t *)(&TempTime2))[i]) { break; }
        }

        if(i == 7)
        {
            Result = 0;
            memcpy((uint32_t *)(InitStructer), (uint32_t *)(&TempTime1), 7 * sizeof(uint32_t)); //读取正确则更新新的时间
            break;
        }
    }

    return Result;
}

uint8_t RTC_SetRTCB(FL_RTCB_InitTypeDef *InitStructer)
{
    uint8_t n, i;
    uint8_t Result;
    FL_RTCB_InitTypeDef TempTime1;

    for(n = 0 ; n < 3; n++)
    {
        FL_RTCB_ConfigTime(RTCB, InitStructer);
        Result = RTC_GetRTCB(&TempTime1);    //读取确认设置结果

        if(Result == 0)
        {
            Result = 1;

            for(i = 0; i < 7; i++) //两者一致, 表示设置成功
            {
                if(((uint32_t *)(&TempTime1))[i] != ((uint32_t *)(InitStructer))[i])
                { break; }
            }

            if(i == 7)
            {
                Result = 0;
                break;
            }
        }
    }

    return Result;
}

/*void Test_RTCB(void)
{
    FL_RTCB_InitTypeDef      InitTime;
    FL_RTCB_InitTypeDef      GetTime;

    RTCB_Init();
    InitTime.year   = 0x20;
    InitTime.month  = 0x02;
    InitTime.day    = 0x21;
    InitTime.week   = 0x04;
    InitTime.hour   = 0x15;
    InitTime.minute = 0x00;
    InitTime.second = 0x00;



    RTC_SetRTCB(&InitTime);
    FL_DelayMs(1000);
    RTC_GetRTCB(&GetTime);

}*/


#include "main.h"
#include "user_init.h"
#include "TestRTC.h"
#include "rtthread.h"

static void led1_task_entry(void *parameter);
static void led2_task_entry(void *parameter);
static void RTC_task_entry(void *parameter);

/* 私有变量 ------------------------------------------------------------------*/
static rt_thread_t led1_Thread = RT_NULL;
static rt_thread_t led2_Thread = RT_NULL;






int main(void)
{
    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    /* SHOULD BE KEPT!!! */
    MF_Clock_Init();

    /* Configure the system clock */
    /* SHOULD BE KEPT!!! */
    MF_SystemClock_Config();

    /* Initialize FL Driver Library */
    /* SHOULD BE KEPT!!! */
    FL_Init();

    /* Initialize all configured peripherals */
    /* SHOULD BE KEPT!!! */
    MF_Config_Init();

    UserInit();
    FoutInit();


    led1_Thread =                          /* 线程控制块指针 */
        rt_thread_create( "LED1",              /* 线程名字 */
                          led1_task_entry, /* 线程入口函数 */
                          RT_NULL,             /* 线程入口函数参数 */
                          128,                 /* 线程栈大小 */
                          2,                   /* 线程的优先级 */
                          20);                 /* 线程时间片 */

    /* 启动线程，开启调度 */
    if (led1_Thread != RT_NULL)
        rt_thread_startup(led1_Thread);
    else
        return -1;

    led2_Thread =                          /* 线程控制块指针 */
        rt_thread_create( "LED2",              /* 线程名字 */
                          led2_task_entry, /* 线程入口函数 */
                          RT_NULL,             /* 线程入口函数参数 */
                          128,                 /* 线程栈大小 */
                          3,                   /* 线程的优先级 */
                          20);                 /* 线程时间片 */

    /* 启动线程，开启调度 */
    if (led2_Thread != RT_NULL)
        rt_thread_startup(led2_Thread);
    else
        return -1;



}


static void led1_task_entry(void *parameter)
{

    while (1)
    {

        LED0_TOG();
        rt_thread_mdelay(200);
    }


}



static void led2_task_entry(void *parameter)
{

    while (1)
    {

        LED1_TOG();
        rt_thread_mdelay(200);
    }


}


static void RTC_task_entry(void *parameter)
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
    rt_thread_mdelay(1000);
    RTC_GetRTCB(&GetTime);





}












////////////////////////////////////////////////////////////////////////////////////////
//支持控制台
void rt_hw_console_output(const char *str)
{


    //  LPUART_Init(LPUART0);
    rt_size_t i = 0, size = 0;
    char a = '\r';

    rt_enter_critical();

    size = rt_strlen(str);

    for (i = 0; i < size; i++)
    {
        if (*(str + i) == '\n')
        {
            FL_LPUART_WriteTXBuff(LPUART0, a); //将发送数据写入发送寄存器

            while(FL_LPUART_IsActiveFlag_TXShiftBuffEmpty(LPUART0) == 0); //等待移位空发送完成

            FL_LPUART_ClearFlag_TXShiftBuffEmpty(LPUART0);
        }

        rt_exit_critical();
    }

}

char rt_hw_console_getchar(void)
{

    //  LPUART_Init(LPUART0);
    int ch = -1;

    if(FL_LPUART_ReadMatchData(LPUART0))              //接收到数据
    {
        FL_LPUART_ClearFlag_RXBuffFull(LPUART0);

        ch = FL_LPUART_ReadRXBuff(LPUART0);        //LPUART接收数据

        while(FL_LPUART_IsActiveFlag_TXBuffEmpty(LPUART0) == FL_RESET)//等待缓冲器空

            FL_LPUART_WriteTXBuff(LPUART0, ch); //将发送数据写入发送寄存器
    }

    return ch;
}





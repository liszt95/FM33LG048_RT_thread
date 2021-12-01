#include "main.h"
#include "user_init.h"
#include "TestRTC.h"
#include "rtthread.h"

static void led1_task_entry(void *parameter);
static void led2_task_entry(void *parameter);
static void RTC_task_entry(void *parameter);

/* ˽�б��� ------------------------------------------------------------------*/
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


    led1_Thread =                          /* �߳̿��ƿ�ָ�� */
        rt_thread_create( "LED1",              /* �߳����� */
                          led1_task_entry, /* �߳���ں��� */
                          RT_NULL,             /* �߳���ں������� */
                          128,                 /* �߳�ջ��С */
                          2,                   /* �̵߳����ȼ� */
                          20);                 /* �߳�ʱ��Ƭ */

    /* �����̣߳��������� */
    if (led1_Thread != RT_NULL)
        rt_thread_startup(led1_Thread);
    else
        return -1;

    led2_Thread =                          /* �߳̿��ƿ�ָ�� */
        rt_thread_create( "LED2",              /* �߳����� */
                          led2_task_entry, /* �߳���ں��� */
                          RT_NULL,             /* �߳���ں������� */
                          128,                 /* �߳�ջ��С */
                          3,                   /* �̵߳����ȼ� */
                          20);                 /* �߳�ʱ��Ƭ */

    /* �����̣߳��������� */
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
//֧�ֿ���̨
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
            FL_LPUART_WriteTXBuff(LPUART0, a); //����������д�뷢�ͼĴ���

            while(FL_LPUART_IsActiveFlag_TXShiftBuffEmpty(LPUART0) == 0); //�ȴ���λ�շ������

            FL_LPUART_ClearFlag_TXShiftBuffEmpty(LPUART0);
        }

        rt_exit_critical();
    }

}

char rt_hw_console_getchar(void)
{

    //  LPUART_Init(LPUART0);
    int ch = -1;

    if(FL_LPUART_ReadMatchData(LPUART0))              //���յ�����
    {
        FL_LPUART_ClearFlag_RXBuffFull(LPUART0);

        ch = FL_LPUART_ReadRXBuff(LPUART0);        //LPUART��������

        while(FL_LPUART_IsActiveFlag_TXBuffEmpty(LPUART0) == FL_RESET)//�ȴ���������

            FL_LPUART_WriteTXBuff(LPUART0, ch); //����������д�뷢�ͼĴ���
    }

    return ch;
}





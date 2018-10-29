/*********************************************************************************************************************

 *
 * @file       		main
 * @author     		Alex
 * @version    		v1.0
 * @Software 		IAR 8.1
 * @Target core		MK60DNZ  MK60DN MK60FX/FN MK66FX/MK66FN
 * @date       		2017-11-9
 ********************************************************************************************************************/
#include "include.h"
#include "isr.h"
#include "Motor.h"
#include "Steer.h"
#include "Camera.h"
#include "UI.h"
#include "MK60_uart.h"
#include "KeyBoardDriver.h"
#include "DataStore.h"
/*����ͷ�ӿ�   SCL   A26 
               SDA  A25
               DATA  B0-7
               PCLK   A27
               VSY    A29
               HREF   A28
*/
///////�������   300HZ     �ӿ�ΪA12
///////����ӿ�   20000HZ   �ӿ�Ϊ  C1  C2

extern uint8 ImageBuff[600];
uint8 ErrLoop = 0;

void main()
{
    uart_init(uart0,38400);
    //ImageSensor_init(ImageBuff ,sizeof(ImageBuff)); ///ͼ���ʼ��
    Common_delay(10);
    OLED_init();
    OLED_PrintInt(0,0,8,5,0);

    IIC_init();                 // ʹ�����IIC
    while(BMX055_init() == false)
    {
        ErrLoop++;
        if(ErrLoop == 5)
            NVIC_SystemReset();                         //////�������5��ʧ����ֱ������
    };           ////BMX055����
    /*
    key_init();
    Data_init();
    ftm_pwm_init(ftm2, ftm_ch0, 300, SteerCenter);         /////�����ʼ��  ����Ϊ1000��500Ϊ50%ռ�ձ�    300HZ
    ftm_pwm_init(ftm0, ftm_ch4, 20000, 0);        //////�����ʼ��
    ftm_pwm_init(ftm0, ftm_ch5, 20000, 0);
    ftm_quad_init(ftm1);
    PIT_IRQ_init(PIT0, 5, PIT0_IRQHandler);
    PIT_IRQ_init(PIT1, 5, PIT1_IRQHandler); //5ms�жϣ����ڰ���ɨ��
    NVIC_EnableIRQ(PIT0_IRQn);
    NVIC_EnableIRQ(PIT1_IRQn);
    OS_menu();
    EnableInterrupts;*/
    while(1)
    {

      printf("hehehehe");
    }
}

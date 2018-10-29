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
/*摄像头接口   SCL   A26 
               SDA  A25
               DATA  B0-7
               PCLK   A27
               VSY    A29
               HREF   A28
*/
///////舵机测试   300HZ     接口为A12
///////电机接口   20000HZ   接口为  C1  C2

extern uint8 ImageBuff[600];
uint8 ErrLoop = 0;

void main()
{
    uart_init(uart0,38400);
    //ImageSensor_init(ImageBuff ,sizeof(ImageBuff)); ///图像初始化
    Common_delay(10);
    OLED_init();
    OLED_PrintInt(0,0,8,5,0);

    IIC_init();                 // 使用软件IIC
    while(BMX055_init() == false)
    {
        ErrLoop++;
        if(ErrLoop == 5)
            NVIC_SystemReset();                         //////如果超出5次失败则直接重启
    };           ////BMX055配置
    /*
    key_init();
    Data_init();
    ftm_pwm_init(ftm2, ftm_ch0, 300, SteerCenter);         /////舵机初始化  精度为1000则500为50%占空比    300HZ
    ftm_pwm_init(ftm0, ftm_ch4, 20000, 0);        //////电机初始化
    ftm_pwm_init(ftm0, ftm_ch5, 20000, 0);
    ftm_quad_init(ftm1);
    PIT_IRQ_init(PIT0, 5, PIT0_IRQHandler);
    PIT_IRQ_init(PIT1, 5, PIT1_IRQHandler); //5ms中断，用于按键扫描
    NVIC_EnableIRQ(PIT0_IRQn);
    NVIC_EnableIRQ(PIT1_IRQn);
    OS_menu();
    EnableInterrupts;*/
    while(1)
    {

      printf("hehehehe");
    }
}

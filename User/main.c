#include "stdint.h"
#include "stm32f10x.h" // Device header
#include "Common_Debug.h"
#include "App_comm.h"
#include "App_sampling.h"

int main()
{
	// Common_Debug_Init();
	debug_init();

	int a = 10;

	// printf("心电监测项目 %d\r\n", a);
	debug_printf("心电监测项目 %d\r\n", a);
	debug_printfln("心电监测项目 %d", a);

	/* 1.启动通信模块 */
	App_Comm_Start();
	debug_printfln("通信模块已经启动");

	/* 2.处理用户指令 */
	debug_printfln("请下发指令：采样率(\"s1000\\n\") 采样时长(\"c60\\n\")");
	uint16_t sampling_rate, duration;
	App_Comm_CommandProcess(&sampling_rate, &duration);
	debug_printfln("采样率=%d, 采样时间=%d", sampling_rate, duration);

	/* 3.启动采集模块 */	
	App_Sampling_Start(sampling_rate, duration);


	while (1)
	{
		/* 4.采样数据 */
		uint16_t data = App_Sampling_ReadVal();
		debug_printfln("data = %d", data);

		App_Comm_SendDataToUpper(data);
	}

	return 0;
}




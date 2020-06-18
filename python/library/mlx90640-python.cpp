#include <stdint.h>
#include <iostream>
#include <cstring>
#include <fstream>
#include <chrono>
#include <thread>
#include <math.h>
#include "MLX90640/MLX90640_API.h"
#include "MLX90640/MLX90640_I2C_Driver.h"

// Despite the framerate being ostensibly FPS hz
// The frame is often not ready in time
// This offset is added to the frame time
// to account for this.

paramsMLX90640 mlx90640;
uint16_t frame[834];
int mlx_addr;

extern "C" 
{
int setup(int addr, int fps){
	static uint16_t eeMLX90640[832];
	mlx_addr = addr;
#ifdef DEBUG
	printf("Setting device mode\n");
#endif	
	MLX90640_SetDeviceMode(mlx_addr, 0);
#ifdef DEBUG
	printf("Set sub page repeat\n");
#endif	
	MLX90640_SetSubPageRepeat(mlx_addr, 0);

	switch(fps){
		case 1:
			// baudrate = 400000;
			MLX90640_SetRefreshRate(mlx_addr, 0b001);
			break;
		case 2:
			// baudrate = 400000;
			MLX90640_SetRefreshRate(mlx_addr, 0b010);
			break;
		case 4:
			// baudrate = 400000;
			MLX90640_SetRefreshRate(mlx_addr, 0b011);
			break;
		case 8:
			// baudrate = 400000;
			MLX90640_SetRefreshRate(mlx_addr, 0b100);
			break;
		case 16:
			// baudrate = 1000000;
			MLX90640_SetRefreshRate(mlx_addr, 0b101);
			break;
		case 32:
			// baudrate = 1000000;
			MLX90640_SetRefreshRate(mlx_addr, 0b110);
			break;
		case 64:
			// baudrate = 1000000;
			MLX90640_SetRefreshRate(mlx_addr, 0b111);
			break;
		default:
#ifdef DEBUG
			printf("Unsupported framerate: %d", fps);
#endif
			return 1;
	}
#ifdef DEBUG
	printf("Setting Chess Mode\n");
#endif
	MLX90640_SetChessMode(mlx_addr);
#ifdef DEBUG
	printf("Dump EE\n");
#endif
	MLX90640_DumpEE(mlx_addr, eeMLX90640);
#ifdef DEBUG
	printf("Extract Parameters\n");
#endif
	MLX90640_ExtractParameters(eeMLX90640, &mlx90640);

	return 0;
}

float * get_frame(void){
	int retries = 10;
	int subpage;
	bool subpages[2] = {0,0};
	float emissivity = 1;
	float eTa;
	static float mlx90640To[768];

	while (retries-- && (!subpages[0] || !subpages[1])){
#ifdef DEBUG
		printf("Retries: %d \n", retries);
#endif

		MLX90640_GetFrameData(mlx_addr, frame);
#ifdef DEBUG
		printf("Got data for page %d\n", MLX90640_GetSubPageNumber(frame));
#endif
		subpage = MLX90640_GetSubPageNumber(frame);

		subpages[subpage] = 1;

#ifdef DEBUG
		printf("Converting data for page %d\n", subpage);
#endif

		eTa = MLX90640_GetTa(frame, &mlx90640);
		MLX90640_CalculateTo(frame, &mlx90640, emissivity, eTa, mlx90640To);

		MLX90640_BadPixelsCorrection((&mlx90640)->brokenPixels, mlx90640To, 1, &mlx90640);
		MLX90640_BadPixelsCorrection((&mlx90640)->outlierPixels, mlx90640To, 1, &mlx90640);
	}
#ifdef DEBUG
	printf("Finishing\n");
#endif

	return mlx90640To;
}

uint16_t * get_raw(void)
{
	int retries = 10;
	int subpage;
	bool subpages[2] = {0,0};

	while (retries-- && (!subpages[0] || !subpages[1])){
#ifdef DEBUG
		printf("Retries: %d \n", retries);
#endif

		MLX90640_GetFrameData(mlx_addr, frame);
#ifdef DEBUG
		printf("Got data for page %d\n", MLX90640_GetSubPageNumber(frame));
#endif
		subpage = MLX90640_GetSubPageNumber(frame);

		subpages[subpage] = 1;
	}

	return frame;
}
} // extern "C"

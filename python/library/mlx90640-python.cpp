#include <stdint.h>
#include <iostream>
#include <cstring>
#include <fstream>
#include <chrono>
#include <thread>
#include <math.h>
#include "MLX90640/MLX90640_API.h"
#include "MLX90640/MLX90640_I2C_Driver.h"

#define MLX_I2C_ADDR 0x33

// Despite the framerate being ostensibly FPS hz
// The frame is often not ready in time
// This offset is added to the frame time
// to account for this.

paramsMLX90640 mlx90640;
uint16_t frame[834];

extern "C" 
{
int setup(int fps){
	static uint16_t eeMLX90640[832];
	
	MLX90640_SetDeviceMode(MLX_I2C_ADDR, 0);
	MLX90640_SetSubPageRepeat(MLX_I2C_ADDR, 0);

	switch(fps){
		case 1:
			// baudrate = 400000;
			MLX90640_SetRefreshRate(MLX_I2C_ADDR, 0b001);
			break;
		case 2:
			// baudrate = 400000;
			MLX90640_SetRefreshRate(MLX_I2C_ADDR, 0b010);
			break;
		case 4:
			// baudrate = 400000;
			MLX90640_SetRefreshRate(MLX_I2C_ADDR, 0b011);
			break;
		case 8:
			// baudrate = 400000;
			MLX90640_SetRefreshRate(MLX_I2C_ADDR, 0b100);
			break;
		case 16:
			// baudrate = 1000000;
			MLX90640_SetRefreshRate(MLX_I2C_ADDR, 0b101);
			break;
		case 32:
			// baudrate = 1000000;
			MLX90640_SetRefreshRate(MLX_I2C_ADDR, 0b110);
			break;
		case 64:
			// baudrate = 1000000;
			MLX90640_SetRefreshRate(MLX_I2C_ADDR, 0b111);
			break;
		default:
#ifdef DEBUG
			printf("Unsupported framerate: %d", fps);
#endif
			return 1;
	}
	MLX90640_SetChessMode(MLX_I2C_ADDR);
	MLX90640_DumpEE(MLX_I2C_ADDR, eeMLX90640);
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

		MLX90640_GetFrameData(MLX_I2C_ADDR, frame);
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

		MLX90640_GetFrameData(MLX_I2C_ADDR, frame);
#ifdef DEBUG
		printf("Got data for page %d\n", MLX90640_GetSubPageNumber(frame));
#endif
		subpage = MLX90640_GetSubPageNumber(frame);

		subpages[subpage] = 1;
	}

	printf("frame[100] : %d \n", frame[100]); 
	printf("frame[200] : %d \n", frame[200]); 
	printf("frame[300] : %d \n", frame[300]); 
	return frame;
}
} // extern "C"

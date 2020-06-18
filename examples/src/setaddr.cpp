#include <stdint.h>
#include <fstream>
#include "headers/MLX90640_I2C_Driver.h"

#define MLX_I2C_ADDR 0xbe33
#define NEW_MLX_I2C_ADDR 0xbe44

int main(int argc, char** argv)
{
	int resp;
	uint16_t data;

	MLX90640_I2CRead(MLX_I2C_ADDR, 0x240F, 1, &data);
	printf("Address: %x\n", data);

	printf("Giving new address %x\n", NEW_MLX_I2C_ADDR);
	resp = MLX90640_I2CWrite(MLX_I2C_ADDR, 0x240F, NEW_MLX_I2C_ADDR);
	MLX90640_I2CRead(MLX_I2C_ADDR, 0x240F, 1, &data);
	printf("Response from writing: %d\n", resp);
	printf("Address: %x\n", data);

	return 0;
}

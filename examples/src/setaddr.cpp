#include <stdint.h>
#include <fstream>
#include <unistd.h>
#include "headers/MLX90640_I2C_Driver.h"

#define MLX_I2C_ADDR 0xbe33
#define NEW_MLX_I2C_ADDR 0xbe44

int main(int argc, char** argv)
{
	int resp;
	uint16_t data;
	uint16_t old_addr;
	uint16_t new_addr;


	old_addr = strtol(argv[1], NULL, 16);
	old_addr = (0xbe << 8) | old_addr;

	new_addr = strtol(argv[2], NULL, 16);
	new_addr = (0xbe << 8) | new_addr;

	MLX90640_I2CRead(old_addr, 0x240F, 1, &data);
	printf("Address: %x\n", data);

	printf("Blanking out old address\n");
	resp = MLX90640_I2CWrite(old_addr, 0x240F, 0);
	sleep(10);

	printf("Giving new address %x\n", new_addr);
	resp = MLX90640_I2CWrite(old_addr, 0x240F, new_addr);
	MLX90640_I2CRead(old_addr, 0x240F, 1, &data);
	printf("Response from writing: %d\n", resp);
	printf("Address: %x\n", data);

	return 0;
}

/*
 * mcp23017.h
 *
 *  Created on: Mar 19, 2024
 *      Author: lando
 */

#ifndef INC_MCP23017_H_
#define INC_MCP23017_H_


//INCLUDES
#include "stm32f4xx_hal.h"
#include "ws2812b.h"


//PORTS
#define MCP23017_PORTA			0x00
#define MCP23017_PORTB			0x01

//Registers
#define REGISTER_IODIRA		0x00
#define REGISTER_IODIRB		0x01


#define REGISTER_GPIOA		0x12
#define REGISTER_GPIOB		0x13

//ENABLES
// I/O Direction
// Default state: MCP23017_IODIR_ALL_INPUT
#define MCP23017_IODIR_ALL_OUTPUT	0x00
#define MCP23017_IODIR_ALL_INPUT	0xFF

 //MCP23017 SLAVE DEVICE ADDRESS
#define MCP23017_ADDRESS_20		0x20
#define MCP23017_ADDRESS_21		0x21
#define MCP23017_ADDRESS_22		0x22
#define MCP23017_ADDRESS_23		0x23
#define MCP23017_ADDRESS_24		0x24
#define MCP23017_ADDRESS_25		0x25
#define MCP23017_ADDRESS_26		0x26
#define MCP23017_ADDRESS_27		0x27



#define I2C_TIMEOUT		100



//DATA STRUCTS
typedef struct {
 	I2C_HandleTypeDef*	hi2c;
 	uint16_t		addr;
 	uint8_t			gpio[2];
 } MCP23017_HandleTypeDef;


 //Functions

 void mcp23017_init(MCP23017_HandleTypeDef *hdev, I2C_HandleTypeDef *hi2c, uint16_t addr);

HAL_StatusTypeDef mcp23017_read(MCP23017_HandleTypeDef *hdev, uint16_t reg, uint8_t *data);

HAL_StatusTypeDef mcp23017_write(MCP23017_HandleTypeDef *hdev, uint16_t reg, uint8_t *data);

 HAL_StatusTypeDef mcp23017_iodir(MCP23017_HandleTypeDef *hdev, uint8_t port, uint8_t iodir);

 HAL_StatusTypeDef mcp23017_read_gpio(MCP23017_HandleTypeDef *hdev, uint8_t port);




 void ledHallPCBSystemCheck(MCP23017_HandleTypeDef hmcp,TIM_HandleTypeDef* htim, int channelNum, int boardNum);


 void mcp23017_init_array(MCP23017_HandleTypeDef **hmcps, int numExpanders, I2C_HandleTypeDef hi2c1);

 bool checkHallSensor(int row, int column, MCP23017_HandleTypeDef hmcps1[8], MCP23017_HandleTypeDef hmcps2[8]);


#endif /* INC_MCP23017_H_ */

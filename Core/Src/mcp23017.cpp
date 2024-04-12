
#include "mcp23017.h"


void mcp23017_init(MCP23017_HandleTypeDef *hdev, I2C_HandleTypeDef *hi2c, uint16_t addr)
 {
 	hdev->hi2c = hi2c;
 	hdev->addr = addr << 1;
 }

HAL_StatusTypeDef mcp23017_read(MCP23017_HandleTypeDef *hdev, uint16_t reg, uint8_t *data)
{
	return HAL_I2C_Mem_Read(hdev->hi2c, hdev->addr, reg, 1, data, 1, I2C_TIMEOUT);
}

HAL_StatusTypeDef mcp23017_write(MCP23017_HandleTypeDef *hdev, uint16_t reg, uint8_t *data)
{
	return HAL_I2C_Mem_Write(hdev->hi2c, hdev->addr, reg, 1, data, 1, I2C_TIMEOUT);
}

 HAL_StatusTypeDef mcp23017_iodir(MCP23017_HandleTypeDef *hdev, uint8_t port, uint8_t iodir)
 {
 	uint8_t data[1] = {iodir};
 	return mcp23017_write(hdev, REGISTER_IODIRA|port, data);
 }

 HAL_StatusTypeDef mcp23017_read_gpio(MCP23017_HandleTypeDef *hdev, uint8_t port)
 {
 	uint8_t data[1];
 	HAL_StatusTypeDef status;
 	status = mcp23017_read(hdev, REGISTER_GPIOA|port, data);
 	if (status == HAL_OK)
 		hdev->gpio[port] = data[0];
 	return status;
 }




 void ledHallPCBSystemCheck(MCP23017_HandleTypeDef hmcp,TIM_HandleTypeDef* htim, int channelNum, int boardNum){
  	//int size = numExpanders;
  	//for(int board = 0; board < 1; board++){
 	mcp23017_read_gpio(&hmcp, MCP23017_PORTA);
 	mcp23017_read_gpio(&hmcp, MCP23017_PORTB);
 	int addOn = 0;
 	if(boardNum % 2 == 1){
 		addOn = 16;
 	}

 	for(int port=0; port<2; port++){ //port used in place of MCP23017_PORTA and MCP23017_PORTB- corresponding to 0x00 and 0x01
 		for (int i=0; i<8; i++){
 			int mask = 1 << i;
 			if((hmcp.gpio[port] & mask) != 0){
 				Set_LED(i + addOn + (8*port),255,0,0);
 			}
 			else{
 				Set_LED(i + addOn + (8*port),0,250,0);
 			}
 		}
 	}

  	//}
  	Set_Brightness(20);
  	WS2812_Send(htim,channelNum);

  }


 void mcp23017_init_array(MCP23017_HandleTypeDef **hmcps, int numExpanders, I2C_HandleTypeDef hi2c1){
	 unsigned int mcpAddress[] = {MCP23017_ADDRESS_20, MCP23017_ADDRESS_21,MCP23017_ADDRESS_22,MCP23017_ADDRESS_23,MCP23017_ADDRESS_24,MCP23017_ADDRESS_25,MCP23017_ADDRESS_26,MCP23017_ADDRESS_27};

	 for(int i = 0; i < numExpanders; i++){
		 mcp23017_init(&((*hmcps)[i]), &hi2c1, mcpAddress[i]);
		 mcp23017_iodir(&((*hmcps)[i]), MCP23017_PORTA, MCP23017_IODIR_ALL_INPUT);
		 mcp23017_iodir(&((*hmcps)[i]), MCP23017_PORTB, MCP23017_IODIR_ALL_INPUT);
	 }
 }


 bool checkHallSensor(int row, int col, MCP23017_HandleTypeDef hmcps1[8], MCP23017_HandleTypeDef hmcps2[8]){
	 int half = row / 8; //top or bottom half of board
	 int hcmpInd = row % 8; // index of expander in each group of 8

	 MCP23017_HandleTypeDef hmcp;
	 if(half == 0){
		 hmcp = hmcps1[hcmpInd];
	 }
	 else{
		 hmcp = hmcps2[hcmpInd];
	 }


	 mcp23017_read_gpio(&hmcp, MCP23017_PORTA);
	 mcp23017_read_gpio(&hmcp, MCP23017_PORTB);

	 int mask = 1 << (col % 8);
	 return ((hmcp.gpio[col/8] & mask) == 0); //returns true if hall is triggered by magnet
 }

#include "stm32f4xx_hal.h"
#include "keypad.h"
#include "main.h"
#include "lcd.h"

char keys[ROWS][COLS] = {
	{'1', '2', '3', 'A'},
	{'4', '5', '6', 'B'},
	{'7', '8', '9', 'C'},
	{'*', '0', '#', 'D'}
};

char Get_Key(void) {
	for (int i = 0; i < COLS; i++) {
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0 << i, GPIO_PIN_SET);
		HAL_Delay(5);
		for (int j = 0; j < ROWS; j++) {
			if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_4 << j)) {
				return keys[j][i];
			}
		}
	}
	return '\0';
}

void Keypad_Test(void) {
	char key = Get_Key();
	if (key == '1' || key == '2' || key == '3' || key == 'A') {
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_2, GPIO_PIN_SET);
	}
	else if (key == '4' || key == '5' || key == '6' || key == 'B') {
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_SET);
	}
	else if (key == '7' || key == '8' || key == '9' || key == 'C') {
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_SET);
	}
	else if (key == '*' || key == '0' || key == '#' || key == 'D') {
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_SET);
	}
	else {
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5, GPIO_PIN_RESET);
	}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_pin)
{
	current_mill = HAL_GetTick();
	if (current_mill - previous_mill > 10) {
		GPIO_init_struct_private.Pin = GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
		GPIO_init_struct_private.Mode = GPIO_MODE_INPUT;
		GPIO_init_struct_private.Pull = GPIO_NOPULL;
		GPIO_init_struct_private.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(GPIOD, &GPIO_init_struct_private);

		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, 1);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, 0);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, 0);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, 0);
		if(GPIO_pin == GPIO_PIN_4 && HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_4)) {
			key = '1';
		}
		else if(GPIO_pin == GPIO_PIN_5 && HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_5)) {
			key = '4';
		}
		else if(GPIO_pin == GPIO_PIN_6 && HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_6)) {
			key = '7';
		}
		else if(GPIO_pin == GPIO_PIN_7 && HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_7)) {
			key = '*';
		}

		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, 0);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, 1);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, 0);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, 0);
		if(GPIO_pin == GPIO_PIN_4 && HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_4)) {
			key = '2';
		}
		else if(GPIO_pin == GPIO_PIN_5 && HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_5)) {
			key = '3';
		}
		else if(GPIO_pin == GPIO_PIN_6 && HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_6)) {
			key = '8';
		}
		else if(GPIO_pin == GPIO_PIN_7 && HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_7)) {
			key = '0';
		}

		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, 0);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, 0);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, 1);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, 0);
		if(GPIO_pin == GPIO_PIN_4 && HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_4)) {
			key = '3';
		}
		else if(GPIO_pin == GPIO_PIN_5 && HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_5)) {
			key = '6';
		}
		else if(GPIO_pin == GPIO_PIN_6 && HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_6)) {
			key = '9';
		}
		else if(GPIO_pin == GPIO_PIN_7 && HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_7)) {
			key = '#';
		}

		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, 0);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, 0);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, 0);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, 1);
		if(GPIO_pin == GPIO_PIN_4 && HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_4)) {
			key = 'A';
		}
		else if(GPIO_pin == GPIO_PIN_5 && HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_5)) {
			key = 'B';
		}
		else if(GPIO_pin == GPIO_PIN_6 && HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_6)) {
			key = 'C';
		}
		else if(GPIO_pin == GPIO_PIN_7 && HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_7)) {
			key = 'D';
		}

		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, 1);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, 1);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, 1);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, 1);
		GPIO_init_struct_private.Mode = GPIO_MODE_IT_RISING;
		GPIO_init_struct_private.Pull = GPIO_PULLDOWN;
		HAL_GPIO_Init(GPIOD, &GPIO_init_struct_private);
		previous_mill = current_mill;
	}
}

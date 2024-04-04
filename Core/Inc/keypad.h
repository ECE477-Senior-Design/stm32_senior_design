//GPIO Pins
//PD4-7: GPIO_EXTI | PD0-3: GPIO_OUTPUT
//NVIC Settings
//EXTI line4 interrupt: Enabled
//EXTI line[9:5] interrupts: Enabled

#ifndef INC_KEYPAD_H_
#define INC_KEYPAD_H_

#define ROWS 4
#define COLS 4

#define MAX_STRING 4

extern char keys[ROWS][COLS];

extern GPIO_InitTypeDef GPIO_init_struct_private;
extern uint32_t current_mill;
extern uint32_t previous_mill;
extern char key;

void Keypad_Test(void);
char Get_Key(void);
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_pin);

#endif

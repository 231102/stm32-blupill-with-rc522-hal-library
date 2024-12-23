#ifndef __LCD_H
#define __LCD_H

#include "main.h"

/* LCD Commands */
#define LCD_CLEAR 0x01         // Clear display
#define LCD_RETURN_HOME 0x02   // Return home
#define LCD_ENTRY_MODE 0x06    // Entry mode set
#define LCD_DISPLAY_ON 0x0C    // Display ON, Cursor OFF
#define LCD_FUNCTION_SET 0x28  // 4-bit mode, 2 lines, 5x8 font

/* Function Prototypes */
void LCD_Init(void);                  // Initialize the LCD
void LCD_Command(uint8_t cmd);        // Send a command to the LCD
void LCD_Write(uint8_t data);         // Write data to the LCD
void LCD_Print(char *str);            // Print a string on the LCD
void LCD_Clear(void);                 // Clear the LCD display
void LCD_Set_Cursor(uint8_t row, uint8_t col); // Set cursor position

#endif /* __LCD_H */

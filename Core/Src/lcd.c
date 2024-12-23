#include "lcd.h"

/* Define GPIO Pins for LCD */
#define LCD_RS_PIN GPIO_PIN_0  // RS pin on PA0
#define LCD_EN_PIN GPIO_PIN_1  // Enable pin on PA1
#define LCD_DATA_PORT GPIOA    // Data pins PA8-PA11

/* Enable GPIO Port for control pins */
#define LCD_RS_GPIO GPIOA      // GPIO port for RS pin
#define LCD_EN_GPIO GPIOA      // GPIO port for Enable pin

/* Helper Macros */
#define LCD_RS_HIGH() HAL_GPIO_WritePin(LCD_RS_GPIO, LCD_RS_PIN, GPIO_PIN_SET)
#define LCD_RS_LOW() HAL_GPIO_WritePin(LCD_RS_GPIO, LCD_RS_PIN, GPIO_PIN_RESET)
#define LCD_EN_HIGH() HAL_GPIO_WritePin(LCD_EN_GPIO, LCD_EN_PIN, GPIO_PIN_SET)
#define LCD_EN_LOW() HAL_GPIO_WritePin(LCD_EN_GPIO, LCD_EN_PIN, GPIO_PIN_RESET)

void LCD_Init(void)
{
    // GPIO Initialization for RS, EN, and Data pins
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // Enable clock for GPIOA (assuming you are using GPIOA for control pins)
    __HAL_RCC_GPIOA_CLK_ENABLE();

    // Initialize RS pin
    GPIO_InitStruct.Pin = LCD_RS_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;  // Push-pull mode
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(LCD_RS_GPIO, &GPIO_InitStruct);

    // Initialize EN pin
    GPIO_InitStruct.Pin = LCD_EN_PIN;
    HAL_GPIO_Init(LCD_EN_GPIO, &GPIO_InitStruct);

    // Initialize Data pins (PA8-PA11 for LCD data)
    GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11;
    HAL_GPIO_Init(LCD_DATA_PORT, &GPIO_InitStruct);

    HAL_Delay(20); // Wait for LCD to power up
    LCD_Command(0x02); // Initialize LCD in 4-bit mode
    LCD_Command(LCD_FUNCTION_SET); // Function set: 4-bit, 2 lines, 5x8 font
    LCD_Command(LCD_DISPLAY_ON); // Display ON, Cursor OFF
    LCD_Command(LCD_CLEAR); // Clear display
    LCD_Command(LCD_ENTRY_MODE); // Entry mode: Increment cursor
    HAL_Delay(2);
}

void LCD_Command(uint8_t cmd)
{
    LCD_RS_LOW();  // RS = 0 for command
    LCD_Write(cmd >> 4); // Send upper nibble
    LCD_Write(cmd & 0x0F); // Send lower nibble
}

void LCD_Write(uint8_t nibble)
{
    // Set data pins (PA8-PA11)
    HAL_GPIO_WritePin(LCD_DATA_PORT, GPIO_PIN_8, (nibble >> 0) & 0x01);
    HAL_GPIO_WritePin(LCD_DATA_PORT, GPIO_PIN_9, (nibble >> 1) & 0x01);
    HAL_GPIO_WritePin(LCD_DATA_PORT, GPIO_PIN_10, (nibble >> 2) & 0x01);
    HAL_GPIO_WritePin(LCD_DATA_PORT, GPIO_PIN_11, (nibble >> 3) & 0x01);

    // Trigger Enable Pin (Generate pulse)
    LCD_EN_HIGH();
    HAL_Delay(1);
    LCD_EN_LOW();
    HAL_Delay(1);
}

void LCD_Print(char *str)
{
    while (*str)
    {
        LCD_RS_HIGH();  // RS = 1 for data
        LCD_Write(*str >> 4); // Send upper nibble
        LCD_Write(*str & 0x0F); // Send lower nibble
        str++;
    }
}

void LCD_Clear(void)
{
    LCD_Command(LCD_CLEAR); // Clear display command
    HAL_Delay(2); // Wait for the command to execute
}

void LCD_Set_Cursor(uint8_t row, uint8_t col)
{
    uint8_t address;

    // Calculate the DDRAM address based on row and column
    if (row == 1)
        address = 0x80 + (col - 1); // First row
    else
        address = 0xC0 + (col - 1); // Second row

    LCD_Command(address); // Set DDRAM address
}

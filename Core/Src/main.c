/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "rc522.h"
#include "string.h"
#include "lcd.h"
#include<stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define MILK_PRICE 50
#define BISCUIT_PRICE 15

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;

/* USER CODE BEGIN PV */
uint8_t status;
uint8_t str[MAX_LEN]; // Max_LEN = 16
uint8_t sNum[5];
uint8_t total_price = 0;
uint8_t milk_count = 0;
uint8_t biscuit_count = 0;
uint8_t display_total = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

	 HAL_Init();
	  SystemClock_Config();
	  MX_GPIO_Init();
	  MX_SPI1_Init();
	  MFRC522_Init();
	  LCD_Init();

	  // Display the welcome message
	  LCD_Clear();
	  LCD_Set_Cursor(1, 1);
	  LCD_Print("Welcome to the");
	  LCD_Set_Cursor(2, 1);
	  LCD_Print("Supermarket");
	  HAL_Delay(3000);

	  LCD_Clear();
	  LCD_Set_Cursor(1, 1);
	  LCD_Print("Add your items");

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */
  MFRC522_Init();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

	  status = MFRC522_Request(PICC_REQIDL, str);
	      status = MFRC522_Anticoll(str);

	      if (status == MI_OK) // Card detected
	      {
	        if ((str[0] == 0x24) && (str[1] == 0xFF) && (str[2] == 0xCC) && (str[3] == 0xDC) && (str[4] == 0xCB))
	        {
	          // Milk card detected
	          if (milk_count == 0) // Adding milk
	          {
	            milk_count++;
	            total_price += MILK_PRICE;
	            display_total = 1; // Set flag to display total

	            // Toggle LED for adding
	            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, 1);
	            HAL_Delay(500);
	            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, 0);
	            HAL_Delay(500);

	            // Display message
	            LCD_Clear();
	            LCD_Set_Cursor(1, 1);
	            LCD_Print("Milk added");
	            HAL_Delay(2000);
	          }
	          else // Removing milk
	          {
	            milk_count--;
	            total_price -= MILK_PRICE;

	            // Toggle second LED for removing
	            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, 1);
	            HAL_Delay(500);
	            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, 0);
	            HAL_Delay(500);

	            // Display message
	            LCD_Clear();
	            LCD_Set_Cursor(1, 1);
	            LCD_Print("Milk removed");
	            HAL_Delay(2000);
	          }
	        }
	        else if ((str[0] == 0x33) && (str[1] == 0x84) && (str[2] == 0xE1) && (str[3] == 0x26) && (str[4] == 0x70))
	        {
	          // Biscuit card detected
	          if (biscuit_count == 0) // Adding biscuit
	          {
	            biscuit_count++;
	            total_price += BISCUIT_PRICE;
	            display_total = 1; // Set flag to display total

	            // Toggle LED for adding
	            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, 1);
	            HAL_Delay(500);
	            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, 0);
	            HAL_Delay(500);

	            // Display message
	            LCD_Clear();
	            LCD_Set_Cursor(1, 1);
	            LCD_Print("Biscuit added");
	            HAL_Delay(2000);
	          }
	          else // Removing biscuit
	          {
	            biscuit_count--;
	            total_price -= BISCUIT_PRICE;

	            // Toggle second LED for removing
	            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, 1);
	            HAL_Delay(500);
	            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, 0);
	            HAL_Delay(500);

	            // Display message
	            LCD_Clear();
	            LCD_Set_Cursor(1, 1);
	            LCD_Print("Biscuit removed");
	            HAL_Delay(2000);
	          }
	        }
	      }

	      // Display total price continuously if the flag is set
	      if (display_total)
	      {
	        LCD_Clear();
	        LCD_Set_Cursor(1, 1);
	        LCD_Print("Total Amount:");
	        LCD_Set_Cursor(2, 1);
	        char buffer[16];
	        sprintf(buffer, "Rs %d", total_price);
	        LCD_Print(buffer);
	        HAL_Delay(1000);
	      }
	      else
	      {
	        // Reset to "Add your items" if no items are added
	        LCD_Clear();
	        LCD_Set_Cursor(1, 1);
	        LCD_Print("Add your items");
	        HAL_Delay(1000);
	      }
	    }

	    /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13|GPIO_PIN_14, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_8
                          |GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);

  /*Configure GPIO pins : PC13 PC14 */
  GPIO_InitStruct.Pin = GPIO_PIN_13|GPIO_PIN_14;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PA0 PA1 PA4 PA8
                           PA9 PA10 PA11 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_8
                          |GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PB0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
/*
void LCD_Init(void)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, 0); // RS = 0
    HAL_Delay(15);
    LCD_Command(0x02); // Initialize LCD in 4-bit mode
    LCD_Command(0x28); // 2-line display, 5x8 font
    LCD_Command(0x0C); // Display on, cursor off
    LCD_Command(0x06); // Auto increment cursor
    LCD_Command(0x01); // Clear display
    HAL_Delay(2);
}

void LCD_Command(uint8_t cmd)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, 0); // RS = 0
    LCD_Write(cmd);
}

void LCD_Write(uint8_t data)
{
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, (data >> 4) & 0x1);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, (data >> 5) & 0x1);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, (data >> 6) & 0x1);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, (data >> 7) & 0x1);
    // Trigger the Enable Pin
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, 1);
    HAL_Delay(1);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, 0);

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, data & 0x1);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, (data >> 1) & 0x1);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, (data >> 2) & 0x1);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, (data >> 3) & 0x1);
    // Trigger the Enable Pin
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, 1);
    HAL_Delay(1);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, 0);
}

void LCD_Print(char *str)
{
    while (*str)
    {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, 1); // RS = 1
        LCD_Write(*str++);
    }
}
*/

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

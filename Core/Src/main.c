/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#define memTemperatureBase  0
#define memTemperatureSize  4
#define memC02Base (memTemperatureBase + memTemperatureSize)
#define memC02Size 4
#define memDeviceNameBase (memC02Bas + memC02Size)
#define memDeviceNameSize 20
//할당해주면 메모리가 중첩될 일이 없다

void writeEeprom(uint16_t address, uint8_t data) {
	uint8_t txBuffer[3];
	txBiffer[0] = address >> 8; //하위비트
	txBiffer[0] = address & 0x00ff; //상위비트
	txBuffer[2] = data;
	HAL_I2C_Master_Transmit(&hi2c1,0xAE ,txBiffer ,3 ,1 );
	// 3=sizeof(txBuffer)
	//0xAE 하드웨어 주소에 따라서 바뀜
	HAL_Delay(5);
}

uint8_t readEeprom(uint8_t address){
	//return하기 위한
	uint8_t txBuffer[2];
	uint8_t rxBuffer;
	txBiffer[0] = address >> 8; //하위비트
	txBiffer[0] = address & 0x00ff; //상위비트
	HAL_I2C_Master_Transmit(&hi2c1,0xAE ,txBiffer ,2 ,1 );
	//전송하며서 txBuffer의 개수는 2개가 됨
	HAL_I2C_Master_Receive(&hi2c, 0xA0, &rxBiffer, 1, 1);
	//읽은 값이 저장이 되어있으니까 이 값을 return 시켜주면 됨
	return rxBuffer;
}


void writeEeprom(uint16_t address, uint8_t data) {
	HAL_I2C_Mem_Write(&hi2c, 0xA0, address, 2, &data, 1, 1);
}

uint8_t readEeprom(uint16_t address, uint8_t address){
	HAL_I2C_Mem_Read(&hi2c, 0xA0, address, 2, &rxBiffer, 1, 1);
	return rxBuffer;
}


typedef struct {
	uint8_t sec;
	uint8_t min;
	uint8_t hour;
	uint8_t day;
	uint8_t data;
	uint8_t month;
	uint8_t year;
} DataTime_t;

voide writeRtc(DataTime_t dataTime){
	/* uint8_t txBuffer[7];
	memcpy(txBuffer,dataTime, 7);*/ // 안써도 됨 (주소가 순서대로 저장되어 있기 때문임)
	HAL_I2C_Mem_Write(&hi2c1, 0xA0, address, 2, &dataTime, 7, 1);
}

DataTime_t readRtc() {
	DataTime_t result;
	HAL_I2C_Mem_Read(&hi2c1, 0xD0, 0, 1, &result, 7, 1);
	return result;
}

void writeEeprom4Byte(uint16_t address, uint32_t data) {
	uint8_t buffer[4];
	/* buffer[0] = data >> 24;
	buffer[1] = data >> 16;
	buffer[2] = data >> 8;
	buffer[3] = data; */
	memcpy(buffer, &data, 4);
	//memcp은 32->8bit(반으로 쪼개짐, 굳이 bit shift 할 필요가 없이 씸플해짐)
	writeEeprom(address, buffer[0]);
	writeEeprom(address1, buffer[1]);
	writeEeprom(address2, buffer[2]);
	writeEeprom(address3, buffer[3]);
	//4byte를 write함
}
//두개의 offset은 같아야함 = address의 순서가 같아야함
uint32_t readEeprom4Byte(uint16_t address) {
	uint8_t rxBuffer[4];
	uint32_t result;
	rxBuffer[0]= readEeprom(address);
	rxBuffer[1]= readEeprom(address + 1);
	rxBuffer[2]= readEeprom(address + 2);
	rxBuffer[3]= readEeprom(address + 3);
	memcpy(&result, rxBuffer, 4);
	return result;
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

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
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
  writeEeprom4Byte(memTemperatureBase, 0x12345678);
  writeEeprom4Byte (memC02Base, 0x87654321);

  //미리 배열에 담아두고 hal이 자동으로 그 기능을 실행해줌 (ack 신호 보내고... 등등)
  //write action
  uint8_t txBuffer[3];
  txBuffer[0] = upperAddress;
  txBuffer[1] = lowerAddress;
  txBuffer[2] = data;
  HAL_I2C_Master_Transmit(&hi2c1, 0x7f, txBuffer, 3, 1);

  //for read from eeprom
  uint8_t txBuffer[2];
  txBuffer[0] = upperAddress;
  txBuffer[1] = lowerAddress;
  HAL_I2C_Master_Transmit(&hi2c1, 0x7f, txBuffer, 2, 1);
  HAL_I2C_Master_Receive(&hi2c1,0x7f, &rxBuffer, 1, 1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 400000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOB_CLK_ENABLE();

}

/* USER CODE BEGIN 4 */

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

/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include 	"main.h"

/*E N V I R O N M E N T A L    S E N S O R S*/
#include	"stts22h.h" 		// Temprature
#include 	"sgp40.h"			// Gas
#include 	"sht40ad1b.h"		// Humidity + Temprature
#include	"lps22df.h"			// Pressure + Temprature
#include 	"sensirion_gas_index_algorithm.h"

/*M O T I O N    S E N S O R S*/
#include	"lis2duxs12.h"		// Accelerometer
#include	"lis2mdl.h"			// Magnetometer
#include	"lsm6dso16is.h"		// Accelerometer + Gyro
#include	"lsm6dsv16x.h"		// Accelerometer + Gyro


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

COM_InitTypeDef BspCOMInit;

I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_I2C1_Init(void);
/* USER CODE BEGIN PFP */
int32_t I2C_Init(void);
int32_t I2C_DeInit(void);
int32_t I2C_Write(uint16_t DevAddr, uint8_t *pData, uint16_t Length);
int32_t I2C_Write_Reg(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length);
int32_t I2C_Read(uint16_t DevAddr, uint8_t *pData, uint16_t Length);
int32_t I2C_Read_Reg(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length);

int32_t I2C_GetTick(void);
void    I2C_Delay(uint32_t ms);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
SGP40_IO_t sgp40_io = {
	.Init    = I2C_Init,
	.DeInit  = I2C_DeInit,
	.BusType = SGP40_I2C_BUS,
	.Address = SGP40_I2C_ADDRESS,
	.Write   = I2C_Write,
	.Read    = I2C_Read,
	.GetTick = I2C_GetTick,
	.Delay   = I2C_Delay
};

SHT40AD1B_IO_t sht40io = {
	.Init    = I2C_Init,
	.DeInit  = I2C_DeInit,
	.BusType = SHT40AD1B_I2C_BUS,
	.Address = SHT40AD1B_I2C_ADDRESS,
	.Write   = I2C_Write,
	.Read    = I2C_Read,
	.GetTick = I2C_GetTick,
	.Delay   = I2C_Delay
};

STTS22H_IO_t stts22h_io = {
	.Init    	= I2C_Init,
	.DeInit  	= I2C_DeInit,
	.BusType 	= STTS22H_I2C_BUS,
	.Address 	= STTS22H_I2C_ADD_H,
	.WriteReg   = I2C_Write_Reg,
	.ReadReg    = I2C_Read_Reg,
	.GetTick 	= I2C_GetTick,
	.Delay   	= I2C_Delay
};

LPS22DF_IO_t lps22_io = {
	.Init    	= I2C_Init,
	.DeInit  	= I2C_DeInit,
	.BusType 	= LPS22DF_I2C_BUS,
	.Address 	= LPS22DF_I2C_ADD_H,
	.WriteReg   = I2C_Write_Reg,
	.ReadReg   	= I2C_Read_Reg,
	.GetTick 	= I2C_GetTick,
	.Delay   	= I2C_Delay
};


SGP40_Object_t sgp40_obj;
GasIndexAlgorithmParams params;
SHT40AD1B_Object_t sht40obj;
STTS22H_Object_t stts22hobj;
LPS22DF_Object_t lps22obj;

//-----------------------------------------------------------------------//
//                          M O T I O N                                 //

LIS2DUXS12_IO_t lis2dux_io = {
	.Init        = I2C_Init,
	.DeInit      = I2C_DeInit,
	.BusType     = 0U,
	.Address     = LIS2DUXS12_I2C_ADD_H,
	.WriteReg    = I2C_Write_Reg,
	.ReadReg     = I2C_Read_Reg,
	.GetTick     = I2C_GetTick,
	.Delay       = I2C_Delay
};


LIS2MDL_IO_t lis2mdl_io = {
	.Init        = I2C_Init,
	.DeInit      = I2C_DeInit,
	.BusType     = 0U,
	.Address     = LIS2MDL_I2C_ADD,
	.WriteReg    = I2C_Write_Reg,
	.ReadReg     = I2C_Read_Reg,
	.GetTick     = I2C_GetTick,
	.Delay       = I2C_Delay
};



LSM6DSO16IS_IO_t lsm6dso_io = {
	.Init        = I2C_Init,
	.DeInit      = I2C_DeInit,
	.BusType     = 0U,
	.Address     = LSM6DSO16IS_I2C_ADD_L,
	.WriteReg    = I2C_Write_Reg,
	.ReadReg     = I2C_Read_Reg,
	.GetTick     = I2C_GetTick,
	.Delay       = I2C_Delay
};


LSM6DSV16X_IO_t lsm6dsv_io = {
	.Init        = I2C_Init,
	.DeInit      = I2C_DeInit,
	.BusType     = 0U,
	.Address     = LSM6DSV16X_I2C_ADD_H,
	.WriteReg    = I2C_Write_Reg,
	.ReadReg     = I2C_Read_Reg,
	.GetTick     = I2C_GetTick,
	.Delay       = I2C_Delay
};

LIS2DUXS12_Object_t lis2duxobj;
LIS2MDL_Object_t lis2mdlobj;
LSM6DSO16IS_Object_t lsm6dsoobj;
LSM6DSV16X_Object_t lsm6dsvobj;

//----------------------------------------------------------------------//


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
	MX_USART1_UART_Init();
	MX_I2C1_Init();
	/* USER CODE BEGIN 2 */

	/* USER CODE END 2 */

	/* Initialize leds */
	BSP_LED_Init(LED_GREEN);
	BSP_LED_Init(LED_BLUE);

	/* Initialize USER push-button, will be used to trigger an interrupt each time it's pressed.*/
	BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI);

	/* Initialize COM1 port (115200, 8 bits (7-bit data + 1 stop bit), no parity */
	BspCOMInit.BaudRate   = 115200;
	BspCOMInit.WordLength = COM_WORDLENGTH_8B;
	BspCOMInit.StopBits   = COM_STOPBITS_1;
	BspCOMInit.Parity     = COM_PARITY_NONE;
	BspCOMInit.HwFlowCtl  = COM_HWCONTROL_NONE;
	if (BSP_COM_Init(COM1, &BspCOMInit) != BSP_ERROR_NONE)
	{
	Error_Handler();
	}


	STTS22H_RegisterBusIO(&stts22hobj, &stts22h_io);
	STTS22H_Init(&stts22hobj);
	STTS22H_TEMP_Enable(&stts22hobj);
	float stts22h_temp;

	LPS22DF_RegisterBusIO(&lps22obj,&lps22_io);
	LPS22DF_Init(&lps22obj);
	LPS22DF_TEMP_Enable(&lps22obj);
	LPS22DF_PRESS_Enable(&lps22obj);
	float lps22_temp;
	float lps22_press;

	SHT40AD1B_RegisterBusIO(&sht40obj,&sht40io);
	SHT40AD1B_Init(&sht40obj);
	SHT40AD1B_HUM_Enable(&sht40obj);
	SHT40AD1B_TEMP_Enable(&sht40obj);
	float sht40_hum;
	float sht40_temp;

	SGP40_RegisterBusIO(&sgp40_obj, &sgp40_io);
	SGP40_Init(&sgp40_obj);
	SGP40_GAS_Enable(&sgp40_obj);
	GasIndexAlgorithm_init(&params, GasIndexAlgorithm_ALGORITHM_TYPE_VOC);
	float sgp40_voc_val;
	int32_t voc_index_value;




    LIS2DUXS12_RegisterBusIO(&lis2duxobj,&lis2dux_io);
    LIS2DUXS12_Init(&lis2duxobj);
    LIS2DUXS12_ACC_Enable(&lis2duxobj);
    LIS2DUXS12_Axes_t lis_acc;

    LIS2MDL_RegisterBusIO(&lis2mdlobj,&lis2mdl_io);
    LIS2MDL_Init(&lis2mdlobj);
    LIS2MDL_MAG_Enable(&lis2mdlobj);
    LIS2MDL_Axes_t mag;

    LSM6DSO16IS_RegisterBusIO(&lsm6dsoobj, &lsm6dso_io);
    LSM6DSO16IS_Init(&lsm6dsoobj);
    LSM6DSO16IS_ACC_Enable(&lsm6dsoobj);
    LSM6DSO16IS_GYRO_Enable(&lsm6dsoobj);
    LSM6DSO16IS_Axes_t lsm6dso_acc, lsm6dso_gyro;

    LSM6DSV16X_RegisterBusIO(&lsm6dsvobj, &lsm6dsv_io);
	LSM6DSV16X_Init(&lsm6dsvobj);
	LSM6DSV16X_ACC_Enable(&lsm6dsvobj);
	LSM6DSV16X_GYRO_Enable(&lsm6dsvobj);
	LSM6DSV16X_Axes_t lsm6dsv_acc, lsm6dsv_gyro;

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

	/* USER CODE END WHILE */

	  STTS22H_TEMP_GetTemperature(&stts22hobj, &stts22h_temp);

	  LPS22DF_TEMP_GetTemperature(&lps22obj, &lps22_temp);
	  LPS22DF_PRESS_GetPressure(&lps22obj, &lps22_press);

	  SHT40AD1B_HUM_GetHumidity(&sht40obj, &sht40_hum);
	  SHT40AD1B_TEMP_GetTemperature(&sht40obj, &sht40_temp);

	  SGP40_GAS_Set_Humidity(&sgp40_obj, sht40_hum);
	  SGP40_GAS_Set_Temperature(&sgp40_obj, sht40_temp);
	  SGP40_GAS_GetValue(&sgp40_obj, &sgp40_voc_val);
	  GasIndexAlgorithm_process(&params, sgp40_voc_val, &voc_index_value);

//		  printf("\033\143"); // goes top left and clears screen (ANSI)

	  BSP_LED_Toggle(LED_BLUE);

	//	  	  printf("STTS22H TEMP: %.2f °C\r\n", stts22h_temp);
	//
	//	  	  printf("\r\nLPS22 TEMP: %.2f °C\r\n", lps22_temp);
	//	  	  printf("LPS22 PRESS: %.2f hPa\r\n", lps22_press);
	//
	//	  	  printf("\r\nSHT40 TEMP: %.2f °C\r\n", sht40_temp);
	//	  	  printf("SHT40 HUM: %.2f %%\r\n", sht40_hum);
	//
	//	  	  printf("\r\nSGP40 VOC Value: %.2f\r\n", sgp40_voc_val);
	//	  	  printf("SGP40 VOC INDEX: %ld \r\n", voc_index_value);






	  LIS2DUXS12_ACC_GetAxes(&lis2duxobj, &lis_acc);

	  LIS2MDL_MAG_GetAxes(&lis2mdlobj, &mag);

	  LSM6DSO16IS_ACC_GetAxes(&lsm6dsoobj, &lsm6dso_acc);
	  LSM6DSO16IS_GYRO_GetAxes(&lsm6dsoobj, &lsm6dso_gyro);

	  LSM6DSV16X_ACC_GetAxes(&lsm6dsvobj, &lsm6dsv_acc);
	  LSM6DSV16X_GYRO_GetAxes(&lsm6dsvobj, &lsm6dsv_gyro);

	  BSP_LED_Toggle(LED_GREEN);

	//	  	  printf("\r\nLIS2DUX X: %ld\tLIS2DUX Y: %ld\tLIS2DUX Z: %ld\r\n",lis_acc.x,lis_acc.y,lis_acc.z);
	//
	//	  	  printf("\r\nLIS2MDL X: %ld\tLIS2MDL Y: %ld\tLISMDL Z: %ld\r\n",mag.x,mag.y,mag.z);
	//
	//	  	  printf("\r\nLSM6DSO X: %ld\tLSM6DSO Y: %ld\tLSM6DSO Z: %ld\r\n",lsm6dso_acc.x,lsm6dso_acc.y,lsm6dso_acc.z);
	//	  	  printf("LSM6DSO GYR X: %ld\tLSM6DSO GYR Y: %ld\tLSM6DSO GYR Y: %ld\r\n",lsm6dso_gyro.x,lsm6dso_gyro.y,lsm6dso_gyro.z);
	//
	//	  	  printf("\r\nLSM6DSV X: %ld\tLSM6DSV Y: %ld\tLSM6DSV Z: %ld\r\n",lsm6dsv_acc.x,lsm6dsv_acc.y,lsm6dsv_acc.z);
	//	  	  printf("LSM6DSV GYR X: %ld\tLSM6DSV GYR Y: %ld\tLSM6DSV GYR Y: %ld\r\n",lsm6dsv_gyro.x,lsm6dsv_gyro.y,lsm6dsv_gyro.z);



		  //print csv
	  printf("%.2f,%.2f,%.2f,"
			   "%.2f,%.2f,%ld,"
			   "%ld,%ld,%ld,"
			   "%ld,%ld,%ld,"
			   "%ld,%ld,%ld,%ld,%ld,%ld,"
			   "%ld,%ld,%ld,%ld,%ld,%ld\r\n",
			   stts22h_temp, lps22_temp, sht40_temp,
			   lps22_press, sht40_hum, voc_index_value,
			   lis_acc.x, lis_acc.y, lis_acc.z,
			   mag.x, mag.y, mag.z,
			   lsm6dso_acc.x, lsm6dso_acc.y, lsm6dso_acc.z,
			   lsm6dso_gyro.x, lsm6dso_gyro.y, lsm6dso_gyro.z,
			   lsm6dsv_acc.x, lsm6dsv_acc.y, lsm6dsv_acc.z,
			   lsm6dsv_gyro.x, lsm6dsv_gyro.y, lsm6dsv_gyro.z);

	//	  	  printf("\033\143");

	//	  	  HAL_Delay(10); something in the function calls a delay so i dont need one
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

  __HAL_FLASH_SET_LATENCY(FLASH_LATENCY_0);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV4;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV1;

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
  hi2c1.Init.Timing = 0x00402D41;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart1, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart1, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
int32_t I2C_Init(void) {
    // Already called in MX_I2C1_Init(), but included here for completeness
    return (HAL_I2C_Init(&hi2c1) == HAL_OK) ? 0 : -1;
}

int32_t I2C_DeInit(void) {
    return (HAL_I2C_DeInit(&hi2c1) == HAL_OK) ? 0 : -1;
}

int32_t I2C_Write(uint16_t DevAddr, uint8_t *pData, uint16_t Length) {
    return (HAL_I2C_Master_Transmit(&hi2c1, DevAddr, pData, Length, HAL_MAX_DELAY) == HAL_OK) ? 0 : -1;
}

int32_t I2C_Write_Reg(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length) {
    return (HAL_I2C_Mem_Write(&hi2c1, DevAddr, Reg,
                              I2C_MEMADD_SIZE_8BIT, pData, Length, HAL_MAX_DELAY) == HAL_OK) ? 0 : -1;
}


int32_t I2C_Read(uint16_t DevAddr, uint8_t *pData, uint16_t Length) {
    return (HAL_I2C_Master_Receive(&hi2c1, DevAddr, pData, Length, HAL_MAX_DELAY) == HAL_OK) ? 0 : -1;
}

int32_t I2C_Read_Reg(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length) {
    return (HAL_I2C_Mem_Read(&hi2c1, DevAddr, Reg,
                             I2C_MEMADD_SIZE_8BIT, pData, Length, HAL_MAX_DELAY) == HAL_OK) ? 0 : -1;
}



//unnecessary but for the sake of completeness
int32_t I2C_GetTick(void) {
    return HAL_GetTick();
}

//unnecessary but for the sake of completeness
void I2C_Delay(uint32_t ms) {
    HAL_Delay(ms);
}

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
#ifdef USE_FULL_ASSERT
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

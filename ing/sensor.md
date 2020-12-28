# stm -> sensor 통신

## STM32F072RB 보드 사용
## Pinout 이미지
![PIR_Sensor](https://user-images.githubusercontent.com/50731733/103164399-2926f500-484e-11eb-9498-354f042f49ff.PNG)

### PIR sensor (적외선 인체감지 센서)
#### Sensor 연결 : PA9 <-> D8번 Pin
![PIR_Sensor2](https://user-images.githubusercontent.com/50731733/103165981-a14ae600-4861-11eb-9482-086d709e7ba1.PNG)

이미지 출처 : https://m.blog.naver.com/PostView.nhn?blogId=eduino&logNo=220892296492&proxyReferer=https:%2F%2Fwww.google.com%2F
#### Test Code
 while (1)  
  {  
	  if (HAL_GPIO_ReadPin(Sensor_GPIO_Port, Sensor_Pin))  
		  printf("Detected!!!\r\n");  
	  else  
		  printf("No Detected...\r\n");  
	  HAL_Delay(1000);  
  }  
#### Test 결과 이미지
![PIR_Sensor_Test_Result](https://user-images.githubusercontent.com/50731733/103164499-5aec8b80-484f-11eb-9f65-072e5b4a6eb0.PNG)

### Relay Module
#### Pin Image
![Relay_Pin](https://user-images.githubusercontent.com/50731733/103164553-55437580-4850-11eb-81c5-b911c175c90b.PNG)

이미지 출처 : https://it-g-house.tistory.com/entry/Arduino-%EC%95%84%EB%91%90%EC%9D%B4%EB%85%B8-%EB%A6%B4%EB%A0%88%EC%9D%B4-Relay-%EC%8A%A4%EC%9C%84%EC%B9%98-%EC%82%AC%EC%9A%A9%EB%B0%A9%EB%B2%95
#### 연결 : PB5 <-> D4
#### Test Code
HAL_GPIO_WritePin(Relay_GPIO_Port, Relay_Pin, SET);  
HAL_Delay(1000);  
HAL_GPIO_WritePin(Relay_GPIO_Port, Relay_Pin, RESET);  
HAL_Delay(1000);  

### 미세먼지 센서(PM2008)
#### Pin Image
#### Sensor 연결 : PA1 <-> A1번 Pin
#### UART 연결 : USART3_TX [PB10 <-> D6], USART3_RX [PB11 <-> CN10->18]
#### Test Code
  uint8_t mise_buffer[32];
  uint8_t mise_send_buffer[7] = {0x42,0x4d,0};
  
  if(HAL_UART_Receive_DMA(&huart3,mise_buffer,32)!=HAL_OK)
  	printf("fail\r\n");
  write_MISE("Passive");
  
  void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
  
	if(huart->Instance == USART3) {
		print_MISE();
	}
  }
  
  void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
  {
  
	if(huart->Instance==USART3)
	{
	}
  }
  
  void print_MISE(void)
  {
  
	uint16_t combine_value, check_byte_receive, check_byte_calculate=0;

	check_byte_receive=mise_buffer[30]<<8|mise_buffer[31];
	for(uint8_t i=0;i<30;i++)
		check_byte_calculate+=mise_buffer[i];

	if(check_byte_receive==check_byte_calculate)
	{
		printf("PM1.0 : %d	",(combine_value=(mise_buffer[10]<<8)|mise_buffer[11]));
		printf("PM2.5 : %d	",(combine_value=(mise_buffer[12]<<8)|mise_buffer[13]));
		printf("PM10 : %d	",(combine_value=(mise_buffer[14]<<8)|mise_buffer[15]));
		printf("0.3um : %d	",(combine_value=(mise_buffer[16]<<8)|mise_buffer[17]));
		printf("0.5um : %d	",(combine_value=(mise_buffer[18]<<8)|mise_buffer[19]));
		printf("1.0um : %d	",(combine_value=(mise_buffer[20]<<8)|mise_buffer[21]));
		printf("2.5um : %d	",(combine_value=(mise_buffer[22]<<8)|mise_buffer[23]));
		printf("5.0um : %d	",(combine_value=(mise_buffer[24]<<8)|mise_buffer[25]));
		printf("10.0um : %d\r\n",(combine_value=(mise_buffer[26]<<8)|mise_buffer[27]));
	}
	else
	{
	}
  }
  
  void write_MISE(char* cmd)
  {
 
	uint16_t verify_byte=0;

	printf("Enter the write_MISE\r\n");

	if(strcmp(cmd,"Read")==0)
	{
		mise_send_buffer[2]=0xe2;
		mise_send_buffer[3]=0x00;
		mise_send_buffer[4]=0x00;
		printf("if's Read\r\n");
	}
	else if(strcmp(cmd,"Passive")==0)
	{
		mise_send_buffer[2]=0xe1;
		mise_send_buffer[3]=0x00;
		mise_send_buffer[4]=0x00;
		printf("if's Passive\r\n");
	}
	else if(strcmp(cmd,"Active")==0)
	{
		mise_send_buffer[2]=0xe1;
		mise_send_buffer[3]=0x00;
		mise_send_buffer[4]=0x01;
		printf("if's Active\r\n");

	}
	else if(strcmp(cmd,"Sleep")==0)
	{
		mise_send_buffer[2]=0xe4;
		mise_send_buffer[3]=0x00;
		mise_send_buffer[4]=0x00;
		printf("if's Sleep\r\n");
	}
	else if(strcmp(cmd,"WakeUp")==0)
	{
		mise_send_buffer[2]=0xe4;
		mise_send_buffer[3]=0x00;
		mise_send_buffer[4]=0x01;
		printf("if's WakeUp\r\n");
	}
	for(uint8_t i=0;i<5;i++)
		verify_byte+=mise_send_buffer[i];
	mise_send_buffer[5]=verify_byte>>8;
	mise_send_buffer[6]=verify_byte;

	while(HAL_UART_GetState(&huart3)!=HAL_UART_STATE_READY)
	{
	}

	if(HAL_UART_Transmit_IT(&huart3,(uint8_t*)mise_send_buffer,7)!=HAL_OK)
	{
	}
	if(strcmp(cmd,"Read")==0)
	{
		while(HAL_UART_GetState(&huart3)!=HAL_UART_STATE_READY)
		{
		}
		if(HAL_UART_Receive_IT(&huart3,mise_send_buffer, 32)!=HAL_OK)
		{
		}
	}

	printf("%d\r\n", mise_send_buffer);
  }
  
### LCD
#### Pin Image
![LCD_Pin](https://user-images.githubusercontent.com/50731733/103191404-3a3c3880-4918-11eb-968e-d4522ebcd6c6.PNG)
출처 : https://learn.sparkfun.com/tutorials/avr-based-serial-enabled-lcds-hookup-guide/i2c-hardware-hookup--example-code---basic
#### Library
https://github.com/MarioHesse/SparkFun_SerLCD_STM32_Library
#### Test Code
  while (1)
  {
	  status_lcd = displayInit(&hi2c1);
	  printf("test\r\n");
	  displayWriteString("TEST");
	  HAL_Delay(2000);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
#### Test 결과 이미지
![LCD_Test_Result](https://user-images.githubusercontent.com/50731733/103191499-930bd100-4918-11eb-9bca-0772210a31d0.jpg)


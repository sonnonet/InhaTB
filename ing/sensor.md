# stm -> sensor 통신

## PIR sensor (적외선 인체감지 센서)
### STM32F072RB 보드 사용
#### Sensor 연결 : PA9 <-> D8번 Pin
#### Pinout 이미지
#### Test_Code
##### while (1)
#####  {
#####	  if (HAL_GPIO_ReadPin(Sensor_GPIO_Port, Sensor_Pin))
#####		  printf("Detected!!!\r\n");
#####	  else
#####		  printf("No Detected...\r\n");
#####	  HAL_Delay(1000);
#####  }
#### Test 결과 이미지

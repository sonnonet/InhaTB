# stm -> sensor 통신

## STM32F072RB 보드 사용
## Pinout 이미지
![PIR_Sensor](https://user-images.githubusercontent.com/50731733/103164399-2926f500-484e-11eb-9498-354f042f49ff.PNG)
### PIR sensor (적외선 인체감지 센서)
#### Sensor 연결 : PA9 <-> D8번 Pin 
![PIR_Sensor2](https://user-images.githubusercontent.com/50731733/103164463-ceda6400-484e-11eb-886c-e86b3c56644e.PNG)
#### Test_Code
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

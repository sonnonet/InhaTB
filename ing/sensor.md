# stm -> sensor 통신

## STM32F072RB 보드 사용
## Pinout 이미지
![PIR_Sensor](https://user-images.githubusercontent.com/50731733/103164399-2926f500-484e-11eb-9498-354f042f49ff.PNG)
### PIR sensor (적외선 인체감지 센서)
#### Sensor 연결 : PA9 <-> D8번 Pin 
이미지 출처 : https://m.blog.naver.com/PostView.nhn?blogId=eduino&logNo=220892296492&proxyReferer=https:%2F%2Fwww.google.com%2F
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

### Relay Module
#### Pin Image
![Relay_Pin](https://user-images.githubusercontent.com/50731733/103164553-55437580-4850-11eb-81c5-b911c175c90b.PNG)

이미지 출처 : https://it-g-house.tistory.com/entry/Arduino-%EC%95%84%EB%91%90%EC%9D%B4%EB%85%B8-%EB%A6%B4%EB%A0%88%EC%9D%B4-Relay-%EC%8A%A4%EC%9C%84%EC%B9%98-%EC%82%AC%EC%9A%A9%EB%B0%A9%EB%B2%95
#### 연결 : PB5 <-> D4
#### Test_Code
HAL_GPIO_WritePin(Relay_GPIO_Port, Relay_Pin, SET);  
HAL_Delay(1000);  
HAL_GPIO_WritePin(Relay_GPIO_Port, Relay_Pin, RESET);  
HAL_Delay(1000);  

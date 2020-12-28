# 라즈베리파이 

---
##### 팀원 김준호.

### stm32 <-> raspberry pi 3 uart 통신
1. stm32 Setting
  - USART3
  - tx : pc4, rx : pc5, baudrate : 9600
  - raspberry pi pinout
  - ![raspberry_pinout](https://user-images.githubusercontent.com/71058075/103200285-43390400-4930-11eb-9867-33d3353640ff.JPG)

- stm32의 tx를 rpi pinout의 GPIO15( RX ), rx를 GPIO14( TX )에 연결합니다.


2. raspberry pi Setting
  - raspi3 이상에선 블루투스를 uart에 사용하기에 블루투스를 끊어줍니다.
  ``` 
  sudo vi /boot/config.txt
  dtoverlay=pi3-disable-bt
  sudo systemctl disable hciuart && reboot
  ```


3. 미세먼지 API
  ```
  import requests
  from bs4 import BeautifulSoup

  URI="http://openapi.airkorea.or.kr/openapi/services/rest/ArpltnInforInqireSvc/getCtprvnMesureSidoLIst?serviceKey="
  SERVICE_KEY=" API 키 값"
  items="&numOfRows=28"
  PageNum="&pageNo=1"
  SidoName="&sidoName=경기"
  searchCondition="&searchCondition=HOUR"
  URI=URI+SERVICE_KEY+items+PageNum+SidoName+searchCondition
  response = requests.get(URI)
  soup = BeautifulSoup(response.text, 'html.parser')
  ItemList = soup.findAll('item')

  for item in ItemList :
      if item.find('cityname').text == '부천시':
          print(item.find('pm10value').text)
          print(item.find('pm25value').text)
  ```

4. STM32으로부터 미세먼지 데이터 수신
```
import serial

ser = serial.Serial('/dev/ttyAMA0')
record = ser.readline()
record = record[:-2].decode()
```

5. Rpi3 에서의 LED 제어

```
from gpiozero import LED
from time import sleep

R_LED = LED(2) # LED를 RPi3 GPIO 2번핀에 연결

R_LED.on()
sleep(1)
R_LED.off()
```

---

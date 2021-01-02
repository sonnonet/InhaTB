import requests
import serial
from time import sleep
from bs4 import BeautifulSoup

URI = "https://api.thingspark.kr/channels/entrys?apiKey='API KEY'"

count = 0

ser = serial.Serial('/dev/ttyAMA0')
result_stm = dict()
add_pm10 = "&field1="
add_pm25 = "&field2="
while True :
    record = ser.readline()
    record = record[:-2].decode()
    record = record.split(',')

    result_stm['pm10'] = record[0]
    result_stm['pm25'] = record[1]

    response = requests.get(URI + add_pm10 + result_stm['pm10'] + add_pm25 + result_stm['pm25'])

    soup = BeautifulSoup(response.text, 'html.parser')

#    count = count + 1
    print(result_stm)
    sleep(31)

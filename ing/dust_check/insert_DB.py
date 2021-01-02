import requests
import serial
from time import sleep

url = "https://api.thingspark.co.kr/update"

params = {
        "apiKey" : "n1bhXYshCWi3lVHW"
}
ser = serial.Serial('/dev/ttyAMA0')

while True :
    record = ser.readline()
    record = record[:-2].decode()
    record = record.split(',')

    params['field1'] = record[0]
    params['field2'] = record[1]

    r = requests.get(url, params=params)

#    count = count + 1
    print(params)
    sleep(31)

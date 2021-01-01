import requests
from time import sleep
from bs4 import BeautifulSoup
import json

class DustDB() :
    def __init__(self):
        self.URI = "https://api.thingspark.kr/channels/65730/last"
        self.response = requests.get(self.URI)

    def get_DB_data(self):
        dbData = dict()
        response = json.loads(self.response.text, encoding='utf-8')

        while True :
            try:
                dbData['pm10'] = int(response.get('field1', 'error'))
                dbData['pm25'] = int(response.get('field2', 'error'))
                break
            except Exception as e :
                print(f"{type(e)} -> {e}")
                continue

        return dbData

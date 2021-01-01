import requests
from bs4 import BeautifulSoup

class DustAPI() :
    def __init__(self):
        self.domain = "http://openapi.airkorea.or.kr/openapi/services/rest/ArpltnInforInqireSvc/getCtprvnMesureSidoLIst?serviceKey="
        self.key = "q%2BHdhDfr7T5e5JaGA%2Bc5blRpuS6036s95xU%2FQ7LKg69%2BSni4AJZuP7sKvUItFJLFwkyuSN3BtjBhW17oI7bmjw%3D%3D"
        self.items = "&numOfRows=28"
        self.page = "&pageNo=1"
        self.sidoname = "&sidoName=경기"
        self.searchCondition = "&searchCondition=HOUR"
        self.URI = self.domain + self.key + self.items + self.page + self.sidoname + self.searchCondition
        self.response = requests.get(self.URI)
        self.soup = BeautifulSoup(self.response.text, 'html.parser')
        self.itemList = self.soup.findAll('item')

    def get_API_data(self):
        result_api = dict()
        for item in self.itemList:
            if item.find('cityname').text == '부천시':
                result_api['pm10'] = int(item.find('pm10value').text)
                result_api['pm25'] = int(item.find('pm25value').text)

        return result_api

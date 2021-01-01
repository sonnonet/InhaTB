from time import sleep
#from gpiozero import LED
from dustDB import DustDB
from dustAPI import DustAPI

def check_dust() :

    db_instance = DustDB().get_DB_data()
    api_instance = DustAPI().get_API_data()

    #공기가 좋을 때
    #내부 pm2.5 <= 15, pm10 <= 30

    if db_instance['pm10'] > api_instance['pm10'] or db_instance['pm25'] > api_instance['pm25'] :
        #LED.off()
        #print("환기 필요")
    else :
        #LED.off()

if __name__ == '__main__' :

    while True :
        check_dust()
        sleep(30)


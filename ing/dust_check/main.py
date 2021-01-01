from time import sleep
from gpiozero import LED
from dustDB import DustDB
from dustAPI import DustAPI

def check_dust() :
    R_LED = LED(2)

    db_instance = DustDB().get_DB_data()
    api_instance = DustAPI().get_API_data()

    #공기가 좋을 때
    #내부 pm2.5 <= 15, pm10 <= 30

    if db_instance['pm10'] <= 30 or db_instance['pm25'] <= 15 :
        R_LED.off()
    else :
        R_LED.on()
        print(db_instance)


if __name__ == '__main__' :

    while True :
        check_dust()
        sleep(30)


### dustAPI.py
- 경기도 부천시에 해당하는 미세먼지 공공 API 값을 받아 <br>
  get_API_data() method를 통해 pm10과 pm2.5의 키를 가진 딕셔너리를 반환해줍니다.
 
### dustDB.py
- thingspark에 업로드된 실내 미세먼지 데이터값을 받아 <br>
  get_DB_data() method를 통해 pm10과 pm2.5의 키를 가진 딕셔너리를 반환해줍니다.
  
### main.py
- dustAPI와 dustDB를 통해 받아온 공공 API값과 실내 데이터 값을 비교하여 <br>
  실내의 미세먼지 농도가 실외의 미세먼지 농도보다 높을 경우 사용자에게 알려줍니다.
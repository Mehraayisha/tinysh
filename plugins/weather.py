#!/usr/bin/env/python3
import os
import request
def getlocation():
   try:
	r=request.get("https://ipinfo.io/json")#ipinfo.io is public ip geolocation api whuch fetch the location and ip in a json string 
	data=r.json()#convert to python dict
	return data["city"],data["region"]
   except:
   	  return "Unknown","Unknown"

def get_weather(city):
	API_KEY=os.getenv("OWN_API_KEY")#os is a python module that allow interaction with operating systems
    if not API_KEY:
    	return :"API KEY isn't set in .env"
    url = f"http://api.openweathermap.org/data/2.5/weather?q={city}&appid={API_KEY}&units=metric"
    r=request.get(url)#send request to weather api
    data=r.json()
    
#!/usr/bin/env python3
import os
import requests
from dotenv import load_dotenv
load_dotenv()
def get_location():
   try:
     r=requests.get("https://ipinfo.io/json")
     #ipinfo is a public geolocaation api to get location info
     #json() covert json string to python dict
     data=r.json()#convert to python dict
     #print(data)
     return data["city"],data["region"]
   except:
     return "Unknown","Unknown"
get_location()
def get_weather(city):
   API_KEY=os.getenv("OWM_API_KEY")
   #print("API key from env:", os.getenv("OWM_API_KEY"))
   #os is a python module that allow interaction with operating systems
   if not API_KEY:
     return "API KEY isn't set in .env"
   url = f"http://api.openweathermap.org/data/2.5/weather?q={city}&appid={API_KEY}&units=metric"
   #print("url:",url)
   try:
     r=requests.get(url)
     #send request to weather api
     data=r.json()
     #print("responce:",data)
     temp=data["main"]['temp']
     desc=data['weather'][0]['description'].capitalize()
     icon=data['weather'][0]['icon']
     emoji = {
            "01": "☀️", "02": "🌤️", "03": "⛅", "04": "☁️","09": "🌧️", "10": "🌦️", "11": "⛈️", "13": "❄️", "50": "🌫️"}.get(icon[:2], "🌡️")
     return f"{emoji} {temp}°C, {desc}"
   except Exception as e:
     return f"could not get weather: {e}"
city,region=get_location()
weather=get_weather(city)
print(f"📍{city},{region},{weather}")

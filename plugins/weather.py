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
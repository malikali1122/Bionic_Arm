#!/usr/bin/env python
# coding: utf-8

# In[2]:




import serial

"""Opening of the serial port"""
try:
    arduino = serial.Serial("COM8",timeout=1)
except:
    print('Please check the port')

"""Initialising variables""" 
rawdata=[]
count=0

"""Receiving data and storing it in a list"""
while True:
    rawdata.append(str(arduino.readline()))
    count+=1
print(rawdata)

def clean(L):#L is a list
    newl=[]#initialising the new list
    for i in range(len(L)):
        temp=L[i][2:]
        newl.append(temp[:-5])
    return newl
    
cleandata=clean(rawdata)

def write(L):
    file=open("data.txt",mode='w')
    for i in range(len(L)):
        file.write(L[i]+'\n')
    file.close()

write(cleandata)


# In[ ]:





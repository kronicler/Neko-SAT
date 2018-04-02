#!/usr/bin/python
import serial 
import io
import os
import time
import rospy
from std_msgs.msg import String 

command = "0"
count = 0
name = "/dev/ttyACM0"
ser = serial.Serial(str(name), 9600, timeout = 0.2)
sio = io.TextIOWrapper(io.BufferedRWPair(ser,ser))

class packetSender:
    def __init__(self):
        pub = rospy.Subscriber('command_topic',String,self.changePacket)

    def changePacket(self,data):
        global command
        command = data.data

def normalPacket():
    global command,count
    satData = []
    print(command)

    print("packet sent: " + str(count))
    count = count + 1
    command = "0"
    time.sleep(0.5)
    
    sio.write(unicode(command))
    satData.append(sio.readline())
    satData.append(sio.readline())

    print satData
    sio.flush()
    if count > 10:
        outfile = open("datafile.csv","a")
        outfile.write(satData[1][1:])
        outfile.close()

if __name__ == '__main__':
    rospy.init_node("baseSerial")
    packet = packetSender()
    while (True):
        normalPacket()
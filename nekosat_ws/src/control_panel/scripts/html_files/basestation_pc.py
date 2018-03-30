#!/usr/bin/python
import serial 
import io
import os
import time
import rospy
from std_msgs.msg import String 

command = "0"
count = 0

def initSerial():
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
    print(command)

    print("packet sent: " + str(count))
    count = count + 1
    command = "0"
    time.sleep(0.5)
    """
    sio.write(unicode(command))
    sio.flush()
    satData = sio.readline()    
    print satData

    outfile = open("datafile.txt","a")
    outfile.write(satData)
    outfile.close()
    """

if __name__ == '__main__':
    rospy.init_node("baseSerial")
    packet = packetSender()
    while (True):
        normalPacket()
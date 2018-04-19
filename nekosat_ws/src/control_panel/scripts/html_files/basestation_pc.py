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
pub = rospy.Publisher('data_topic',String)

class packetSender:
    def __init__(self):
        rospy.Subscriber('command_topic',String,self.changePacket)

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
    time.sleep(0.7)
    
    sio.write(unicode(command))
    satData.append(sio.readline())
    satData.append(sio.readline())

    temp = str(satData[1])[1:]
    data = temp.split(",")
    sio.flush()
    if count > 10 and len(temp) > 32:
        temp = temp[:-1] + ", " + str(44330 * (1 - (float(data[2])/101325)**(1/5.255))) + "\n"
        print(temp)
        pub.publish(temp[:-1])
        outfile = open("/home/ttg/catkin_ws/src/control_panel/scripts/html_files/datafile.csv","a")
        outfile.write(temp)
        outfile.close()

if __name__ == '__main__':
    rospy.init_node("baseSerial")
    packet = packetSender()
    while (True):
        normalPacket()
#!/usr/bin/python


#this code runs on the xbee coordinator that is set to API mode 2

import serial
from xbee import ZigBee
from xbee.helpers.dispatch import Dispatch
import time

PORT = '/dev/tty.usbserial-A900UF4T'
BAUD_RATE = 9600

UNKNOWN = '\xff\xfe' 
WHERE = '\x00\x13\xA2\x00\x40\x98\xDA\x08'
dataString='Hello\n'

# Open serial port
ser = serial.Serial(PORT, BAUD_RATE)


#sends data to xbee address
def sendData(address, datatosend):
    zb.send('tx', dest_addr_long = address, dest_addr = UNKNOWN, data = datatosend)

def recvData (data):
    print data



zb = ZigBee(ser)


#test data sending method
ack = '0013A2004098DA02'
while True:
    try:
        sendData(WHERE, dataString)
        time.sleep(0)


    except KeyboardInterrupt:
        break

    zb.at(frame_id='A', command='MY')
    reply = zb.wait_read_frame()
    print(reply)


    # print(zb.wait_read_frame())

zb.halt()
ser.close()
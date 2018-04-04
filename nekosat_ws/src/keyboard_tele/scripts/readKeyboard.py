#!/usr/bin/python
import rospy
from std_msgs.msg import *
from pynput import keyboard
rospy.init_node("keyboardControl")
pub = rospy.Publisher('command_topic',String)

command = "0"
print("Press ESC key to exit the program")
class keyboard_controller:
    global command
    def on_press(key):
        global command
        try:
            command = str(key.char)
            if command == "1" or command == "2" or command == "3" or command == "4":
                pub.publish(command)
                print('alphanumeric key {0} pressed'.format(key.char))
        except AttributeError:
            print('special key {0} pressed'.format(key))

    def on_release(key):
        if key == keyboard.Key.esc:
            return False

    # Collect events until released
    with keyboard.Listener(on_press=on_press,on_release=on_release) as listener:
        listener.join()

control = keyboard_controller
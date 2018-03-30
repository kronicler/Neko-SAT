#!/usr/bin/python
import matplotlib.pyplot as plt
from matplotlib import style
import numpy as np
import mpld3
import csv

x = []
pressure = []
temperature = []
humidity = []
# Expand as necessary

def generate():
    # collecting all data from the CSV file
    with open("/home/ttg/catkin_ws/src/control_panel/scripts/html_files/data.csv",'r') as csvfile:
        plots = csv.reader(csvfile, delimiter=',')
        for c,row in enumerate(plots):
            x.append(c)
            pressure.append(row[2])
            temperature.append(row[3])
            humidity.append(row[4])
            #print(row)
    csvfile.close()

    # style.use("ggplot")
    # style.use("fivethirtyeight")
    style.use("bmh")

    #plotting the first graph
    fig1 = plt.figure(1)
    plt.plot(x,temperature)
    plt.xlabel('Readings')
    plt.ylabel('Temperature')
    plt.title('Temperature Graph', size=20)
    mpld3.save_html(fig1,"/home/ttg/catkin_ws/src/control_panel/scripts/html_files/graphs/temp.html")

    fig2 = plt.figure(2)
    plt.plot(x, humidity)  
    plt.xlabel('Readings')
    plt.ylabel('Humidity')
    plt.title('Humidity Graph', size=20)
    mpld3.save_html(fig2,"/home/ttg/catkin_ws/src/control_panel/scripts/html_files/graphs/humid.html")

    fig3 = plt.figure(3)
    plt.plot(x, pressure)
    plt.xlabel('Readings')
    plt.ylabel('Pressure')
    plt.title('Pressure Graph', size=20)
    mpld3.save_html(fig3,"/home/ttg/catkin_ws/src/control_panel/scripts/html_files/graphs/pres.html")

generate()
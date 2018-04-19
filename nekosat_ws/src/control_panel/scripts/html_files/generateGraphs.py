#!/usr/bin/python
import matplotlib.pyplot as plt
from matplotlib import style
import numpy as np
import mpld3
import csv

x = []
humidity = []
heading = []
pressure = []
temperature = []
airQuality = []
dust = []
altitude = []
# Expand as necessary

def generate():
    # collecting all data from the CSV file
    with open("/home/ttg/catkin_ws/src/control_panel/scripts/html_files/datafile.csv",'r') as csvfile:
        plots = csv.reader(csvfile, delimiter=',')
        for c,row in enumerate(plots):
            x.append(c)
            humidity.append(float(row[0]))
            heading.append(float(row[1]))
            pressure.append(float(row[2]))
            temperature.append(float(row[3]))
            airQuality.append(float(row[4]))
            dust.append(float(row[5]))
            altitude.append(float(row[6]))

            #print(row)
    csvfile.close()

    # style.use("ggplot")
    # style.use("fivethirtyeight")
    style.use("bmh")

    #plotting the first graph
    fig1 = plt.figure(1)
    plt.plot(x,humidity)
    plt.xlabel('Readings')
    plt.ylabel('Relative Humidity [Percentage]')
    plt.title('Humidity Graph', size=20)
    mpld3.save_html(fig1,"/home/ttg/catkin_ws/src/control_panel/scripts/html_files/graphs/humid.html")

    fig2 = plt.figure(2)
    plt.plot(x, heading)  
    plt.xlabel('Readings')
    plt.ylabel('Heading [Degrees]')
    plt.title('Heading Graph', size=20)
    mpld3.save_html(fig2,"/home/ttg/catkin_ws/src/control_panel/scripts/html_files/graphs/head.html")

    fig3 = plt.figure(3)
    plt.plot(x, pressure)
    plt.xlabel('Readings')
    plt.ylabel('Pressure [Pa]')
    plt.title('Pressure Graph', size=20)
    mpld3.save_html(fig3,"/home/ttg/catkin_ws/src/control_panel/scripts/html_files/graphs/pres.html")

    fig4 = plt.figure(4)
    plt.plot(x, temperature)
    plt.xlabel('Readings')
    plt.ylabel('Temperature [Degree Celsius]')
    plt.title('Temperature Graph', size=20)
    mpld3.save_html(fig4,"/home/ttg/catkin_ws/src/control_panel/scripts/html_files/graphs/temp.html")

    fig5 = plt.figure(5)
    plt.plot(x, airQuality)
    plt.xlabel('Readings')
    plt.ylabel('Air Quality')
    plt.title('Air Quality Graph', size=20)
    mpld3.save_html(fig5,"/home/ttg/catkin_ws/src/control_panel/scripts/html_files/graphs/air.html")

    fig6 = plt.figure(6)
    plt.plot(x, dust)
    plt.xlabel('Readings')
    plt.ylabel('Dust')
    plt.title('Dust Graph', size=20)
    mpld3.save_html(fig6,"/home/ttg/catkin_ws/src/control_panel/scripts/html_files/graphs/dust.html")

    fig6 = plt.figure(7)
    plt.plot(x, altitude)
    plt.xlabel('Readings')
    plt.ylabel('Altitude')
    plt.title('Altitude Graph', size=20)
    mpld3.save_html(fig6,"/home/ttg/catkin_ws/src/control_panel/scripts/html_files/graphs/alt.html")

generate()
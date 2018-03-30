#!/usr/bin/env python
from flask import Flask
import sys, os
from html_files import generateGraphs

app = Flask(__name__)

@app.route("/stop")
def stop():
	os.system('killall rosbag')
	return 'done!'

@app.route("/start")
def start():
	os.chdir('/root/bags')
	os.system('rosbag record -a &')
	return 'started!'
        
@app.route("/refresh")
def refresh():
    generateGraphs.generate()
    return "ok"

if __name__ == "__main__":
    app.run(debug = True, host='0.0.0.0', port=5000, passthrough_errors=True)
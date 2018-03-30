#!/usr/bin/env python
import rospkg
import os
import SimpleHTTPServer
import SocketServer

rp = rospkg.RosPack()
path = rp.get_path("control_panel")
os.chdir(path+"/scripts/html_files")
Handler = SimpleHTTPServer.SimpleHTTPRequestHandler
httpd = SocketServer.TCPServer(("", 8000), Handler)
httpd.serve_forever()

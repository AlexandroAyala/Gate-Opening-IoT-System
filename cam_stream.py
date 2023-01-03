# Web streaming example
# Source code from the official PiCamera package
# http://picamera.readthedocs.io/en/latest/recipes2.html#web-streaming

import io
import picamera
import logging
import socketserver
from threading import Condition
from http import server
import RPi.GPIO as GPIO
from flask import Flask, request, render_template, url_for
import threading
import cgi

PAGE="""\
<html>
<head>
<title>Raspberry Pi - Surveillance Camera</title>
</head>
<body>
<center><h1>Raspberry Pi Surveillance Camera</h1></center>
<center><img src="stream.mjpg" width="640" height="480"></center>
<center><form method="POST" enctype="multipart/form-data" action="/">
    <input type="text" name="unlock/lock" placeholder="unlock/lock/stop">
    <input type="submit" value="Submit"></form></center>
</body>
</html>
"""
#######
#initialize GPIO
GPIO.setmode(GPIO.BOARD)
GPIO.setwarnings(False)
GPIO.setup(15, GPIO.OUT) #this will connect to the Tiva and tell it to unlock
GPIO.setup(16, GPIO.OUT) # this will tell tiva to lock 
GPIO.setup(13, GPIO.IN)  #this will be PIR sensor 1 for turning on system
GPIO.setup(11, GPIO.IN) #this will be PIR 2 for turning off system
#######

class StreamingOutput(object):
    def __init__(self):
        self.frame = None
        self.buffer = io.BytesIO()
        self.condition = Condition()
    def write(self, buf):
        if buf.startswith(b'\xff\xd8'):
            # New frame, copy the existing buffer's content and notify all
            # clients it's available
            self.buffer.truncate()
            with self.condition:
                self.frame = self.buffer.getvalue()
                self.condition.notify_all()
            self.buffer.seek(0)
        return self.buffer.write(buf)

class StreamingHandler(server.BaseHTTPRequestHandler):
    def do_GET(self):
        if self.path == '/':
            self.send_response(301)
            self.send_header('Location', '/index.html')
            self.end_headers()
        elif self.path == '/index.html':
            content = PAGE.encode('utf-8')
            self.send_response(200)
            self.send_header('Content-Type', 'text/html')
            self.send_header('Content-Length', len(content))
            self.end_headers()
            self.wfile.write(content)
        elif self.path == '/stream.mjpg':
            self.send_response(200)
            self.send_header('Age', 0)
            self.send_header('Cache-Control', 'no-cache, private')
            self.send_header('Pragma', 'no-cache')
            self.send_header('Content-Type', 'multipart/x-mixed-replace; boundary=FRAME')
            self.end_headers()
            try:
                while True:
                    with output.condition:
                        output.condition.wait()
                        frame = output.frame
                    self.wfile.write(b'--FRAME\r\n')
                    self.send_header('Content-Type', 'image/jpeg')
                    self.send_header('Content-Length', len(frame))
                    self.end_headers()
                    self.wfile.write(frame)
                    self.wfile.write(b'\r\n')
            except Exception as e:
                logging.warning(
                    'Removed streaming client %s: %s',
                    self.client_address, str(e))
        else:
            self.send_error(404)
            self.end_headers()
    def do_POST(self):
        if self.path.endswith('/'):
            ctype, pdict = cgi.parse_header(self.headers.get('Content-Type'))
            pdict['boundary'] = bytes(pdict['boundary'], "utf-8")
            content_len = int(self.headers.get('Content-length'))
            pdict['CONTENT-LENGTH'] = content_len
                                      
            if ctype == 'multipart/form-data':
                fields = cgi.parse_multipart(self.rfile, pdict)
                new_val = fields.get('unlock/lock')
                
            self.send_response(301)
            self.send_header('Content-Type', 'text/html')
            self.send_header('Location', '/index.html')
            self.end_headers()
            if(new_val[0] == "unlock"): #this section is reading the user inputs
                print(new_val)
                GPIO.output(15, True)
            elif(new_val[0] == "lock"):
                print(new_val)
                GPIO.output(16, True)
                camera.stop_recording()
            elif(new_val[0] == "stop"):
                camera.stop_recording()
            else:
                print("Error: Invalid Answer")
            #self.wfile.write(encode(new_val))       
         
    
class StreamingServer(socketserver.ThreadingMixIn, server.HTTPServer):
    allow_reuse_address = True
    daemon_threads = True

####might not actually need this section
app = Flask(__name__)

@app.route('/', methods = ['POST', 'GET'])
def index():
    if request.method == 'POST':
        if request.form['unlock/lock'] == 'unlock':
            print("Gate Unlocked")
        elif request.form['unlock/lock'] == 'lock':
            print("Gate Locked")
        else:
            pass
    return render_template('index.html');
###


#if (GPIO.input(13)): #if PIR sensor 1 is triggered
with picamera.PiCamera(resolution='640x480', framerate=24) as camera:
    if(GPIO.input(13)):
        print("Object Detected")
        output = StreamingOutput()
    #Uncomment the next line to change your Pi's Camera rotation (in degrees)
        camera.rotation = 180
    
        camera.start_recording(output, format='mjpeg')
    
        address = ('', 8000)
        server = StreamingServer(address, StreamingHandler)
        server.serve_forever()
        '''threading.Thread(None, server.run).start()
    #Unlock, Lock = buttonpress()
        #my_form_post()
        '''
    elif (GPIO.input(11)):
        print("Object has passed")
        

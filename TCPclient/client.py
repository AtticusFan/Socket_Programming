import socket
import time
import cv2 as cv
HOST = 'localhost'
PORT = 7000
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST, PORT))

while True:
    f= open('Readme.txt', 'rb') #開檔(二進位制)
    outdata = f.read()
    print('Send:\n' + str(outdata))
    s.send(outdata)
    indata = s.recv(1024)
    if len(indata) == 0: #如果server關閉
        s.close()
        print('Server closed connection.')
        break
    print('Recive:' + indata.decode('utf-8'))
    
    time.sleep(5)    #休息5秒後繼續傳送
f.close()
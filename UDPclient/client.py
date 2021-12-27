import socket
import time
HOST = 'localhost'
PORT = 12000
s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

while True:
    f= open('Readme.txt', 'rb')
    message = f.read()
    s.sendto(message, (HOST, PORT))
    data, addr=s.recvfrom(4096)
    if len(data) == 0: #如果server關閉
        s.close()
        print('Server closed connection.')
        break
    print("Server:")
    print(str(data))
    f.close()
    time.sleep(5)
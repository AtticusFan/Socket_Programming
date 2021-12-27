import socket
import time

HOST = 'localhost'
PORT = 7000

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
s.bind((HOST, PORT))
s.listen(5)

print('Server start at: %s:%s' % (HOST, PORT))
print('Wait for connection...')

while True:
    conn, addr = s.accept()
    print('connected by ' + str(addr))

    while True:
        indata = conn.recv(1024)
        if len(indata) == 0: #如果Client關閉
            conn.close()
            print('Client closed connection.')
            break
        print('Recive:\n' + indata.decode('utf-8'))

        outdata = 'Echo\n' + indata.decode('utf-8')
        conn.send(outdata.encode('utf-8'))
import socket
HOST = 'localhost'
PORT = 12000
s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
s.bind((HOST, PORT))
print('Server start at: %s:%s' % (HOST, PORT))
print('Wait for connection...')
while True:
    data,addr=s.recvfrom(4096)
    print('Recive:\n' + str(data))
    '''
    if len(data) == 0: #如果Client關閉
            s.close()
            print('Client closed connection.')
            break
    '''
    mes="Hello client. How's your day?".encode('utf-8')
    s.sendto(mes, addr)
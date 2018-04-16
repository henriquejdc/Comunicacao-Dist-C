import socket
import threading
import time

tLock = threading.Lock()
shutdown = False

def receving(name, sock):
    while not shutdown:
        try:
            tLock.acquire()
            while True:
                data, addr = sock.recvfrom(1024)
                decoded_data = data.decode('utf-8')
                print(decoded_data+'\n')
        except:
            pass
        finally:
            tLock.release()

def past_conv():
    arquivo = open('conversa.txt', 'r')
    conteudo = arquivo.readlines()
    print(conteudo)


host = '127.0.0.1'
port = 0

server = ('127.0.0.1', 5000)

s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
s.bind((host, port))
s.setblocking(0)

receivingThread = threading.Thread(target = receving, args = ("RecvThread", s))
receivingThread.start()

past_conv()
print("Press Enter Before Send Message, message send only with ->")
alias = input("Name: ")
message = 'Connect'
message = (alias + ": " + message)
message = message.encode('utf-8')
s.sendto(message, server)
tLock.acquire()
tLock.release()
time.sleep(0.3)

message = input(alias + "-> ")

while (message != 'Quit'):
    if (message != ''):
        message = (alias + ": " + message)
        message = message.encode('utf-8')
        s.sendto(message, server)
    tLock.acquire()
    tLock.release()
    message = input(alias + "-> ")
    time.sleep(0.3)

message = 'Desconnect'
message = (alias + ": " + message)
message = message.encode('utf-8')
s.sendto(message, server)
shudown = True
receivingThread.join()
s.close()

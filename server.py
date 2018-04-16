import socket
import time

host = '127.0.0.1'
port = 5000

clients = []

s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
s.bind((host, port))
s.setblocking(0)
arquivo = open('conversa.txt', 'r')
conteudo = arquivo.readlines()

quitting = False
print("Server Started.")
while not quitting:
    try:
        data, addr = s.recvfrom(1024)
        decoded_data = data.decode('utf-8')
        conteudo.append(decoded_data +'\n')
        arquivo = open('conversa.txt', 'w')
        arquivo.writelines(conteudo)
        arquivo.close()

        if ("Quit") in str(decoded_data):
            quitting = True

        if addr not in clients:
            print("Client ", addr, "has connected.")
            clients.append(addr)

        print(time.ctime(time.time()), addr, ":", decoded_data)
        time.sleep(0.2)
        for client in clients:
            if addr != client:
                s.sendto(data, client)

    except:
        pass    
s.close()

import socket
import os

def client():
    host = socket.gethostname()
    port = 5000 
    client_socket = socket.socket()
    client_socket.connect((host, port))

    name=input("Enter your name: ")
    client_socket.send(name.encode())

    starter=client_socket.recv(2048).decode()
    print(starter)

    while True:
        path=client_socket.recv(2048).decode()

        sendermessage = input(path) 

        if sendermessage == 'exit':
            break
        elif sendermessage == 'clear':
            client_socket.send(sendermessage.encode()) 
            os.system('cls')
            continue
        else:    
            client_socket.send(sendermessage.encode()) 

        received = client_socket.recv(2048).decode()
        print(received)

    #client_socket.close()

if __name__ == '__main__':
    client()
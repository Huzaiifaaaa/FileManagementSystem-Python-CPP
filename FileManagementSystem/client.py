import socket

def client():
    host = socket.gethostname()
    port = 5000 
    client_socket = socket.socket()
    client_socket.connect((host, port))

    starter=client_socket.recv(1024).decode()
    print(starter)

    while True:
        path=client_socket.recv(1024).decode()

        sendermessage = input(path) 
        client_socket.send(sendermessage.encode()) 

        received = client_socket.recv(1024).decode()
        #print(data)

    client_socket.close()

if __name__ == '__main__':
    client()
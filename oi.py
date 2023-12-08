import socket

client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client.connect(('194.210.177.59', 50000))

while True:
    message = input("Enter a message: ")
    client.send(message.encode('utf-8'))
    data = client.recv(1024)
    print(data.decode('utf-8'))
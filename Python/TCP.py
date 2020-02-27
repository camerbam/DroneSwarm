import socket

TCP_IP = "127.0.0.1"
TCP_PORT = 65000
BUFFER_SIZE = 1024
messages = ['{"msgId":"referee:3","type":"base","msg":{"gameId":4}}']

sock = socket.socket(socket.AF_INET, # Internet
                     socket.SOCK_STREAM) # TCP

sock.connect((TCP_IP, TCP_PORT))

for message in messages:
  m = bytearray(str(len(message)).zfill(5) + message)
  m = hex(1) +m
  sock.send(m)

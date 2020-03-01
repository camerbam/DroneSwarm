import socket
import json
from Crypto.PublicKey import RSA
from Crypto.Cipher import PKCS1_v1_5 as Cipher_PKCS1_v1_5
import binascii

def createMsg(message):
  m = bytearray('\x01')
  l = str(len(message)).zfill(5)
  m.extend(l)
  m.extend(message)
  print binascii.hexlify(m)
  return m

def createEncryptedMessage(message, key):
  a = createMsg(key.encrypt(message))
  return a

TCP_IP = "127.0.0.1"
TCP_PORT = 65000
BUFFER_SIZE = 1024
messages = [
  '{"msgId":"referee:3","type":"ReadyMsg","msg":{}}',
  '{"msgId":"referee:3","type":"ReadyRspRsp","msg":{"gameId":4}}',
  '{"msgId":"referee:3","type":"HitTargetMsg","msg":{"gameId":2,"id":1,"target":{"x":2,"y":3}}}',
  '{"msgId":"referee:3","type":"FinishMsg","msg":{"gameId":4}}'
  ]

sock = socket.socket(socket.AF_INET, # Internet
                     socket.SOCK_STREAM) # TCP

sock.settimeout(10)

sock.connect((TCP_IP, TCP_PORT))
key = sock.recv(2048)
print key
key = key.split("END RSA PUBLIC KEY-----\n")
key = RSA.importKey(key[0])
cipher = Cipher_PKCS1_v1_5.new(key)

sock.settimeout(10)

for message in messages:
  received = ""
  i = 0
  while i < 3 and len(received) == 0:
    a = createEncryptedMessage(message, cipher)
    sock.send(a)
    try:
      received = key.decrypt(sock.recv(1024))
      if "PingMsg" in received:
        sock.send(createEncryptedMessage('{"msgId":"referee:3","type":"PingRsp","msg":{"gameId":4}}', cipher))
    except:
      i = i + 1
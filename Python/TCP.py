import socket
import json
from Crypto.PublicKey import RSA
from Crypto.Cipher import PKCS1_v1_5 as Cipher_PKCS1_v1_5
import binascii
import time

def createMsg(message):
  m = bytearray('\x01')
  l = str(len(message)).zfill(5)
  m.extend(l)
  m.extend(message)
  return m

def createEncryptedMessage(message, key):
  return createMsg(key.encrypt(message))

TCP_IP = "127.0.0.1"
TCP_PORT = 65000
BUFFER_SIZE = 1024
messages = [
  '{"msgId":"referee:3","type":"ReadyMsg","msg":{}}',
  '{"msgId":"referee:3","type":"ReadyRspRsp","msg":{"gameId":4}}',
  '{"msgId":"referee:3","type":"HitTargetMsg","msg":{"gameId":2,"id":1,"target":{"x":2,"y":3}}}'
  ]

sock = socket.socket(socket.AF_INET, # Internet
                     socket.SOCK_STREAM) # TCP

sock.connect((TCP_IP, TCP_PORT))
key = sock.recv(2048)
key = key.split("END RSA PUBLIC KEY-----\n")
key = RSA.importKey(key[0])
cipher = Cipher_PKCS1_v1_5.new(key)

for message in messages:
  sock.send(createEncryptedMessage(message, cipher))
  recv = sock.recv(2048)

sock.send(createEncryptedMessage('{"msgId":"referee:3","type":"PingRsp","msg":{"gameId":4}}', cipher))
sock.send(createEncryptedMessage('{"msgId":"referee:3","type":"FinishMsg","msg":{"gameId":4}}', cipher))
recv = sock.recv(2048)

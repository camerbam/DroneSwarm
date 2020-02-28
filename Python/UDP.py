import socket

UDP_IP = "127.0.0.1"
UDP_PORT = 8882
messages = ["command",
  "back 20",
  "battery?",
  "cw 20",
  "ccw 20",
  "down 20",
  "flip b",
  "forward 20",
  "go 20 30 40 50",
  "left 20",
  "mdirection 0",
  "mon",
  "moff",
  "right 20",
  "speed?",
  "takeoff",
  "time?",
  "up 20",
  "land"]

sock = socket.socket(socket.AF_INET, # Internet
                     socket.SOCK_DGRAM) # UDP

sock.settimeout(10)

for message in messages:
  i = 0
  data = ''
  while i < 3 and len(data) != 2:
    sock.sendto(message, (UDP_IP, UDP_PORT))
    try:
      data, addr = sock.recvfrom(1024) # buffer size is 1024 bytes
    except:
      i = i + 1


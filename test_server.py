import socket
import numpy
import cv2

def recvall(sock, count):
    buf = b''
    while count:
        newbuf = sock.recv(count)
        if not newbuf: return None
        buf += newbuf
        count -= len(newbuf)
    return buf

server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.bind(('127.0.0.1', 9001))
server.listen(1)
conn, addr = server.accept()
conn.send("hello".encode())
length = recvall(conn, 16)
stringData =recvall(conn, int(length))
endMark = stringData.split(b'\n\b\n\b')[-1].decode()
print(endMark)
close()

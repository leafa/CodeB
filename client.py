import socket
import sys
    
def run(*commands):
    data="Z2 0312\n" + "\n".join(commands) + "\nCLOSE_CONNECTION\n"
    try:
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.connect(("codebb.cloudapp.net", 17429))
        sock.sendall(data)
        sfile = sock.makefile()
        rline = sfile.readline()
        while rline:
            print(rline.strip())
            rline = sfile.readline()
    finally:
        sock.close()

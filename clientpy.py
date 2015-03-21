import socket
import sys
import Tkinter

stocks = []

def run(*commands):
    data="Z2 0312\n" + "\n".join(commands) + "\nCLOSE_CONNECTION\n"
    try:
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.connect(("codebb.cloudapp.net", 17429))
        sock.sendall(data)
        sfile = sock.makefile()
        result = sfile.read()
        # while rline:
        #     print(rline.strip())
        #     rline = sfile.readline()
    finally:
        sock.close()
        
    return result;

def update_stocks():
    res = run("SECURITIES").split()
    if res[0] != "SECURITIES_OUT":
        print("Did not get stocks information from server")
        return

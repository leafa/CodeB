import socket
import sys

def run(*commands):
    data="Z2 0312\n" + "\n".join(commands) + "\nCLOSE_CONNECTION\n"
    try:
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.connect(("codebb.cloudapp.net", 17429))
        sock.sendall(data)
        sfile = sock.makefile()
        result = sfile.read()
    finally:
        sock.close()
        
    return result;

def update_stocks():
    res = run("SECURITIES").split()
    if res[0] != "SECURITIES_OUT":
        print("Did not get stocks information from server")
        return
    return res

def main():
    

if __name__ == '__main__':
    main()
    


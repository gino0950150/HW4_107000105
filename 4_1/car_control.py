import time
import serial
import sys,tty,termios

class _Getch:
    def __call__(self):
        fd = sys.stdin.fileno()
        old_settings = termios.tcgetattr(fd)
        try:
            tty.setraw(sys.stdin.fileno())
            ch = sys.stdin.read(1)
        finally:
            termios.tcsetattr(fd, termios.TCSADRAIN, old_settings)
        return ch
def get():
    d1 = input()
    s.write(d1.encode())
    s.write(" ".encode())        
    d2 = input()
    s.write(d2.encode())
    s.write(" ".encode())
    dir = input()
    s.write(dir.encode())      
    s.write("\n".encode())        
    return 1

s = serial.Serial(sys.argv[1])

while get():
    i = 0
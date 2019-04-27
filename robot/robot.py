import socket
import time
import threading
import select

UDP_IP = '192.168.4.1'
UDP_PORT = 33146
send_sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)


def send_message(msg):
    send_sock.sendto(bytes(msg, "utf-8"), (UDP_IP, UDP_PORT))


def check_ip():
    addrs=socket.gethostbyname_ex(socket.gethostname())
    addrs=addrs[2]
    for a in addrs:
        if a.startswith('192.168.4.'):
            return True
    return False


class Robot:
    def __init__(self):
        self.done=False
        self.recv_thread=None
        self.sensor=-1
        if not check_ip():
            print("Not connected to robot")
            self.done=True
            return
        
    def cleanup(self):
        self.stop()
        time.sleep(0.1)
        self.done=True
        self.recv_thread.join()
        time.sleep(0.1)
        
    def __enter__(self):
        self.recv_thread=threading.Thread(target=self.udp_recv)
        self.recv_thread.start()
        return self
        
    def __exit__(self, exc_type, exc_val, exc_tb):
        self.cleanup()
        
    def udp_recv(self):
        recv_sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        recv_sock.bind(("",UDP_PORT))
        while not self.done:
            rd,wr,x=select.select([recv_sock],[],[],1.0)
            if len(rd)>0:
                msg,addr=recv_sock.recvfrom(100)
                msg=msg.decode('utf-8')
                try:
                    self.sensor=int(msg)
                except ValueError:
                    self.sensor=-1
        recv_sock.close()
        
        
    def drive(self,left,right):
        send_message('D {} {}'.format(left,right))
        
    def stop(self):
        self.drive(0,0)
        
    def step(self,left,left_ticks,right,right_ticks):
        send_message('S {} {} {} {}'.format(left,left_ticks,right,right_ticks))
    
    def lidar(self,state):
        send_message('L {}'.format(state))
        
    def servo(self,value):
        if value<-1:
            value=-1
        if value>1:
            value=1
        send_message('R {}'.format(int(175*value+375)))

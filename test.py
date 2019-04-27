from robot.robot import Robot
import time

with Robot() as r:
    r.lidar(1)
    r.servo(0)
    for i in range(100):
        r.step(1000,50,1000,50)
        for j in range(100):
            print(r.sensor)
            time.sleep(0.1)
        break
        r.drive(800,800)
        print(r.sensor)
        if 0 < r.sensor < 300:
            r.stop()
            r.lidar(0)
            time.sleep(0.5)
            r.drive(1000,-1000)
            time.sleep(1.6)
            r.stop()
            break
        time.sleep(0.1)
    r.stop()

import serial
import syslog
import time

#The following line is for serial over GPIO
# port = '/dev/ttyS0'
port = '/dev/tty.wchusbserial1420'
baud = 115200

ard = serial.Serial(port,115200,timeout=5)
time.sleep(2) #wait for Arduino

start_t = time.time()
with open("data_logger/output.csv", mode="w+") as out_file:
    out_file.truncate(0)
    out_file.write("pwm, D/Vp, D/Vr, y_po, p_po, r_po, state, yaw, pitch, roll\n")
    while(time.time() - start_t < 10):
	    # Serial read section
        cur_count = ard.inWaiting()
        if(cur_count != 0):
            msg_b = ard.read(cur_count) # read all characters in buffer
            msg_s = msg_b.decode("ascii")
            if(len(msg_s) > 10): # 6
                print (msg_s)
                out_file.write(msg_s)
print("End of programme")
    #separate errors into list [timestamp, force error, current force error, velocity error, current velocity error]
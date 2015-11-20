import time
import numpy as np
import matplotlib.pyplot as plt
import array
import timeit
import math

D = 0.00635;
h1 = 0.02;
h2 = 0.03;
g = 9.8;
Ki = 4;
Kp = 1;
Kd = 0;
L = 0.02;
#p = 1000;
size = 1;
phi = 0.95
#u = 8.90 * pow(10,-4)
#Q2=array.array('f',(0,)*size)
Q2 = [0.0]
#print "Q2", Q2
T = []
T += Q2
A = 0.0054;
V = 0.000162;

#C = pid(Kp, Ki, Kd)
#d = 1193.8* pow(levelSetpoint,-0.905);
t=0;
i=1;
T[0]=0;
VolOfStone = 0;
tic = timeit.default_timer()



#print T
#print Q2


while h2 >= h1:
#    %     prompt = 'What is the current level of water?';
#    %     h2 = input(prompt);
    toc = timeit.default_timer()
    T.extend([toc-tic])
#    print "T: ", T
    Q2.extend([(math.pi*pow(D, 2)/4)*phi*math.sqrt(2*g*(h2))])
    V = V - Q2[i]*(T[i]-T[i-1])
    h2 = V/A;
    VolOfStone = VolOfStone + Q2[i]*(T[i]-T[i-1])
    print('T: ', T[i])
    print('Q: ', Q2[i])
    print("\n") 
    i = i+1

    

Volume = VolOfStone
print('Total Volume: ', Volume)
time = toc;


plt.figure()
plt.plot( T[:], Q2[:])
plt.xlabel('time(s)')
plt.ylabel('Net Flow Rate')
plt.savefig('div2.png',dpi=300, bbox_inches='tight')
plt.show()


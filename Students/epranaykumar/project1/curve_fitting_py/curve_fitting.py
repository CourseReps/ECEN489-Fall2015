import sqlite3
import matplotlib.pyplot as plt
import numpy as np
from scipy.optimize import curve_fit

def func(x, a, b):
     return a *  x + b


conn = sqlite3.connect('./data.db')
c = conn.cursor()

c.execute('SELECT Voltage, Flowrate FROM DATA ORDER BY Timestamp')

all_rows = c.fetchall()


conn.commit()
conn.close()

x=[]
y=[]
for i in range(0,len(all_rows)):

    x.append(int(all_rows[i][0]))
    y.append(float(all_rows[i][1]))

    #if(x[i]==9):
       # plt.plot(x,y, 'ro')

par = curve_fit(func,x, y)
print(par[0][0],par[0][1])

ydata = func(x,par[0][0],par[0][1])


plt.plot(x[0:7],y[0:7], 'r^') #'bs', t, t**3, 'g^'
plt.plot(x[8:15],y[8:15], 'g^')
plt.plot(x[16:23],y[16:23], 'b^')
plt.plot(x[24:31],y[24:31], 'ro')
plt.plot(x[32:39],y[32:39], 'go')
plt.plot(x[40:47],y[40:47], 'bo')

plt.plot(x, ydata, 'k')
plt.axis([0, 10, 0, 12])
plt.show()


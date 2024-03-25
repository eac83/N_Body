import numpy as np
import matplotlib.pyplot as plt
from math import sqrt

YEAR = 60*60*24*365
TIME_STEP = YEAR * 3.79e7
TIME_END = YEAR * 13.8e9
OMEGA_M = 0.3153
OMEGA_LAMBDA = 0.6847
H_0 = 67.4 / 3.0857e19
a = 1/(1089+1)
time = 0#YEAR * 3.79e5
a_e = a
a_rk = a
a_es = [a]
a_rks = [a]
times = []
times.append(time)

def E(a):
    return OMEGA_M/a + OMEGA_LAMBDA*(a**2)

while a_rk<1:
    time += TIME_STEP
    k_1 = H_0 * sqrt(E(a_rk))
    k_2 = H_0 * sqrt(E(a_rk + TIME_STEP*k_1/2))
    k_3 = H_0 * sqrt(E(a_rk + TIME_STEP*k_2/2))
    k_4 = H_0 * sqrt(E(a_rk + TIME_STEP*k_3))
    a_rk += TIME_STEP/6 * (k_1 + 2*k_2 + 2*k_3 + k_4)
    a_rks.append(a_rk)
    times.append(time)
print(time/(YEAR*1e9))
#plt.plot(np.array(times)/YEAR, a_es, label="Euler")
plt.plot(np.array(times)/YEAR, a_rks, label="RK4")
#plt.legend()
#plt.show()
#a_ratio = [a_rk/a_e for a_rk, a_e in zip(a_rks, a_es)]
#plt.plot(np.array(times)/YEAR, 1-np.array(a_ratio))
plt.show()
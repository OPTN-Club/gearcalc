import matplotlib.pyplot as plt
import numpy as np



#consts
WHEEL_DIAMETER = 25
PI = 3.14






rpms = range(0, 12000)
wheelspeeds = [(WHEEL_DIAMETER*rpm)/63360*60*PI for rpm in rpms]

#equation for torque curve, this is approximated by a polynomial
#TODO replace with real data inputs
torque_curve = [221 + -1.74*rpm + 3.33*rpm^2 + -0.266*rpm^3 for rpm in rpms]


fig, ax = plt.subplots()
ax.plot(rpms, torque_curve)

ax.set(xlabel='rpm', ylabel='torque')
ax.grid()

plt.show()

import matplotlib.pyplot as plt
import numpy as np
from bokeh.io import show, save, output_file
from bokeh.models import Slider, ColumnDataSource, CustomJS
from bokeh.plotting import figure
from bokeh.layouts import row, column
from bokeh.palettes import Paired12 as palette

# Specify a file to write the plot to
output_file("index.html")


#consts
WHEEL_DIAMETER = 25
PI = 3.14
RPM_RANGE = range(0, 12000)


#input data
final_drive = 3.9
gear_ratios = [2.7, 2.11, 1.7, 1.39, 1.16, .99, .85, 0.001, 0.001, .001]
#equation for torque curve, this is approximated by a polynomial
#TODO replace with real data inputs
torque_curve = [221 + -1.74*(rpm/1000) + 3.33*(rpm/1000)**2 + -0.266*(rpm/1000)**3 for rpm in RPM_RANGE]


wheelspeeds = [(WHEEL_DIAMETER*rpm)/63360*60*PI for rpm in RPM_RANGE]


power_curve = [torque_curve[rpm] * rpm/5252 for rpm in RPM_RANGE]

# Make the plot
p = figure(plot_height=400, plot_width=1200, width_policy='fixed', title="Gear Ratio Tuner")
p.axis.visible = False
source = None

slider = []
for idx, gear in enumerate(gear_ratios):
    gear_adjusted_wheel_speeds = [wheelspeed/(gear*final_drive) for wheelspeed in wheelspeeds if gear>0.01]
    gear_adjusted_torque = [torque*(gear*final_drive) for torque in torque_curve if gear>0.01]
    source = ColumnDataSource(data=dict(x=gear_adjusted_wheel_speeds, y=gear_adjusted_torque))
    line = p.line('x','y', source=source, color=palette[idx], line_width=3)

    callback = CustomJS(args=dict(source=source, wheelspeeds=wheelspeeds, torque_curve=torque_curve, final_drive=final_drive), code="""
        const data = source.data;
        const f = cb_obj.value
        const x = data['x']
        const y = data['y']
        if(f>0.01){

        for(let wheelspeed in wheelspeeds){
            x[wheelspeed] = wheelspeeds[wheelspeed]/(f*final_drive);
        }
        for(let torque in torque_curve){
            y[torque] = torque_curve[torque]*f*final_drive;
        }
        source.change.emit();
        }
        else{
        for(let wheelspeed in wheelspeeds){
            x[wheelspeed] = 0;
        }
        for(let torque in torque_curve){
            y[torque] = 0;
        }
        source.change.emit();
        }
    """)
    slider.append(Slider(start=0, end=10, value=gear, step=.01, title=f"Gear {idx+1}"))
    slider[idx].js_on_change('value', callback)

layout = column(*slider, p)


save(layout)

import matplotlib.pyplot as plt
import numpy as np
from bokeh.io import show, save, output_file
from bokeh.models import Slider, ColumnDataSource, CustomJS
from bokeh.plotting import figure
from bokeh.layouts import row, column
from bokeh.palettes import Paired12 as palette

# Specify a file to write the plot to
output_file("blank2.html")

p = figure(plot_height=400, plot_width=1200, width_policy='fixed', title="Gear Ratio Tuner")
line = p.line([1],[1], color=palette[0], line_width=3)
save(p)

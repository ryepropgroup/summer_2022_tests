import matplotlib.pyplot as matlab
import numpy

file_dir = "ignition test fixed data 2022-06-24 20-05-56.txt"
img_size = (11.75, 8.25) #size of A4 paper in inches
img_dpi = 600

data = numpy.genfromtxt(file_dir, dtype = [float, float, float], names = ['time', 'temperature', 'voltage'], delimiter = ',', skip_header = 1)

time_0_index = data['time'][numpy.argmax(data['voltage']>0.00)]
data['time'] = (data['time']-time_0_index)/1000000

fig, ax1 = matlab.subplots()
fig.set_size_inches(img_size)
ax1.set_title("Igniter Measurements vs Time")
ax1.grid(axis ='both', which = 'major', linestyle = 'solid')
ax1.grid(axis = 'both', which = 'minor', linestyle = 'dotted')
ax1.set_xticks(numpy.arange(-5, 20, 5), which = 'major')
ax1.minorticks_on()
ax2 = ax1.twinx()

ax1.set_xlabel('Time (s)')
ax1.set_xlim(-5, 20)
ax1.set_ylabel('Flame Temperature (C)', color = 'orange')
ax1.tick_params(axis = 'y', colors = 'orange')
ax1.spines['left'].set_color('orange')
ax1.spines['right'].set_visible(False)
ax1.set_ylim(-50,1400)
ax1.plot(data['time'], data['temperature'], color = 'orange', marker = None)

ax2.set_ylabel("Voltage (V)", color = 'blue')
ax2.tick_params(axis ='y', colors = 'blue')
ax2.spines['right'].set_color('blue')
ax2.spines['left'].set_visible(False)
ax2.set_ylim(-0.25,7)
ax2.plot(data['time'], data['voltage'], color = 'blue', marker = None)
ax2.grid(False)

#matlab.get_current_fig_manager().full_screen_toggle()
matlab.savefig("igniter_graph.png", bbox_inches = 'tight', dpi = img_dpi)

time = (2.5, 10)
temp = (1020, 1380)
volt_per_temp = 7/1400
ax1.set_xticks(numpy.arange(-5, 20, 1), which = 'major')
ax1.set_xlim(time)
ax1.set_ylim(temp)
ax2.set_ylim(t*volt_per_temp for t in temp)
matlab.savefig("igniter_graph_zoom_temp.png", bbox_inches ='tight', dpi = img_dpi)

time = (-0.1, 5.1)
volt = (2.1, 5.1)
temp_per_volt = 1400/7
ax2.set_ylim(volt)
ax1.set_xticks(numpy.arange(-5, 20, 1), which = 'major')
ax1.set_xlim(time)
ax1.set_ylim(v*temp_per_volt for v in volt)
matlab.savefig("igniter_graph_zoom_volt.png", bbox_inches ='tight', dpi = img_dpi)

matlab.show()
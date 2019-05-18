import numpy as np
import matplotlib.pyplot as plt


def lineplot(x_data, y_data, x_label="testx", y_label="testy", title="test"):
    # Create the plot object
    _, ax = plt.subplots()    # Plot the best fit line, set the linewidth (lw), color and
    # transparency (alpha) of the line
    ax.plot(x_data, y_data, lw = 2, color = '#539caf', alpha = 1)    # Label the axes and provide a title
    ax.set_title(title)
    ax.set_xlabel(x_label)
    ax.set_ylabel(y_label)


data1 = []
data2 = []
with open('test', 'r', encoding='utf-8') as f:
    step = 0
    while True:
        lines = f.readline()
        step += 1
        if step == 2001:
            step = 0
            continue
        if not lines:
            break
        temp1, temp2, _, _, _ = lines.split(',')
        data1.append(float(temp1))
        data2.append(float(temp2))
lineplot(range(0, 40000, 100), data1[:40000:100])

import numpy as np
import matplotlib.pyplot as plt

def add_plot(ax, file, start=0, stop=0, breathing_scale=1):
    with open(file) as csvfile:
        print("loading: ", file)
        data = np.genfromtxt(csvfile, delimiter=',')

        if start > data.shape[0]-10:
            start = data.shape[0]-10

        if stop > data.shape[0] or stop == 0:
            stop = data.shape[0]

        current = data[start:stop, 0]
        breathing = data[start:stop, 1]
        ax.set_title(file)
        ax.plot(current, label='Current')
        ax.plot(breathing*breathing_scale, label='Breathing')
        ax.legend(loc='upper right')

def main():
    start = 100
    stop = 3000
    breathing_scale = 20
    print("plotting range: ", start, stop)

    f, (ax1, ax2, ax3) = plt.subplots(3, 1)
    add_plot(ax1, "../data/20200317/cpap_spd90_breathing.csv", start, stop, breathing_scale)
    add_plot(ax2, "../data/20200317/bipap_spd90_breathing_sync.csv", start, stop, breathing_scale)
    add_plot(ax3, "../data/20200317/bipap_spd90_breathing_async.csv", start, stop, breathing_scale)
    plt.show()

if __name__ == '__main__':
    main()

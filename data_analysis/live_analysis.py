import numpy as np
import plotting
import cv2
import sys

MAX_INT = 100000

PLOT_TV_MAX = 1000
PLOT_PRESS_MAX = 30
PLOT_X = 500
VOLUME_SCALER = 0.0018


def print_stats(idx, rate, tv, ipap, peep):
    print("idx: {}\trate: {:.2f}/min\ttv: {:.2f}mL\tipap: {:.2f}cm\tpeep: {:.2f}cm".format(idx, rate, tv, ipap, peep))


def alarm(msg, v=None):
    if v:
        print("ALARM: {} {:.2f}".format(msg, v))
    else:
        print("ALARM: {}".format(msg))


def main():

    vol_leakage_warning = 2000
    vol_max = MAX_INT
    vol_max_idx = 0
    vol_max_ts = 0
    vol_min = -MAX_INT
    vol_min_idx = 0
    vol_min_ts = 0
    vol_tidal = 0
    vol_tidal_smoothing = 0.5
    ipap = 0
    peep = 0
    rate = 0
    press_max = 0
    press_min = MAX_INT
    rate_smoothing = 0.5
    backup_rate = 8
    backup_rate_dt = 60/backup_rate

    inspiratory_ts = MAX_INT
    expiratory_ts = MAX_INT
    expiratory_vol = MAX_INT
    expiratory_vol_prev = MAX_INT
    leak_est = 0

    inspiratory = False

#    csvfile = "../data/20200331/time_volume_pressure.csv"
    csvfile = "../data/20200331/tvp_bipap.csv"
    data = np.genfromtxt(csvfile, delimiter=',')
    data[:, 1] *= VOLUME_SCALER

    plot = plotting.LinePlot("plots", ["time", "volume", "pressure"], 2, 800, 400, -PLOT_TV_MAX / 5, PLOT_TV_MAX)
    pv_plot = plotting.Plot2D("PV", 500, 500, -PLOT_PRESS_MAX / 10, PLOT_PRESS_MAX, -PLOT_TV_MAX / 5, PLOT_TV_MAX)

    v = 0
    v_avg = 0
    v_smoothing = 0.98
    i = 8300

    while True:
        v_prev = v
        ts = data[i, 0]
        v = data[i, 1]
        p = data[i, 2]

        if p > press_max:
            press_max = p
        if p < press_min:
            press_min = p
        if v > vol_max:
            vol_max = v
            vol_max_ts = ts
            vol_max_idx = plot.x
        if v < vol_min:
            vol_min = v
            vol_min_ts = ts
            vol_min_idx = plot.x

        v_avg = v_avg*v_smoothing + (1-v_smoothing)*v

        # warnings
        if expiratory_vol != MAX_INT:
            if v-expiratory_vol > vol_leakage_warning:
                alarm("Leakage Warning:", v-expiratory_vol)

        # detect breathing state
        inspiratory_prev = inspiratory

        # some histeresis
#        if (v > v_avg) and (v_prev < v_avg):  # inhale start condition
        if v > v_prev:  # inhale start condition
            inspiratory = True
#        if (v < v_avg) and (v_prev > v_avg):  # exhale start condition
        if v < v_prev:  # exhale start condition
            inspiratory = False

        # been too long since breathing, force transition
        if (ts - expiratory_ts) > backup_rate_dt:
            print("forcing inspiratory cycle: {:.2f}s".format(ts - expiratory_ts))
            inspiratory = True
            vol_min_ts = ts
        if (ts - inspiratory_ts) > backup_rate_dt:
            print("forcing expiratory cycle:{:.2f}s".format(ts - inspiratory_ts))
            inspiratory = False
            vol_max_ts = ts

        # process measurements
        if inspiratory and not inspiratory_prev:  # inhale start event
 #           plot.mark((255, 255, 0))
            peep = press_min
            press_min = MAX_INT

            if vol_min != -MAX_INT:
                plot.point(vol_min_idx, vol_min, (255, 255, 0))
                # calculate breathing rate
                expiratory_ts_prev = expiratory_ts
                expiratory_ts = vol_min_ts
                expiratory_vol_prev = expiratory_vol
                expiratory_vol = vol_min
                if expiratory_vol_prev != MAX_INT:
                    leak_est = expiratory_vol-expiratory_vol_prev
                if expiratory_ts_prev != 0:
                    dt = expiratory_ts - expiratory_ts_prev
                    rate = rate*rate_smoothing + (1-rate_smoothing)*(60/dt)
                print_stats(i, rate, vol_tidal-leak_est/2, ipap, peep)
            vol_max = -MAX_INT

        if not inspiratory and inspiratory_prev:  # exhale start event
#            plot.mark((0, 255, 255))
            ipap = press_max
            press_max = 0
            if vol_max != MAX_INT:
                plot.point(vol_max_idx, vol_max, (0, 255, 255))
            if (vol_max != MAX_INT) and (vol_min != -MAX_INT):
                vol_tidal = vol_tidal_smoothing*vol_tidal + (1-vol_tidal_smoothing)*(vol_max - vol_min)
                inspiratory_ts = vol_max_ts
                print_stats(i, rate, vol_tidal-leak_est/2, ipap, peep)
            vol_min = MAX_INT

        # pressure volume plot
        if expiratory_ts != MAX_INT:
            pv_plot.lineTo(p, v-expiratory_vol)
            font_size = 1.0
            pv_plot.mark_line(ipap, name="IPAP", color=(0, 0.5, 0.5), size=font_size, axis=1, show_value=True)
            pv_plot.mark_line(peep, name="PEEP", color=(0, 0.5, 0.5), size=font_size, axis=1, show_value=True)
            value_text = "{:.0f}mL".format(vol_tidal-leak_est/2)
            pv_plot.mark_line(vol_tidal+leak_est/2, name="TV Est", color=(0.5, 0.5, 0.0), size=font_size, axis=0,  name2=value_text)
            value_text = "{:.0f}mL".format(leak_est)
            pv_plot.mark_line(leak_est/2, name="Leak Est", color=(0.25, 0.25, 0.0), size=font_size, axis=0, name2=value_text)

            if v-expiratory_vol > vol_leakage_warning:
                cv2.putText(pv_plot.img, "ALARM:", (20, 100), cv2.FONT_HERSHEY_PLAIN, 3, (0,0,1), thickness=3)
                cv2.putText(pv_plot.img, "Leakage Warning", (20, 100 +60), cv2.FONT_HERSHEY_PLAIN, 3, (0,0,1), thickness=3)

        pv_plot.show()

        # line plots
        plot.add([v-expiratory_vol, p*20])
        plot.show()
        c = cv2.waitKey(10) & 0xFF
        if c == 27:
            break
        if c == ord(' '):
            print("reset")
            i = 0

        i += 1
        if i >= data.shape[0]:
            i = 0


if __name__ == '__main__':
    main()

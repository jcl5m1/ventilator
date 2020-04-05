import numpy as np
import plotting
import cv2
import serial

MAX_INT = 100000
VOLUME_LEAKAGE_WARNING = 2000
PLOT_TV_MAX = 1000
PLOT_PRESS_MAX = 30
PLOT_X = 500
VOLUME_SCALER = 0.0018
MINIMUM_VOLUME_BREATH = 10  # prevents noise when idle
VOL_TIDAL_SMOOTHING = 0.5
RATE_SMOOTHING = 0.5
INIT_BACKUP_RATE = 8

USE_LOG_FILE = True

#    LOG_FILE = "../data/20200331/time_volume_pressure.csv"
LOG_FILE = "../data/20200331/tvp_cpap.csv"




def alarm(msg, v=None):
    if v:
        print("ALARM: {} {:.2f}".format(msg, v))
    else:
        print("ALARM: {}".format(msg))


def serial_test():
    ser = serial.Serial("COM4", 57600)
    print(ser.name)
    for i in range(100):
        try:
            parts = ser.readline().decode('ascii').split(',')
        except Exception as x:
            print(x)
            continue
        if len(parts) < 2:
            continue
        ts = float(parts[0])
        vol = int(parts[1])
        press = float(parts[2])
        print(ts, vol, press)


def serial_parse(ser):
    try:
        parts = ser.readline().decode('ascii').split(',')
    except Exception as x:
        print(x)
        return None
    if len(parts) < 2:
        print("Not enough columns of data: ", parts)
        return None
    ts = float(parts[0])
    vol = int(parts[1])
    press = float(parts[2])

    return ts, vol, press


def file_parse(file):
    try:
        parts = file.readline().split(',')
    except Exception as x:
        print(x)
        return None

    if parts[0] == '':
        file.seek(0)
        print("End of File.  Restarting")
        return None

    if len(parts) < 2:
        print("Not enough columns of data: ", parts)
        return None
    ts = float(parts[0])
    vol = int(parts[1])
    press = float(parts[2])

    return ts, vol, press


class Ventilator:
    def __init__(self):
        self.vol_leakage_warning = VOLUME_LEAKAGE_WARNING
        self.vol_max = MAX_INT
        self.vol_max_idx = 0
        self.vol_max_ts = 0
        self.vol_min = -MAX_INT
        self.vol_min_idx = 0
        self.vol_min_ts = 0
        self.vol_tidal = 0
        self.v_prev = 0
        self.ppeak = 0
        self.peep = 0
        self.rate = 0
        self.press_max = 0
        self.press_min = MAX_INT
        self.backup_rate = INIT_BACKUP_RATE
        self.backup_rate_dt = 60 / self.backup_rate

        self.inspiratory_ts = MAX_INT
        self.expiratory_ts = MAX_INT
        self.expiratory_ts_prev = MAX_INT
        self.expiratory_vol = 0
        self.expiratory_vol_prev = 0
        self.leak_est = 0

        self.inspiratory = False
        self.inspiratory_prev = False
        self.ts = 0
        self.ts_prev = 0

        self.reset()

    def reset(self):
        self.vol_leakage_warning = VOLUME_LEAKAGE_WARNING
        self.vol_max = -MAX_INT
        self.vol_max_idx = 0
        self.vol_max_ts = 0
        self.vol_min = MAX_INT
        self.vol_min_idx = 0
        self.vol_min_ts = 0
        self.vol_tidal = 0
        self.v_prev = 0
        self.ppeak = 0
        self.peep = 0
        self.rate = 0
        self.press_max = 0
        self.press_min = MAX_INT
        self.backup_rate = INIT_BACKUP_RATE
        self.backup_rate_dt = 60 / self.backup_rate

        self.inspiratory_ts = MAX_INT
        self.expiratory_ts = MAX_INT
        self.expiratory_ts_prev = MAX_INT
        self.expiratory_vol = 0
        self.expiratory_vol_prev = 0
        self.leak_est = 0

        self.inspiratory = False
        self.inspiratory_prev = False
        self.ts = 0
        self.ts_prev = 0

    def print_report(self):
        print(
            "ts: {}\tRate: {:.2f}/min\tTV Est: {:.2f}mL\tPpeak: {:.2f}cm\tPEEP: {:.2f}cm".format(
                self.ts, self.rate, self.vol_tidal - self.leak_est / 2, self.ppeak, self.peep))

    def process(self, ts, v, p, plot):
        # keep track of min/max values for this cycle

        if self.ts_prev > ts:
            self.reset()
        self.ts_prev = self.ts
        self.ts = ts
        if p > self.press_max:
            self.press_max = p
        if p < self.press_min:
            self.press_min = p

        if v > self.vol_max:
            self.vol_max = v
            self.vol_max_ts = ts
            self.vol_max_idx = plot.x
        if v < self.vol_min:
            self.vol_min = v
            self.vol_min_ts = ts
            self.vol_min_idx = plot.x

        # warnings
        if self.expiratory_vol != MAX_INT:
            if v - self.expiratory_vol > self.vol_leakage_warning:
                alarm("ts:{} Leakage Warning:".format(ts), v - self.expiratory_vol)

        # detect breathing state
        self.inspiratory_prev = self.inspiratory

        # some histeresis would be helpful
        if v > self.v_prev:  # inhale start condition
            self.inspiratory = True
        #        if (v < v_avg) and (v_prev > v_avg):  # exhale start condition
        if v < self.v_prev:  # exhale start condition
            self.inspiratory = False

        self.v_prev = v

        # # been too long since breathing, force transition
        # if (ts - self.expiratory_ts) > self.backup_rate_dt:
        #     print("forcing inspiratory cycle: {:.2f}s".format(ts - self.expiratory_ts))
        #     self.inspiratory = True
        #     self.vol_min_ts = ts
        # if (ts - self.inspiratory_ts) > self.backup_rate_dt:
        #     print("forcing expiratory cycle:{:.2f}s".format(ts - self.inspiratory_ts))
        #     self.inspiratory = False
        #     self.vol_max_ts = ts

        # process event triggers
        if self.inspiratory and not self.inspiratory_prev:  # inhale start event
            self.inhale_start(ts, v, p, plot)

        if not self.inspiratory and self.inspiratory_prev:  # exhale start event
            self.exhale_start(ts, v, p, plot)

    def inhale_start(self, ts, v, p, plot):
        self.peep = p
        self.press_min = MAX_INT
        if self.vol_min != MAX_INT:
            plot.point(self.vol_min_idx, self.vol_min - self.expiratory_vol, (255, 255, 0), size=3, lineThickness=2)

            # calculate breathing rate
            self.expiratory_ts_prev = self.expiratory_ts
            self.expiratory_ts = self.vol_min_ts
            self.expiratory_vol_prev = self.expiratory_vol
            self.expiratory_vol = self.vol_min
            if self.expiratory_vol_prev != MAX_INT:
                self.leak_est = self.expiratory_vol - self.expiratory_vol_prev
            if self.expiratory_ts_prev != 0:
                dt = self.expiratory_ts - self.expiratory_ts_prev
                self.rate = self.rate * RATE_SMOOTHING + (1 - RATE_SMOOTHING) * (60 / dt)
            self.print_report()
        self.vol_max = -MAX_INT

    def exhale_start(self, ts, v, p, plot):
        self.ppeak = self.press_max
        self.press_max = 0

        # breath was too shallow to count.  Eliminate noise when flow is too small/idle
        if (self.expiratory_vol != 0) and (v < self.expiratory_vol + MINIMUM_VOLUME_BREATH):
            self.inspiratory = True
            return

        if self.vol_max != MAX_INT:
            plot.point(self.vol_max_idx, self.vol_max - self.expiratory_vol, (0, 255, 255), size=3, lineThickness=2)
        if (self.vol_max != MAX_INT) and (self.vol_min != -MAX_INT):
            self.vol_tidal = VOL_TIDAL_SMOOTHING * self.vol_tidal + (1 - VOL_TIDAL_SMOOTHING) * (
                        self.vol_max - self.vol_min)
            self.inspiratory_ts = self.vol_max_ts
            self.print_report()
            self.vol_min = MAX_INT


def main():
    vent = Ventilator()
    file = None
    ser = None

    file_skip_ts = 0

    if USE_LOG_FILE:
        file = open(LOG_FILE)

        # skip to timestamp if set
        while True:
            ret = file_parse(file)
            if ret is None:
                continue
            ts, v, p = ret
            if ts > file_skip_ts:
                break
    else:
        ser = serial.Serial("COM4", 57600)
        print("Open Serial:", ser.name)

    plot = plotting.LinePlot("plots", ["time", "volume", "pressure"], 2, 800, 400, -PLOT_TV_MAX / 5, PLOT_TV_MAX)
    pv_plot = plotting.Plot2D("PV", 500, 500, -PLOT_PRESS_MAX / 10, PLOT_PRESS_MAX, -PLOT_TV_MAX / 5, PLOT_TV_MAX)

    while True:
        if USE_LOG_FILE:
            ret = file_parse(file)
            if ret is None:
                continue
            ts, v, p = ret
        else:
            ret = serial_parse(ser)
            if ret is None:
                continue
            ts, v, p = ret
        v *= VOLUME_SCALER # convert to mL

        vent.process(ts, v, p, plot)

        # pressure volume plot
        if vent.expiratory_ts != MAX_INT:
            pv_plot.lineTo(p, v - vent.expiratory_vol)
            font_size = 1.0
            pv_plot.mark_line(vent.ppeak, name="Ppeak", color=(0, 0.5, 0.5), size=font_size, axis=1, show_value=True)
            pv_plot.mark_line(vent.peep, name="PEEP", color=(0, 0.5, 0.5), size=font_size, axis=1, show_value=True, rightJustify=True)
            value_text = "{:.0f}mL".format(vent.vol_tidal - vent.leak_est / 2)
            pv_plot.mark_line(vent.vol_tidal + vent.leak_est / 2, name="TV Est", color=(0.5, 0.5, 0.0), size=font_size, axis=0,
                              name2=value_text)
            value_text = "{:.0f}mL".format(vent.leak_est)
            pv_plot.mark_line(vent.leak_est / 2, name="Leak Est", color=(0.25, 0.25, 0.0), size=font_size, axis=0,
                              name2=value_text)

            if v - vent.expiratory_vol > vent.vol_leakage_warning:
                cv2.putText(pv_plot.img, "ALARM:", (20, 100), cv2.FONT_HERSHEY_PLAIN, 3, (0, 0, 1), thickness=3)
                cv2.putText(pv_plot.img, "Leakage Warning", (20, 100 + 60), cv2.FONT_HERSHEY_PLAIN, 3, (0, 0, 1),
                            thickness=3)


        pv_plot.show()

        # line plots
        plot.add([v-vent.expiratory_vol, p*20])
        plot.show()
        c = cv2.waitKey(10) & 0xFF
        if c == 27:
            break
        if c == ord(' '):
            if USE_LOG_FILE:
                file.seek(0)
            vent.reset()
            print("reset")


if __name__ == '__main__':
    main()

import math
import numpy as np
import cv2
import time

colorTable = [[0, 255, 0],
              [0, 0, 255],
              [255, 0, 255],
              [0, 255, 255],
              [255, 255, 0]]


class Plot2D:
    def __init__(self, name, width, height, xMin, xMax, yMin, yMax):
        self.name = name
        self.img = np.zeros((height, width, 3))
        self.xMin = xMin
        self.xMax = xMax
        self.yMin = yMin
        self.yMax = yMax
        self.history = []
        self.history_size = 250

    def clear(self):
        cv2.rectangle(self.img, (0, 0), (self.img.shape[1], self.img.shape[0]), (0, 0, 0), -1)
        self.history = []

    def convertX(self, x):
        return int(self.img.shape[1]*(x-self.xMin)/float(self.xMax-self.xMin))

    def convertY(self, y):
        return int(self.img.shape[0]*(1.0 - (y-self.yMin)/float(self.yMax-self.yMin)))

    def mark_line(self, v, name=None, color=(1.0, 1.0, 1.0), size=1.0, axis=0, show_value=False, name2=None):
        line_spacing = int(20*size)
        text_y_pos = 0
        if axis == 0:
            y = self.convertY(v)
            cv2.line(self.img, (0, y), (self.img.shape[1], y), color)
            text_y_pos = y + line_spacing
            if name:
                cv2.putText(self.img, name, (self.convertX(0)+5, text_y_pos), cv2.FONT_HERSHEY_PLAIN, size, color)
            if name2:
                text_y_pos += line_spacing
                cv2.putText(self.img, name2, (self.convertX(0)+5, text_y_pos), cv2.FONT_HERSHEY_PLAIN, size, color)
            if show_value:
                text_y_pos += line_spacing
                cv2.putText(self.img, "{:.1f}".format(v), (self.convertX(0) + 5, text_y_pos), cv2.FONT_HERSHEY_PLAIN, size, color)
        else:
            x = self.convertX(v)
            cv2.line(self.img,  (x, 0), (x, self.img.shape[0]), color)
            text_y_pos = self.convertY(0) + line_spacing
            if name:
                cv2.putText(self.img, name, (x+5, text_y_pos), cv2.FONT_HERSHEY_PLAIN, size, color)
            if name2:
                text_y_pos += line_spacing
                cv2.putText(self.img, name2, (x+5, text_y_pos), cv2.FONT_HERSHEY_PLAIN, size, color)
            if show_value:
                text_y_pos += line_spacing
                cv2.putText(self.img, "{:.1f}".format(v), (x+5, text_y_pos), cv2.FONT_HERSHEY_PLAIN, size, color)

    def point(self, x, y, color=(1.0, 1.0, 1.0), size=3, lineThickness=1):
        x = self.convertX(x)
        y = self.convertY(y)
        self.history.append([x, y])
        cv2.circle(self.img, (x, y), size, color, lineThickness)

    def lineTo(self, x, y, color=(1.0, 1.0, 1.0), size=3, lineThickness=1):
        x = self.convertX(x)
        y = self.convertY(y)
        self.history.append((x, y))

        if len(self.history) > self.history_size:
            self.history.pop(0)

        cv2.rectangle(self.img, (0, 0), (self.img.shape[1], self.img.shape[0]), (0, 0, 0), -1)

        # axis line
        c = (0.25, 0.25, 0.25)
        self.mark_line(0, axis=0, color=c)
        self.mark_line(0, axis=1, color=c)
        # cv2.line(self.img, (0, self.convertY(0)), (self.img.shape[1], self.convertY(0)), c)
        # cv2.line(self.img, (self.convertX(0), 0), (self.convertX(0), self.img.shape[0]), c)

        cv2.circle(self.img, (x, y), size, color, lineThickness)

        if len(self.history) < 2:
            return

        for i in range(len(self.history)-1):
            p1 = self.history[i+1]
            p2 = self.history[i]
            s = (self.history_size-len(self.history)+i)/self.history_size
            c = (color[0]*s, color[1]*s, color[2]*s)
            cv2.line(self.img, p1, p2, color=c, thickness=lineThickness)

    def show(self):
        cv2.imshow(self.name, self.img)


class LinePlot:
    def __init__(self, name, series_names, xStep, width, height, yMin=-1, yMax=1, series_colors=None):
        self.name = name
        cv2.namedWindow(self.name)
        cv2.setMouseCallback(self.name, self.on_mouse, self)
        self.series_names = series_names
        self.img = np.zeros((height, width, 3), np.uint8)
        self.runtime = 0
        self.x = -1
        self.yMin = yMin
        self.yMax = yMax
        self.xStep = xStep
        self.font = cv2.FONT_HERSHEY_SIMPLEX
        self.prev_values = None
        self.color_table = colorTable
        self.horizontal_lines = [0]
        if series_colors is not None:
            self.color_table = series_colors
        self.timestamps = []
        self.start_time = time.time()
        self.mouse_down_time = -1
        self.thickness = 1

    def on_mouse(self, event, x, y, flags, param):
        cursor_time = -1

        for ts in param.timestamps:
            if ts[0] < x:
                continue
            cursor_time = ts[1]-param.start_time
            break

        value_y = (1 - y/self.img.shape[0])*(self.yMax-self.yMin) + self.yMin
        cv2.rectangle(param.img, (0, 0), (600, 20), (0, 0, 0), -1)
        cv2.putText(param.img, "{0:.3f}, {1:.3f}".format(cursor_time, value_y),
                    (5, 15), cv2.FONT_HERSHEY_PLAIN, 0.9, (128, 128, 128))
        if event == cv2.EVENT_LBUTTONDOWN:
            self.mouse_down_time = cursor_time
        if event == cv2.EVENT_LBUTTONUP:
            self.mouse_down_time = -1

        if self.mouse_down_time != -1:
            dt = cursor_time-self.mouse_down_time
            freq = 0
            if dt > 0:
                freq = 1.0/dt
            cv2.putText(param.img, "delta: {0:.3f}s freq:{1:.3f}Hz".format(dt, freq),
                        (100, 15), cv2.FONT_HERSHEY_PLAIN, 0.9, (128, 128, 128))

    def clear(self):
        self.x = -1
        cv2.rectangle(self.img, (0, 0), (self.img.shape[1], self.img.shape[0]), (0, 0, 0), -1)
        self.prev_values = None
        self.start_time = time.time()
        self.x = 0
        self.timestamps = []

    def convertY(self, y):
        v = self.img.shape[0]*(1.0 - (y-self.yMin) / float(self.yMax - self.yMin))
        return int(v)

    def mark(self, color=(128, 128, 128)):
        cv2.line(self.img, (self.x-1, 0), (self.x-1, self.img.shape[0]), color)

    def point(self, x, y, color=(255,255,255), size=3, lineThickness=1):
        cv2.circle(self.img, (x, self.convertY(y)), size, color, lineThickness)

    def add(self, values):
        if self.prev_values is None:
            self.prev_values = np.copy(values)
        self.timestamps.append((self.x, time.time()))
        x2 = self.x+self.xStep

        # wipe line
        cv2.rectangle(self.img, (self.x, 0), (x2, self.img.shape[0]), (0, 0, 0), -1)
        cv2.line(self.img, (x2, 0), (x2, self.img.shape[0]), (64, 64, 64))

        for i in range(len(self.horizontal_lines)):
            y1 = self.convertY(self.horizontal_lines[i])
            y2 = self.convertY(self.horizontal_lines[i])
            cv2.line(self.img, (self.x, y1), (x2, y2), (64, 64, 64), 1)

        for i in range(len(values)):
            y1 = self.convertY(self.prev_values[i])
            y2 = self.convertY(values[i])
            cv2.line(self.img, (self.x, y1), (x2, y2), self.color_table[i], self.thickness)

        self.x += self.xStep
        self.prev_values = np.copy(values)
        if self.x > self.img.shape[1]:
            self.clear()

    def show(self):
        cv2.imshow(self.name, self.img)



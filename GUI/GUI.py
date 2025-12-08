import sys
import serial
import threading
import pyqtgraph as pg
from PyQt5 import QtWidgets, QtCore

PORT = "COM7"   # Change to your COM port
BAUD = 115200
MAX_POINTS = 500

# Sensor labels in the same order as printf()
keys = [
    "temp1", "temp2", "temp3", "press", "hum", "voc",
    "accx1", "accy1", "accz1", "magx1", "magy1", "magz1",
    "accx2", "accy2", "accz2",
    "gyrx1", "gyry1", "gyrz1",
    "accx3", "accy3", "accz3",
    "gyrx2", "gyry2", "gyrz2"
]

# Units for legends/titles
sensor_labels = {
    "temp1": "Temp1 [°C]", "temp2": "Temp2 [°C]", "temp3": "Temp3 [°C]",
    "press": "Pressure [hPa]", "hum": "Humidity [%]", "voc": "VOC Index [ppb]",
    "accx1": "AccX1 [mg]", "accy1": "AccY1 [mg]", "accz1": "AccZ1 [mg]",
    "magx1": "MagX1 [mG]", "magy1": "MagY1 [mG]", "magz1": "MagZ1 [mG]",
    "accx2": "AccX2 [mg]", "accy2": "AccY2 [mg]", "accz2": "AccZ2 [mg]",
    "gyrx1": "GyroX1 [mdps]", "gyry1": "GyroY1 [mdps]", "gyrz1": "GyroZ1 [mdps]",
    "accx3": "AccX3 [mg]", "accy3": "AccY3 [mg]", "accz3": "AccZ3 [mg]",
    "gyrx2": "GyroX2 [mdps]", "gyry2": "GyroY2 [mdps]", "gyrz2": "GyroZ2 [mdps]"
}

data_buffer = {k: [] for k in keys}

def serial_reader():
    ser = serial.Serial(PORT, BAUD)
    while True:
        try:
            line = ser.readline().decode(errors="ignore").strip("\r\n ")
            values = list(map(float, line.split(',')))
            if len(values) == len(keys):
                for i, key in enumerate(keys):
                    data_buffer[key].append(values[i])
                    if len(data_buffer[key]) > MAX_POINTS:
                        data_buffer[key] = data_buffer[key][-MAX_POINTS:]
        except:
            pass

class SensorDashboard(QtWidgets.QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("OctaSense")
        self.resize(1400, 900)

        tabs = QtWidgets.QTabWidget()
        self.setCentralWidget(tabs)

        # Colors and styles
        self.colors = [
            'r', 'g', 'b', 'm', 'c', 'y', 'w', '#FF8000', '#8000FF', '#00FF80',
            '#FF0080', '#80FF00', '#0080FF', '#FFBF00', '#00BFFF', '#8B0000'
        ]
        styles = [QtCore.Qt.SolidLine, QtCore.Qt.DashLine, QtCore.Qt.DotLine]

        # Dictionary: { sensor_key: [curve1, curve2, ...] }
        self.curves = {k: [] for k in keys}

        def make_plot(title, sensor_list):
            plot = pg.PlotWidget(title=title)
            legend = plot.addLegend(offset=(30, 30))
            for i, sensor in enumerate(sensor_list):
                color = self.colors[i % len(self.colors)]
                style = styles[(i // len(self.colors)) % len(styles)]
                pen = pg.mkPen(color=color, width=2, style=style)
                curve = plot.plot(pen=pen, name=sensor_labels[sensor])
                self.curves[sensor].append(curve)
            return plot

        # Tab 1 – Environment
        tabs.addTab(make_plot("Temperature [°C]",
                              ["temp1", "temp2", "temp3"]),
                    "Environment: Temp")
        
        tabs.addTab(make_plot("Pressure [hPa] Humidity [%] and VOC",
                              ["press", "hum", "voc"]),
                    "Environment: Others")

        # Tab 2 – IMUs
        tabs.addTab(make_plot("LIS2DUXS12 Acc [mg]",
                              ["accx1", "accy1", "accz1"]),
                    "IMU #1")
        
        tabs.addTab(make_plot("LSM6DSO: Acc [mg] + Gyro [mdps]",
                              ["accx2", "accy2", "accz2","gyrx1", "gyry1", "gyrz1"]),
                    "IMU #2")
        
        tabs.addTab(make_plot("LSM6DSV: Acc [mg] + Gyro [mdps]",
                              ["accx3", "accy3", "accz3","gyrx2", "gyry2", "gyrz2"]),
                    "IMU #3")
        
        tabs.addTab(make_plot("LIS2MDL Mag [mG]",
                              ["magx1", "magy1", "magz1"]),
                    "Magneto")

        # Overview tab
        tabs.addTab(self.make_overview_tab(), "All Sensors")

        # Update timer
        self.timer = QtCore.QTimer()
        self.timer.timeout.connect(self.update_plots)
        self.timer.start(50)

    def make_overview_tab(self):
        scroll = QtWidgets.QScrollArea()
        scroll.setWidgetResizable(True)
        container = QtWidgets.QWidget()
        layout = QtWidgets.QGridLayout(container)

        cols = 4  # number of plots per row
        for idx, key in enumerate(keys):
            row, col = divmod(idx, cols)

            # Rounded corner frame
            frame = QtWidgets.QFrame()
            frame.setStyleSheet("""
                QFrame {
                    border: 1px solid #555;
                    border-radius: 7px;
                    background-color: #1e1e1e;
                }
            """)
            frame_layout = QtWidgets.QVBoxLayout(frame)

            plot = pg.PlotWidget(title=sensor_labels[key])
            plot.setMinimumSize(250, 200)
            plot.addLegend(offset=(10, 10))

            color = self.colors[idx % len(self.colors)]
            pen = pg.mkPen(color=color, width=2)
            curve = plot.plot(pen=pen, name=sensor_labels[key])
            self.curves[key].append(curve)  # Append to shared dict

            frame_layout.addWidget(plot)
            layout.addWidget(frame, row, col)

        scroll.setWidget(container)
        return scroll

    def update_plots(self):
        for key, curve_list in self.curves.items():
            for curve in curve_list:
                curve.setData(data_buffer[key])

if __name__ == "__main__":
    threading.Thread(target=serial_reader, daemon=True).start()
    app = QtWidgets.QApplication(sys.argv)
    win = SensorDashboard()
    win.show()
    sys.exit(app.exec_())

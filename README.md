# SCB4XV1-C092RC-IKS4A1
Contains code for CUBE IDE and Python GUI for SCB4XV1 + C092RC + IKS4A1.

## Cube IDE Code
In the folder **Cube IDE Code** there is a `main.c` and a `main.h` file. Replace the ones in the existing `/Core/Src` (for `main.c`) and `/Core/Inc` (for `main.h`) with these, and you should be good to deploy the project onto the Nucleo C092RC.


**Disclaimer:** The step of replacing the files in your original Cube IDE program will produce errors if you haven't followed the step-by-step guide on *Mouser blog link*.

## GUI
For the GUI you can either run the Python file `GUI.py`. Just make sure to change the COM port at the very top:

```python
...
PORT = "COM7"   # Change to your COM port
BAUD = 115200
MAX_POINTS = 500
...

```

Or you can run the executable `GUI.exe` and select the baud rate (which by default should be **115200**, matching the USART baud rate on the Nucleo C092RC).


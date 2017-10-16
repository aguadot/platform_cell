import sys
from cx_Freeze import setup, Executable

# Dependencies are automatically detected, but it might need fine tuning.
build_exe_options = {"packages": ["os"], "excludes": ["tkinter"]}

# GUI applications require a different base on Windows (the default is for a
# console application).
base = None
if sys.platform == "win32":
    base = "Win32GUI"

setup(  name = "movimiento_led",
        version = "0.1",
        description = "Aplicacion para contro platform_cell!",
        options = {"build_exe": build_exe_options},
        executables = [Executable("movimiento_led.py", base=base)])


"""
ejecutar en la linea de comandos
python setup.py build
"""

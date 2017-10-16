# -*- coding: cp1252 -*-
import sys
import serial

from time import sleep
from PyQt5.QtWidgets import QMainWindow, QPushButton, QApplication, QLabel

labelText = "elegir led"
ser = serial.Serial()
#ser.port = "COM32"
ser.baudrate = "9600"

def scan_serial():

    dispositivos_serie = []
    for i in range(1, 50):
        try:
            #-- Abrir puerto serie
            s = serial.Serial("COM"+str(i), 9600, timeout = 3)

            #comprobamos que el dispositivo es el que buscamos
            try:
                recibo = s.readline()
                if str(recibo[0] )== '97':
                    dispositivos_serie.append(s.port)
                    #print("COM" + str(i))
            except:
                pass

            #-- Cerrar puerto
            s.close()
        except:
            pass
            
    return dispositivos_serie


class Example(QMainWindow):
    
    def __init__(self):
        super().__init__()
        
        self.initUI()
        
        
    def initUI(self):

        
        self.label_1 = QLabel(labelText, self)
        self.label_1.setGeometry(30, 20, 300, 25)
        
        self.label_2 = QLabel(self)
        self.label_2.move(150,150)
        
        self.led1 = QPushButton("led 1", self)
        self.led1.move(30, 50)

        self.led2 = QPushButton("led 2", self)
        self.led2.move(150, 50)

        self.led3 = QPushButton("led 3", self)  
        self.led3.move(270, 50)

        self.con_serie= QPushButton("Conectar", self)
        self.con_serie.move(150,100)

        self.statusBar()
        self.setGeometry(500, 500, 500, 500)
        self.setWindowTitle('Event sender')
        self.show()
        self.led1.setEnabled(False)
        self.led2.setEnabled(False)
        self.led3.setEnabled(False)
        self.con_serie.setEnabled(True)
        self.con_serie.setText("Conectar")
        self.label_1.setText("Conectar puerto Serie")

        self.led1.clicked.connect(self.buttonClicked)            
        self.led2.clicked.connect(self.buttonClicked)
        self.led3.clicked.connect(self.buttonClicked)
        self.con_serie.clicked.connect(self.buttonConect)
        
    def buttonConect(self):
        
        sender = self.sender() #nos dice cual es el boton pulsado
        sleep(2)
        puertos = scan_serial()
        if len (puertos) != 0:
            i=0
            for n in puertos:
                puerto = str (puertos[i])
                ser.port = puerto
                #print (ser.port)
                i = i+1
        try:
            if sender.text() == "Conectar":
                ser.open()
                ex.label_1.setText("Conectado, elige laposición")
                ex.con_serie.setText("Desconectar")
                ex.led1.setEnabled(True)
                ex.led2.setEnabled(True)
                ex.led3.setEnabled(True)
                
                
            elif sender.text() == "Desconectar":
                ser.close()
                ex.label_1.setText("Conectar Puerto Serie")
                ex.con_serie.setText("Conectar")
                ex.led1.setEnabled(False)
                ex.led2.setEnabled(False)
                ex.led3.setEnabled(False)
           
        except serial.SerialException:

            ex.label_1.setText("Error al conectar puerto Serie")
            ser.close()
            ex.con_serie.setText("Conectar")
        
    def buttonClicked(self):
        try:
            ser.flushInput() #limpia el buffer del puerto serie
            sender = self.sender() #nos dice cual es el boton pulsado
            sleep(2)

            if sender.text() == "led 1":
                labelText = "situación actual led: 1"
                ser.write(b'1')
                ex.label_1.setText(labelText)
     
            elif sender.text()== "led 2":
                labelText = "situación actual led: 2"
                ser.write(b'2')
                ex.label_1.setText(labelText)
                
            else: 
                labelText = "situación actual led: 3"
                ser.write(b'3')
                ex.label_1.setText(labelText)
                
            recibo = ser.readline()
            
            if str (recibo[0]) == '49':
                ex.led1.setEnabled(False)
                ex.led2.setEnabled(True)
                ex.led3.setEnabled(True)
            elif str (recibo[0]) == '50':
                ex.led1.setEnabled(True)
                ex.led2.setEnabled(False)
                ex.led3.setEnabled(True)
            elif str (recibo[0]) == '51':
                ex.led1.setEnabled(True)
                ex.led2.setEnabled(True)
                ex.led3.setEnabled(False)
                

        except serial.SerialException:

            ex.label_1.setText("Error al conectar puerto Serie")
            ser.close()
            ex.con_serie.setText("Conectar")
            ex.led1.setEnabled(False)
            ex.led2.setEnabled(False)
            ex.led3.setEnabled(False)

    
            
if __name__ == '__main__':
    
    app = QApplication(sys.argv)
    ex = Example()
    sys.exit(app.exec_())

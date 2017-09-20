# -*- coding: cp1252 -*-
import sys
import serial
from time import sleep
from PyQt5.QtWidgets import QMainWindow, QPushButton, QApplication, QLabel

labelText = "elegir led"

class Example(QMainWindow):
    
    def __init__(self):
        super().__init__()
        
        self.initUI()
        
        
    def initUI(self):      

        self.ser = serial.Serial("COM32",9600)

        self.label_1 = QLabel(labelText, self)
        self.label_1.setGeometry(30, 90, 300, 25)

        
        self.label_2 = QLabel("prueba", self)
        self.label_2.move(150,150)
        
        led1 = QPushButton("led 1", self)
        led1.move(30, 50)

        led2 = QPushButton("led 2", self)
        led2.move(150, 50)

        led3 = QPushButton("led 3", self)  
        led3.move(270, 50)
        
        led1.clicked.connect(self.buttonClicked)            
        led2.clicked.connect(self.buttonClicked)
        led3.clicked.connect(self.buttonClicked)
        
        self.statusBar()
        
        self.setGeometry(500, 500, 500, 500)
        self.setWindowTitle('Event sender')
        self.show()

        
    def buttonClicked(self):
         
        self.ser.flushInput() #limpia el buffer del puerto serie
        sender = self.sender() #nos dice cual es el boton pulsado
        sleep(2)
        if sender.text() == "led 1":
            labelText = "situación actual led: 1"
            self.ser.write(b'1')
            #ex.label_1.move(30, 80)
            ex.label_1.setText(labelText)
 
        elif sender.text()== "led 2":
            labelText = "situación actual led: 2"
            self.ser.write(b'2')
            #ex.label_1.move(30, 80)
            ex.label_1.setText(labelText)
        else: 
            labelText = "situación actual led: 3"
            self.ser.write(b'3')
            #ex.label_1.move(30, 80)
            ex.label_1.setText(labelText)
        recibo = self.ser.readline()
        ex.label_2.setText(str (recibo))
        """
        except:
            self.label_2.setText("error")
        """
            
if __name__ == '__main__':
    
    app = QApplication(sys.argv)
    ex = Example()
    sys.exit(app.exec_())

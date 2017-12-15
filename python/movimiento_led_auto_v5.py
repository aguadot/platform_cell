# -*- coding: utf-8 -*-
import sys

import threading
import serial

from time import sleep, time
from PyQt5.QtWidgets import QMainWindow, QPushButton, QApplication, QLabel, QLineEdit, QCheckBox

labelText = "elegir led"
ser = serial.Serial()
# ser.port = "COM32"
ser.baudrate = "115200"
ser.timeout = 10
vuelta = 1

class Example(QMainWindow):
    def __init__(self) -> object:
        super().__init__()

        self.initUI()

    def initUI(self):

        self.label_1 = QLabel(labelText, self)
        self.label_1.setGeometry(30, 20, 300, 25)
        self.label_1.setText("Conectar puerto Serie")

        self.con_serie = QPushButton("Conectar", self)
        self.con_serie.move(30, 50)
        self.con_serie.setEnabled(True)
        self.con_serie.setText("Conectar")
        self.con_serie.clicked.connect(self.buttonConect)

        self.text_2 = QLineEdit(self)
        self.text_2.move(140, 100)
        self.text_2.setText('01')
        self.text_2.setInputMask('9''9') # mascara de entrada de numeros enteros
        self.text_2.setEnabled(False)

        self.label_2 = QLabel(labelText, self)
        self.label_2.setGeometry(30, 100, 125, 25)
        self.label_2.setText("pasos del filtro 1-72")
        self.label_2.setEnabled(False)

        self.label_3 = QLabel(labelText, self)
        self.label_3.setGeometry(30, 150, 300, 25)
        self.label_3.setText("led a usar")
        self.label_3.setEnabled(False)

        self.label_4 = QLabel(labelText, self)
        self.label_4.setGeometry(300, 100, 300, 25)
        self.label_4.setText("")
        self.label_4.setEnabled(True)

        self.label_5 = QLabel(labelText, self)
        self.label_5.setGeometry(250, 175, 300, 25)
        self.label_5.setText("")
        self.label_5.setEnabled(True)

        self.check_1 = QCheckBox(self)
        self.check_1.setGeometry(125, 150, 120, 20)
        self.check_1.setText("led 1")
        self.check_1.setEnabled(False)

        self.check_2 = QCheckBox(self)
        self.check_2.setGeometry(125, 175, 120, 20)
        self.check_2.setText("led 2")
        self.check_2.setEnabled(False)

        self.check_3 = QCheckBox(self)
        self.check_3.setGeometry(125, 200, 120, 20)
        self.check_3.setText("led 3")
        self.check_3.setEnabled(False)

        self.con_auto = QPushButton("Comenzar", self)
        self.con_auto.move(30, 250)
        self.con_auto.setEnabled(False)
        self.con_auto.clicked.connect(self.buttonComenzar)

        self.label_6 = QLabel(labelText, self)
        self.label_6.setGeometry(200,300,300,25)
        self.label_6.setText("")
        self.label_6.setEnabled(True)

        self.label_7 = QLabel(labelText, self)
        self.label_7.setGeometry(200, 350, 300, 25)
        self.label_7.setText("Enfoque manual")
        self.label_7.setEnabled(False)

        self.enf_5= QPushButton("<<", self)
        self.enf_5.move(30,375)
        self.enf_5.clicked.connect(self.enfocar)
        self.enf_5.setEnabled(False)

        self.enf_1=QPushButton("<", self)
        self.enf_1.move(130,375)
        self.enf_1.clicked.connect(self.enfocar)
        self.enf_1.setEnabled(False)

        self.enf_inv_1 = QPushButton(">", self)
        self.enf_inv_1.move(260, 375)
        self.enf_inv_1.clicked.connect(self.enfocar)
        self.enf_inv_1.setEnabled(False)

        self.enf_inv_5 = QPushButton(">>", self)
        self.enf_inv_5.move(360, 375)
        self.enf_inv_5.clicked.connect(self.enfocar)
        self.enf_inv_5.setEnabled(False)

        self.autofocus = QPushButton("autofocus",self)
        self.autofocus.move(190,425)
        self.autofocus.clicked.connect(self.autofocu)
        self.autofocus.setEnabled(False)

        self.statusBar()
        self.setGeometry(500, 500, 500, 500)
        self.setWindowTitle('Event sender')
        self.show()

    def buttonConect(self):
        global vuelta
        sender = self.sender()  # nos dice cual es el boton pulsado
        sleep(2)
        puertos = scan_serial()
        if len(puertos) > 0:
            i = 0
            for n in puertos:
                puerto = str(puertos[i])
                ser.port = puerto
                # print (ser.port)
                i = i + 1
        try:
            if sender.text() == "Conectar":
                ser.open()
                self.label_6.setText("iniciando")
                self.con_auto.setEnabled(False)
                terminado1 = threading.Thread(target=comprobar) #esperamos confirmación para arrancar
                terminado1.start()
                terminado1.join()
                ex.label_1.setText("Desconectar Puerto Serie")
                ex.con_serie.setText("Desconectar")
                self.text_2.setEnabled(True)
                self.text_2.setFocus()
                self.text_2.setText("01")
                self.label_3.setEnabled(True)
                self.label_2.setEnabled(True)
                self.check_1.setEnabled(True)
                self.check_1.setChecked(False)
                self.check_2.setEnabled(True)
                self.check_2.setChecked(False)
                self.check_3.setEnabled(True)
                self.check_3.setChecked(False)
                self.label_7.setEnabled(True)
                self.enf_5.setEnabled(True)
                self.enf_1.setEnabled(True)
                self.enf_inv_1.setEnabled(True)
                self.enf_inv_5.setEnabled(True)
                self.autofocus.setEnabled(True)
                #self.con_auto.setEnabled(True)
                vuelta = 2


            elif sender.text() == "Desconectar":
                ser.close()
                ex.label_1.setText("Conectar Puerto Serie")
                ex.con_serie.setText("Conectar")
                self.text_2.setEnabled(False)
                self.label_3.setEnabled(False)
                self.label_2.setEnabled(False)
                self.check_1.setEnabled(False)
                self.check_2.setEnabled(False)
                self.check_3.setEnabled(False)
                self.con_auto.setEnabled(False)
                self.label_6.setText("")
                self.label_4.setText("")
                self.label_5.setText("")
                self.label_7.setEnabled(False)
                self.enf_5.setEnabled(False)
                self.enf_1.setEnabled(False)
                self.enf_inv_1.setEnabled(False)
                self.enf_inv_5.setEnabled(False)
                self.autofocus.setEnabled(False)
                vuelta = 1

        except serial.SerialException:

            ex.label_1.setText("Error al conectar puerto Serie")
            ser.close()
            ex.con_serie.setText("Conectar")

    def buttonComenzar(self):

        codigo_led = 0
        codigo_filtro = 0
        leds = False
        pasos = False
        try:
            ser.flushInput()
            ser.flushOutput()
            #sleep(2)
            # primero comprobar que el texto es un entero comprendido entre 1 y 72
            if not self.text_2.text(): # si el texto está vacio le ponemos un 00 para evitar eroores de cadena vacia
                self.text_2.setText("00")
            if (int(self.text_2.text()) > 0) and (int(self.text_2.text()) <= 72):
                codigo_filtro = self.text_2.text()
                self.label_4.setText("")
                pasos = True

            else:

                self.label_4.setText("Inserte un numero entre 1 y 72")


            if self.check_1.isChecked():
                if self.check_2.isChecked():
                    if self.check_3.isChecked():
                        codigo_led = '7'  # led1 -> led2 -> led3
                        leds = True
                    else:
                        codigo_led = '6'  # led1 -> led2
                        leds = True
                else:
                    if self.check_3.isChecked():
                        codigo_led = '5'  # led1 -> led3
                        leds = True
                    else:
                        codigo_led = '4'  # led1
                        leds = True
            else:
                if self.check_2.isChecked():
                    if self.check_3.isChecked():
                        codigo_led = '3'  # led2 -> led3
                        leds = True
                    else:
                        codigo_led = '2'  # led2
                        leds = True
                else:
                    if self.check_3.isChecked():
                        codigo_led = '1'  # led3
                        leds = True
                    else:
                        codigo = 0  # pedir marcar alguno
                        leds = False

            if leds == True:
                self.label_5.setText("")
                if pasos == True:
                    self.label_6.setText("en proceso")
                    self.con_auto.setEnabled(False)
                    self.enf_5.setEnabled(False)
                    self.enf_1.setEnabled(False)
                    self.enf_inv_1.setEnabled(False)
                    self.enf_inv_5.setEnabled(False)
                    self.autofocus.setEnabled(False)
                    trama_completa = '{0},{1}'.format(codigo_led, codigo_filtro) #enviamos la trama con la secuencia de led´s y el nº de saltos del filtro
                    #print(bytes(trama_completa, 'utf-8'))
                    ser.write(bytes(trama_completa, 'utf-8'))

                    terminado = threading.Thread(target=comprobar)
                    terminado.start()

                else:
                    pass
            else:
                self.label_5.setText("Elija uno o varios leds")

        except serial.SerialException:

            ex.label_1.setText("Error al conectar puerto Serie")
            ser.close()
            ex.con_serie.setText("Conectar")

    def enfocar(self):
        sender = self.sender()
        num_pasos=0
        enfocar='8'
        try:
            ser.flushInput()
            ser.flushOutput()
            if sender.text() == "<":
                num_pasos='1'
            if sender.text() == "<<":
                num_pasos='2'
            if sender.text() == ">":
                num_pasos='3'
            if sender.text() == ">>":
                num_pasos='4'
            else:
                pass
            self.label_6.setText("en proceso")
            self.con_auto.setEnabled(False)
            self.enf_5.setEnabled(False)
            self.enf_1.setEnabled(False)
            self.enf_inv_1.setEnabled(False)
            self.enf_inv_5.setEnabled(False)
            self.autofocus.setEnabled(False)
            mov_enfoque = '{0},{1}'.format(enfocar, num_pasos)  # enviamos la trama con la secuencia de enfocar siempre es un 8 y el nº de pasos a mover
            # print(bytes(trama_completa, 'utf-8'))
            ser.write(bytes(mov_enfoque, 'utf-8'))

            terminado = threading.Thread(target=comprobar)
            terminado.start()

        except serial.SerialException:

            ex.label_1.setText("Error al conectar puerto Serie")
            ser.close()
            ex.con_serie.setText("Conectar")

    def autofocu(self):

        try:
           enfocar='9'
           num_pasos='0'
           self.label_6.setText("en proceso")
           self.con_auto.setEnabled(False)
           self.enf_5.setEnabled(False)
           self.enf_1.setEnabled(False)
           self.enf_inv_1.setEnabled(False)
           self.enf_inv_5.setEnabled(False)
           self.autofocus.setEnabled(False)
           mov_enfoque = '{0},{1}'.format(enfocar, num_pasos)  # enviamos la trama con la secuencia de enfocar siempre es un 8 y el nº de pasos a mover
           # print(bytes(trama_completa, 'utf-8'))
           ser.write(bytes(mov_enfoque, 'utf-8'))

           terminado = threading.Thread(target=comprobar)
           terminado.start()


        except serial.SerialException:

            ex.label_1.setText("Error al conectar puerto Serie")
            ser.close()
            ex.con_serie.setText("Conectar")

def comprobar():

    hora_inicio = time()
    hora_actual = time()
    ok = False
    global vuelta
    while not ok : # revisamos cada 2 segundos si tenemos contestación de arduino
        while (hora_actual < (hora_inicio + 2)):
            hora_actual = time()
        if ser.inWaiting():
            data = ser.read()
            #print("recibo:")
            #print(data)
            #print("vuelta:")
            #print(vuelta)
            if str(data[0]) == '49':
                if vuelta == 1 :
                    ex.con_auto.setEnabled(True)
                    ex.label_6.setText("Inicio correcto")
                elif vuelta == 2 :
                    ex.label_6.setText("proceso realizado correctamente")
                    ex.con_auto.setEnabled(True)
                    ex.enf_5.setEnabled(True)
                    ex.enf_1.setEnabled(True)
                    ex.enf_inv_1.setEnabled(True)
                    ex.enf_inv_5.setEnabled(True)
                    ex.autofocus.setEnabled(True)
                #print("ok")
                ok = True
            else:
                #print("ko")
                ok = False
                # return False
        else:
            ok = False
            #print("error")
        hora_inicio = time()

def scan_serial():

    dispositivos_serie = []
    for i in range(1, 50):
        try:
            # -- Abrir puerto serie
            s = serial.Serial("COM" + str(i), 115200, timeout=7)

            # comprobamos que el dispositivo es el que buscamos
            try:
                recibo = s.readline()
                if '97' == str(recibo[0]):
                    dispositivos_serie.append(s.port)
                    # print("COM" + str(i))
            except:
                pass
            s.flushInput()
            s.flushOutput()
            # -- Cerrar puerto
            s.close()
        except:
            pass

    return dispositivos_serie


if __name__ == '__main__':
    app = QApplication(sys.argv)
    ex = Example()
    sys.exit(app.exec_())
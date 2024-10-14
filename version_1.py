import serial
import matplotlib.pyplot as plt
plt.ion()
# Actualiza con tu puerto serial
device = '/dev/ttyACM1'
# Configurar la conexión serial
ser = serial.Serial(device, 9600)
op = 0
while op != "e" and op != "E":
    print("\n\n\nOPCIONES:\n\n"
          "1- Muestra el grafico de temperatura\n"
          "2- Muestra el grafico de humedad\n"
          "3- Imprime el puerto serial\n"
          "e- Exit")
    op=input("OPCIÓN: ")
    if op == "1":
        print("\n\nTemperaturas")
        T = [0]*10
        x = [1,2,3,4,5,6,7,8,9,10]
        while True:                                                             #Mostra gràfica temperatura (1)
            try:
                data = ser.readline().decode('utf-8').split()
                t = float(data[0])
                if data[0] == "nan":
                    ser.write("EL".encode('utf-8'))
                plt.cla()
                plt.axis([0,11,20,30])
                T = [T[1], T[2], T[3], T[4], T[5], T[6], T[7], T[8], T[9], t]
                tm = (T[0]+T[1]+T[2]+T[3]+T[4]+T[5]+T[6]+T[7]+T[8]+T[9])/10
                if t<26:
                    plt.plot(x, T, 'green')                                         #Parametros y color
                else:
                    plt.plot(x, T, 'red')
                    plt.text(4,15, "Alta temperatura (>26ºC)", color='red')
                plt.title("Temperaturas")
                plt.xlabel("Últimas 10 temperaturas")
                plt.ylabel("Temperatura (ºC)")
                plt.show()
                print("Temperatura:", t, "ºC\nMedia:", tm, "ºC\n")
                plt.pause(1)
            except IndexError:                                                  #Evita que no llegeixi bé la llista
                None
            except ValueError:
                None
                if data[0] == "nan":
                    ser.write("EL".encode('utf-8'))
            except KeyboardInterrupt:
                plt.close()
                break
    if op == "2":
        print("\n\nHumedades:")
        H = [0]*10
        x = [1,2,3,4,5,6,7,8,9,10]
        while True:                                                             #Mostra gràfica temperatura (1)
            try:
                data = ser.readline().decode('utf-8').split()
                h = float(data[1])
                if data[1] == "nan":
                    ser.write("EL".encode('utf-8'))
                plt.cla()
                plt.axis([0,11,50,100])
                H = [H[1], H[2], H[3], H[4], H[5], H[6], H[7], H[8], H[9], h]
                hm = (H[0]+H[1]+H[2]+H[3]+H[4]+H[5]+H[6]+H[7]+H[8]+H[9])/10
                plt.plot(x, H, 'blue')                                         #Parametros y color
                plt.title("Humedades")
                plt.xlabel("Últimas 10 humedades")
                plt.ylabel("Humedad (ºC)")
                plt.show()
                print("Humedad:", h, "ºC\nMedia:", hm, "%\n")
                plt.pause(1)
            except IndexError:                                                  #Evita que no llegeixi bé la llista
                None
            except ValueError:
                None
            except KeyboardInterrupt:
                plt.close()
                break
    if op == "3":
        while True:
            try:
                data = ser.readline().decode('utf-8').split()
                print("Temperatura:", data[0],"ºC   |   Humedad:", data[1], "%")
            except IndexError:                                                  #Evita que no llegeixi bé la llista
                None
            except KeyboardInterrupt:
                break
    if op == "e" or op == "E":
        None
    else:
        print("Opción no válida")
ser.close()
print("\nFin del programa...")
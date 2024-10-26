import serial
import matplotlib.pyplot as plt
plt.ion()
# Actualiza con tu puerto serial
device = '/dev/ttyACM0'
# Configurar la conexión serial
ser = serial.Serial(device, 9600)
op = 0
while op != "e" and op != "E":
    print("\n\n\nOPCIONES:\n\n"
          "1- Muestra los gráficos ambientales\n"
          "2- Distancias\n"
          "3- Imprime el puerto serial\n"
          "e- Exit")
    op=input("OPCIÓN: ")
    if op == "1":
        print("\n\nTemperaturas")
        T = [0]*10
        H = [0]*10
        P = [0]*10
        A = [0]*10
        x = [1,2,3,4,5,6,7,8,9,10]
        while True:                                                             #Mostra gràfica temperatura (1)
            try:
                data = ser.readline().decode('utf-8').split()
                t = float(data[0])
                h = float(data[1])
                p = float(data[2])
                a = float(data[3])
                T = [T[1], T[2], T[3], T[4], T[5], T[6], T[7], T[8], T[9], t]
                H = [H[1], H[2], H[3], H[4], H[5], H[6], H[7], H[8], H[9], h]
                A = [A[1], A[2], A[3], A[4], A[5], A[6], A[7], A[8], A[9], a]
                P = [P[1], P[2], P[3], P[4], P[5], P[6], P[7], P[8], P[9], p]
                sumt=0
                for i in T:
                    sumt = sumt + i
                tm = sumt/10
                sumh=0
                for i in H:
                    sumh = sumh + i
                hm = sumh/10
                
                gt=plt.subplot(2,2,1)
                gt.cla()
                gt.axis([1,10,20,40])
                if t<30:
                    gt.plot(x, T, 'green')                                         #Parametros y color
                else:
                    gt.plot(x, T, 'red')
                    gt.text(1,25, "Alta temperatura (>30ºC)", color='red')
                gt.set_title("Temperatura (ºC)")
                gt.grid()

                gh=plt.subplot(2,2,2)
                gh.cla()
                gh.axis([1,10,40,100])
                gh.plot(x, H, 'blue')                                         #Parametros y color
                gh.set_title("Humedad (%)")
                gh.grid()

                gp=plt.subplot(2,2,3)
                gp.cla()
                gp.axis([1,10,50000,150000])
                gp.plot(x, P, 'orange')                                         #Parametros y color
                gp.set_title("Altitud ()")
                gp.grid()

                ga=plt.subplot(2,2,4)
                ga.cla()
                ga.axis([1,10,-50,100])
                ga.plot(x, A, 'yellow')                                         #Parametros y color
                ga.set_title("Presión (Pa)")
                ga.grid()

                plt.subplots_adjust(wspace=0.3, hspace=0.5)
                plt.show()
                print("Temperatura:", t, "ºC      Media:", tm, "ºC")
                print("Humedad:", h, "ºC          Media:", hm, "%")
                print("Altitud:", a, "")
                print("Presión:", p, "Pa\n")
                plt.pause(0.01)
            except IndexError:              #Evita que no llegeixi bé la llista
                None
            except ValueError:
                None
            except KeyboardInterrupt:
                plt.close()
                break
    if op == "2":
        D = [0]*19
        x = [0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180]
        while True:                                                             #Mostra gràfica temperatura (1)
            try:
                data = ser.readline().decode('utf-8').split()
                d = float(data[0])
                i = int(data[1])
                plt.cla()
                plt.axis([0,180,0,100])
                D[int(i/10)]= d
                plt.bar(x, D, color='blue')                                         #Parametros y color
                plt.title("Radar")
                plt.show()
                plt.pause(0.01)
                print(d, i)
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
                data = ser.readline().decode('utf-8').rstrip()
                print(data)
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
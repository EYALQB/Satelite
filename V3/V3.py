from tkinter import *
import serial
import matplotlib.pyplot as plt
import math

plt.ion()
# Actualiza con tu puerto serial
device = '/dev/ttyACM0'
# Configurar la conexión serial
ser = serial.Serial(device, 9600)
# Variables globales
Amb = False
Dist = False
T = [0]*10
H = [0]*10
D = [0]*36
T2 = [0]*10
H2 = [0]*10
angulos = [math.radians(deg) for deg in range(0, 360, 10)]  # Ángulos en radianes

# Variables de estado para controlar la creación de la figura y ejes
fig1, gt, gh = None, None, None
fig2, gd = None, None  # Definimos fig y gd como None para poder manejarlos

def toggle(var):
    """Cambia el estado de la variable 'var' y actualiza la interfaz"""
    global Amb, Dist,fig1, gt, gh, fig2, gd
    if var == "Amb":
        if Amb:
            Amb = False

        else:
            Amb = True
            fig1, (gt, gh) = plt.subplots(2, 1)
        grafica_ambiental()  # Iniciar la actualización de la variable i

    if var == "Dist":
        if Dist:
            Dist = False
        else:
            Dist = True
            # Aquí se crea la figura y los ejes solo si no han sido creados aún
            fig2, gd = plt.subplots(subplot_kw={'projection': 'polar'})
        grafica_radar()  # Iniciar la actualización del radar

def grafica_ambiental():
    global T, H, Amb, data, fig1, gt, gh
    x=[1,2,3,4,5,6,7,8,9,10]
    if Amb:
        if(data[0]=="Amb"):
            t = float(data[1])
            h = float(data[2])
            for i in range(0,9):
                T[i]=T[i+1]
            T[9]=t
            for i in range(0,9):
                H[i]=H[i+1]
            H[9]=h
            sumt=0
            for i in T:
                sumt = sumt + i
            tm = round(sumt/10,2)
            sumh=0
            for i in H:
                sumh = sumh + i
            hm = round(sumh/10,2)
            
            plt.figure(fig1)
            gt=plt.subplot(2,1,1)
            gt.cla()
            gt.axis([1,10,15,40])
            if t<30:
                gt.plot(x, T, 'green')                                         #Parametros y color
            else:
                gt.plot(x, T, 'red')
                gt.text(2,25, "Alta temperatura (>30ºC)", color='red')
            gt.set_title("Temperatura (ºC)")
            gt.text(7,42, f"Media: {tm} ºC", color='green')
            gt.grid()

            gh=plt.subplot(2,1,2)
            gh.cla()
            gh.axis([1,10,40,100])
            gh.plot(x, H, 'blue')                                         #Parametros y color
            gh.set_title("Humedad (%)")
            gh.text(7,105, f"Media: {hm} %", color='blue')
            gh.grid()

            plt.subplots_adjust(wspace=0.3, hspace=0.5)
            plt.draw()
        window.after(50, grafica_ambiental)       # Llamamos a esta función nuevamente después de 100 ms (0.1 segundo)
    else:
        plt.close(fig1)

def grafica_radar():
    global D, Dist, angulos, fig2, gd, data
    if Dist:
        if data[0] == "dist":
            d1 = float(data[1])  # Primer valor de distancia
            i = int(data[2])     # Índice en grados
            d2 = float(data[3])  # Segundo valor de distancia
            if i==180:
                D[180] = d1
                D[0] = d2
            else:
                D[i//10] = d1
                D[(i//10) + 18] = d2
            
            plt.figure(fig2)
            # Limpia y actualiza el gráfico
            gd.cla()
            gd.set_ylim(0, 100)
            gd.fill(angulos, D, color='green', alpha=0.3)  # Relleno del gráfico
            plt.title("Radar en tiempo real")
            
            plt.draw()  # Pausa para actualizar la gráfica sin bloquear

            # Llama nuevamente a `grafica_radar` después de 50 ms si `Dist` sigue activo
        window.after(50, grafica_radar)
    else:
        plt.close(fig2)

def revisar_datos():
    global data
    data = ser.readline().decode('utf-8').split()  # Lectura del puerto serial
    window.after(50, revisar_datos)

# Configuración inicial de la ventana
window = Tk()
window.geometry("500x500")
window.rowconfigure(0, weight=0)
window.rowconfigure(1, weight=1)
window.rowconfigure(2, weight=1)
window.columnconfigure(0, weight=1)
window.columnconfigure(1, weight=1)

# Título
tituloLabel = Label(window, text="Menú", font=("Courier", 20, "italic"))
tituloLabel.grid(row=0, column=0, rowspan=1, columnspan=3, padx=5, pady=5)

# Botón para cambiar el estado de la gráfica de ambiente
BotonAmb = Button(window, text="Gráficas de ambiente", bg='red', fg="black", command=lambda: toggle("Amb"))
BotonAmb.grid(row=2, column=0, padx=5, pady=5)

# Botón para cambiar el estado del radar
BotonDist = Button(window, text="Radar", bg='blue', fg="black", command=lambda: toggle("Dist"))
BotonDist.grid(row=2, column=1, padx=5, pady=5)

# 
fraseEntry = Entry(window, text="2")
fraseEntry.grid(row=0, column=0, columnspan = 1, padx=5, pady=5)

revisar_datos()
# Inicia el bucle principal de Tkinter
window.mainloop()
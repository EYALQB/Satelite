import math
import re
import numpy as np
import matplotlib
from tkinter import *
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import serial

# Configuración para gráficas interactivas
matplotlib.use('TkAgg')

# Variables globales
Amb = False
Dist = False
T = [0] * 10
H = [0] * 10
D = [0] * 36
angulos = [math.radians(deg) for deg in range(0, 360, 10)]
fig1, gt, gh, fig2, gd = None, None, None, None, None
data = []

# Expresión regular para extraer coordenadas del simulador de órbitas
regex = re.compile(r"Position: \(X: ([\d\.-]+) m, Y: ([\d\.-]+) m, Z: ([\d\.-]+) m\)")

# Variables globales del simulador de órbitas
x_vals, y_vals, z_vals = [], [], []
orbit_fig, orbit_ax = None, None
orbit_plot, current_point = None, None
Orb = False  # Inicialmente Orb está en False

# Configuración de serial
ser = serial.Serial('COM3', 9600, timeout=1)

# Función para alternar entre los estados de las gráficas
def toggle(var):
    global Amb, Dist, fig1, gt, gh, fig2, gd, orbit_fig
    if var == "Amb":
        Amb = not Amb
        if Amb:
            fig1, (gt, gh) = plt.subplots(2, 1)
            grafica_ambiental()
        else:
            plt.close(fig1)
    elif var == "Dist":
        Dist = not Dist
        if Dist:
            fig2, gd = plt.subplots(subplot_kw={'projection': 'polar'})
            grafica_radar()
        else:
            plt.close(fig2)
    elif var == "Orb":
        if orbit_fig is None:
            simulador_orbitas()  # Llama al simulador si no está en ejecución
        else:
            plt.close(orbit_fig)  # Cierra el gráfico y reinicia las variables
            orbit_fig = None

# Gráfica ambiental
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

# Gráfica radar
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

def simulador_orbitas():
    """Inicializa y actualiza el simulador de órbitas."""
    global x_vals, y_vals, z_vals, orbit_fig, orbit_ax, orbit_plot, current_point

    # Si el gráfico no está inicializado, lo crea
    if orbit_fig is None:
        orbit_fig = plt.figure()
        orbit_ax = orbit_fig.add_subplot(111, projection='3d')

        # Dibujar la Tierra
        R_EARTH = 6371000
        u = np.linspace(0, 2 * np.pi, 100)
        v = np.linspace(0, np.pi, 100)
        x_sphere = R_EARTH * np.outer(np.cos(u), np.sin(v))
        y_sphere = R_EARTH * np.outer(np.sin(u), np.sin(v))
        z_sphere = R_EARTH * np.outer(np.ones(np.size(u)), np.cos(v))
        orbit_ax.plot_surface(x_sphere, y_sphere, z_sphere, color='orange', alpha=0.5, label='Earth')

        orbit_ax.set_xlim(-7e6, 7e6)
        orbit_ax.set_ylim(-7e6, 7e6)
        orbit_ax.set_zlim(-7e6, 7e6)
        orbit_ax.set_xlabel('X (meters)')
        orbit_ax.set_ylabel('Y (meters)')
        orbit_ax.set_zlabel('Z (meters)')
        orbit_ax.set_title('Satellite Equatorial Orbit')
        orbit_ax.legend()

        # Inicializar trazas
        orbit_plot, = orbit_ax.plot3D([], [], [], 'b-', label='Satellite Orbit')
        current_point = orbit_ax.scatter([], [], [], color='red', s=50, label='Current Position')

    # Actualizar datos si están disponibles
    if Orb:
    
        if data[0] == "Orb":
        
            # Extraer valores X, Y, Z desde la línea de datos
            x = float(data[1].split(":")[1])  # Obtiene el valor después de "X:"
            y = float(data[2].split(":")[1])  # Obtiene el valor después de "Y:"
            z = float(data[3].split(":")[1])  # Obtiene el valor después de "Z:"
            
            # Actualizar listas de posiciones
            x_vals.append(x)
            y_vals.append(y)
            z_vals.append(z)

            # Actualizar el gráfico
            orbit_plot.set_data_3d(x_vals, y_vals, z_vals)
            current_point._offsets3d = ([x], [y], [z])
            
            # Ajustar límites dinámicos
            max_range = max(map(abs, x_vals + y_vals + z_vals)) * 1.1
            orbit_ax.set_xlim(-max_range, max_range)
            orbit_ax.set_ylim(-max_range, max_range)
            orbit_ax.set_zlim(-max_range, max_range)
       
        # Usar `Tkinter.after()` para actualizar periódicamente
        window.after(50, simulador_orbitas)
    else:
        plt.close(orbit_fig)

# Inicia la revisión de datos del puerto serial
def revisar_datos():
    global data, Orb
    if ser and ser.in_waiting > 0:
        line = ser.readline().decode('utf-8').strip()
        data = line.split()
        print("Datos recibidos:", data)

        # Activar simulación de órbitas si se recibe la señal de órbita
        if data[0] == "Orb" and Orb == False:
            print("Activando simulación de órbitas...")
            Orb = True  # Activar simulación de órbitas
        
    window.after(100, revisar_datos)

# Configuración de la interfaz
window = Tk()
window.geometry("500x500")
window.title("Interfaz de satélite")
window.rowconfigure([0, 1, 2], weight=1)
window.columnconfigure([0, 1], weight=1)

# Título
tituloLabel = Label(window, text="Menú", font=("Courier", 20, "italic"))
tituloLabel.grid(row=0, column=0, columnspan=2, pady=10)

# Botones para activar/desactivar gráficas
BotonAmb = Button(window, text="Gráficas de ambiente", bg='red', fg="black", command=lambda: toggle("Amb"))
BotonAmb.grid(row=2, column=0, padx=10, pady=10)

BotonDist = Button(window, text="Radar", bg='blue', fg="black", command=lambda: toggle("Dist"))
BotonDist.grid(row=2, column=1, padx=10, pady=10)

BotonOrbita = Button(window, text="Simulador de órbitas", bg='green', fg="black", command=lambda: toggle("Orb"))
BotonOrbita.grid(row=2, column=2, padx=5, pady=5)

# Inicia la revisión de datos del puerto serial
revisar_datos()

# Inicia el bucle principal de la interfaz
window.mainloop()

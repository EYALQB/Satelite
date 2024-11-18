from tkinter import *

#Variables
CalcMedia=0
EnvAmb=1
Amb=0
Rad=0
Pos=0
ContrServ=0

def toggle(var):
    global CalcMedia, EnvAmb, Amb, Rad, Pos, ContrServ
    print("Has activado toogle() de la variable", var)
    if var=="CalcMedia":
        if CalcMedia==0:
            CalcMedia=1
            botonMedia.config(text="Calcular media\nen tierra")
            media.config(text="Calculando media en\nel satélite")
        else:
            CalcMedia=0
            botonMedia.config(text="Calcular media en\nel satélite")
            media.config(text="Calculando media\nen tierra")
    if var=="Amb":
        if Amb==0:
            Amb=1
            botonGrafAmb.config(text="Ocultar gráficas de ambiente")
        else:
            Amb=0
            botonGrafAmb.config(text="Mostrar gráficas de ambiente")
    if var=="Rad":
        if Amb==0:
            Amb=1
            botonGrafRad.config(text="Ocultar radar")
        else:
            Amb=0
            botonGrafRad.config(text="Mostrar radar")
    if var=="Pos":
        if Pos==0:
            Pos=1
            botonGrafPos.config(text="Ocultar posición")
        else:
            Pos=0
            botonGrafPos.config(text="Mostrar posición")
    if var=="EnvAmb":
        if EnvAmb==0:
            EnvAmb=1
            botonDatosAmb.config(text="Detener envío\nde datos")
            datosAmb.config(text="Envío activado")
        else:
            EnvAmb=0
            botonDatosAmb.config(text="Reanudar envío\nde datos")
            datosAmb.config(text="Envío desactivado")
    if var=="ContrServ":
        if ContrServ==0:
            ContrServ=1
            botonServo.config(text="Activar modo automático")
            servo.config(text="Modo manual")
            botonIzquierda.config(bg="red")
            botonDerecha.config(bg="red")
        else:
            ContrServ=0
            botonServo.config(text="Activar modo manual")
            servo.config(text="Modo automático")
            botonIzquierda.config(bg="grey")
            botonDerecha.config(bg="grey")

def girar(lado):
    global ContrServ
    if ContrServ==1:
        if lado=="<":
            print("Has girado a la izquierda")
        elif lado == ">":
            print("Has girado a la derecha")
    else:
        None

def enviarConfig():
    global CalcMedia, EnvAmb, ContrServ
    iAmb = intervalAmb.get()
    iPos = intervalPos.get()
    configuración = " ".join((str(CalcMedia), str(EnvAmb), str(ContrServ), iAmb, iPos))
    print("Cnf", configuración)

# Configuración inicial de la ventana
menu = Tk()
menu.geometry("800x500")
menu.rowconfigure(0, weight=2)
menu.rowconfigure(1, weight=1)
menu.rowconfigure(2, weight=1)
menu.rowconfigure(3, weight=1)
menu.rowconfigure(4, weight=1)
menu.rowconfigure(5, weight=1)
menu.columnconfigure(0, weight=1)
menu.columnconfigure(1, weight=1)
menu.columnconfigure(2, weight=1)
menu.columnconfigure(3, weight=1)

#Título
titulo = Label(menu, text="Menú principal", font=("Courier", 20, "bold"))
titulo.grid(row=0, column=0, columnspan = 4, padx=5, pady=5)

#Ambiente
Ambiente = Label(menu, text="Ambiente", font=("Courier", 15, "bold"))
Ambiente.grid(row=1, column=0, columnspan = 2, padx=5, pady=5)
# Mostrar/ocultar la gráfica de ambiente
botonGrafAmb = Button(menu, text="Mostrar gráficas de ambiente", bg='blue', fg="black", command=lambda: toggle("Amb"))
botonGrafAmb.grid(row=2, column=0, columnspan = 2, padx=5, pady=5)
# Detener/reanudar el envío de datos de ambiente
datosAmb = Label(menu, text="Envío activado", font=("Courier", 10, "italic"))
datosAmb.grid(row=3, column=0, padx=5, pady=5)
botonDatosAmb = Button(menu, text="Detener envío\nde datos", bg='blue', fg="black", command=lambda: toggle("EnvAmb"))
botonDatosAmb.grid(row=3, column=1, padx=5, pady=5)
# Cambiar donde calcular la media
media = Label(menu, text="Calculando media\nen tierra", font=("Courier", 10, "italic"))
media.grid(row=4, column=0, padx=5, pady=5)
botonMedia = Button(menu, text="Calcular la media\nen el satélite", bg='blue', fg="black", command=lambda: toggle("CalcMedia"))
botonMedia.grid(row=4, column=1, padx=5, pady=5)

#Radar
Radar = Label(menu, text="Radar", font=("Courier", 15, "bold"))
Radar.grid(row=1, column=2, columnspan = 2, padx=5, pady=5)
# Mostrar/ocultar el radar
botonGrafRad = Button(menu, text="Mostrar radar", bg='red', fg="black", command=lambda: toggle("Rad"))
botonGrafRad.grid(row=2, column=2, columnspan=2, padx=5, pady=5)
# Quien controla el servo
servo = Label(menu, text="Modo automático", font=("Courier", 10, "italic"))
servo.grid(row=3, column=2, padx=5, pady=5)
botonServo = Button(menu, text="Activar modo manual", bg='red', fg="black", command=lambda: toggle("ContrServ"))
botonServo.grid(row=3, column=3, padx=5, pady=5)
# Girar Servo
botonIzquierda = Button(menu, text="<", bg='grey', fg="black", command=lambda: girar("<"))
botonIzquierda.grid(row=4, column=2, padx=5, pady=5)
botonDerecha = Button(menu, text=">", bg='grey', fg="black", command=lambda: girar(">"))
botonDerecha.grid(row=4, column=3, padx=5, pady=5)

#Posición
Posición = Label(menu, text="Ambiente", font=("Courier", 15, "bold"))
Posición.grid(row=5, column=0, columnspan = 2, padx=5, pady=5)
# Mostrar/ocultar la gráfica de ambiente
botonGrafPos = Button(menu, text="Mostrar posición", bg='yellow', fg="black", command=lambda: toggle("Pos"))
botonGrafPos.grid(row=6, column=0, columnspan = 2, padx=5, pady=5)

#Intervalos
intervalos = Label(menu, text="Definir intervalos de envío de datos", font=("Courier", 15, "bold"))
intervalos.grid(row=7, column=0, columnspan = 4, padx=5, pady=5)
# Interval ambient
intervalAmb = Entry(menu)
intervalAmb.grid(row=8, column=2, padx=5, pady=5)
intervalAmb.insert(0, "1000")
introduceIntervaloAmb = Label(menu, text="Intervalo de envío de datos de ambiente (ms):", font=("Courier", 10, "italic"))
introduceIntervaloAmb.grid(row=8, column=0, columnspan=2, padx=5, pady=5)
# Interval posición
intervalPos = Entry(menu)
intervalPos.grid(row=9, column=2, padx=5, pady=5)
intervalPos.insert(0, "1000")
introduceIntervaloPos = Label(menu, text="Intervalo de envío de posición (ms):", font=("Courier", 10, "italic"))
introduceIntervaloPos.grid(row=9, column=0, columnspan=2, padx=5, pady=5)
# Enviar datos
botonAplicar = Button(menu, text="Aplicar cambios", bg='green', fg="black", command=enviarConfig)
botonAplicar.grid(row=8, column=3, rowspan=2, padx=5, pady=5)
enviarConfig()
# Inicia el bucle principal de Tkinter
menu.mainloop()
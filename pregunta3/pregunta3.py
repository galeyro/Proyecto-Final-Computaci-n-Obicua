#Codigo Python 
import serial 
import json 
import requests 
import csv 
import os 
from datetime import datetime 
import tkinter as tk 
from tkinter import ttk, messagebox 
from threading import Thread 

# Configuración del puerto serie 
arduino = serial.Serial('COM7', 9600)  # Cambia 'COM4' al puerto de tu Arduino 

# Configuración de Ubidots 
UBIDOTS_TOKEN = "BBUS-oZ2GM2MnMmYhDbpVpnFLEVxONo7tca" 
UBIDOTS_URL = "http://industrial.api.ubidots.com/api/v1.6/devices/contenedor"

# Nombre del archivo CSV 
ARCHIVO_CSV = "datos_sensor.csv" 

# Crear archivo CSV si no existe
if not os.path.exists(ARCHIVO_CSV):
    with open(ARCHIVO_CSV, mode='w', newline='') as archivo:
        escritor = csv.writer(archivo)
        escritor.writerow(["Fecha y Hora", "Temperatura (°C)", "Humedad (%)"])

# Funciones auxiliares
def enviar_a_ubidots(datos):
    try:
        headers = {"X-Auth-Token": UBIDOTS_TOKEN, "Content-Type": "application/json"}
        response = requests.post(UBIDOTS_URL, headers=headers, json=datos)
        if response.status_code == 200:
            print("Datos enviados a Ubidots correctamente.")
        else:
            print(f"Error al enviar a Ubidots: {response.status_code}, {response.text}")
    except Exception as e:
        print(f"Error en la conexión a Ubidots: {e}")

def guardar_en_csv(datos):
    try:
        with open(ARCHIVO_CSV, mode='a', newline='') as archivo:
            escritor = csv.writer(archivo, delimiter=',')
            escritor.writerow([
                datetime.now().strftime("%Y-%m-%d %H:%M:%S"),
                datos["temperatura"],
                datos["humedad"]
            ])
        print("Datos guardados en el archivo CSV.")
    except Exception as e:
        print(f"Error al guardar en el archivo CSV: {e}")

# Funciones para la interfaz gráfica
def iniciar_lectura():
    def leer_datos():
        try:
            while leyendo:
                if arduino.in_waiting > 0:
                    linea = arduino.readline().decode('utf-8').strip()
                    datos = json.loads(linea)

                    # Mostrar datos en la tabla
                    tabla.insert("", "end", values=(
                        datetime.now().strftime("%Y-%m-%d %H:%M:%S"),
                        datos["temperatura"],
                        datos["humedad"]
                    ))

                    # Guardar en CSV y enviar a Ubidots
                    guardar_en_csv(datos)
                    enviar_a_ubidots({"temperatura": datos["temperatura"], "humedad": datos["humedad"]})

        except Exception as e:
            print(f"Error durante la lectura: {e}")

    global leyendo
    leyendo = True
    hilo = Thread(target=leer_datos)
    hilo.daemon = True
    hilo.start()

def detener_lectura():
    global leyendo
    leyendo = False
    messagebox.showinfo("Lectura detenida", "La lectura de datos se ha detenido.")

def abrir_ventana_datos():
    ventana_datos = tk.Toplevel()
    ventana_datos.title("Monitor de Datos del Sensor")
    ventana_datos.geometry("800x400")

    frame_tabla = ttk.Frame(ventana_datos)
    frame_tabla.pack(fill=tk.BOTH, expand=True, padx=10, pady=10)

    columnas = ("Fecha y Hora", "Temperatura (°C)", "Humedad (%)")

    global tabla
    tabla = ttk.Treeview(frame_tabla, columns=columnas, show="headings")
    for col in columnas:
        tabla.heading(col, text=col)
        tabla.column(col, anchor="center")

    tabla.pack(fill=tk.BOTH, expand=True)

    frame_botones = ttk.Frame(ventana_datos)
    frame_botones.pack(fill=tk.X, padx=10, pady=10)

    btn_iniciar = ttk.Button(frame_botones, text="Iniciar Lectura", command=iniciar_lectura)
    btn_iniciar.pack(side=tk.LEFT, padx=5)
    btn_detener = ttk.Button(frame_botones, text="Detener Lectura", command=detener_lectura)
    btn_detener.pack(side=tk.LEFT, padx=5)

    ventana_datos.transient(ventana_principal)
    ventana_datos.grab_set()
    ventana_principal.wait_window(ventana_datos)

# Crear la interfaz principal
ventana_principal = tk.Tk()
ventana_principal.title("Control del Sensor")
ventana_principal.geometry("300x200")

btn_abrir = ttk.Button(ventana_principal, text="Abrir Monitor de Datos", command=abrir_ventana_datos)
btn_abrir.pack(expand=True, pady=50)

ventana_principal.mainloop()

# Cerrar conexiones al cerrar la ventana principal
def cerrar_conexiones():
    global leyendo
    leyendo = False
    arduino.close()
    print("Conexión con Arduino cerrada.")
    ventana_principal.destroy()

ventana_principal.protocol("WM_DELETE_WINDOW", cerrar_conexiones)
# =================================================================================
#  File             : pedro_app.py
#  Description      : Control serial interface for the Pedro robot
#  Supported Boards : Rev2 and Rev3
#
#  Author           : Almoutazar SAANDI
#  Last update      : May 9, 2025
#  Version          : v1.0.1
#
#  Robot Firmware Requirement:
#  ---------------------------------------------------------------------------------
#  This application requires the firmware serialMode.ino uploaded to the Pedro robot board
#
#  Required Sketch: serialMode.ino
#  Communication: Serial (baudrate 9600)
#  Repository/Source: https://github.com/almtzr/Pedro/tree/main/code/serialMode
# =================================================================================

import tkinter as tk
import serial
import serial.tools.list_ports
from PIL import Image, ImageTk

class PedroApp:
    def __init__(self, root):
        self.root = root
        self.root.title("Pedro Robot App")
        self.root.geometry("850x750")
        self.selected_servo = tk.IntVar(value=1)
        self.serial_port = None

        self.build_ui()
        self.list_serial_ports()

    def build_ui(self):
        # === 
        title_label = tk.Label(self.root, text="Pedro Serial Controller", font=("Helvetica", 30, "bold"))
        title_label.pack(pady=(10, 5))

        # === 
        version_frame = tk.Frame(self.root)
        version_frame.pack(padx=20, pady=5)

        version_label = tk.Label(version_frame, text="v1.0.1", font=("Helvetica", 12), anchor="e")
        version_label.pack(side="right") 

        frame = tk.Frame(self.root, padx=20, pady=10)
        frame.pack()

        # ==== 
        serial_frame = tk.LabelFrame(frame, text="Serial Connection", font=("Arial", 16), padx=12, pady=12)
        serial_frame.grid(row=0, column=0, columnspan=1, sticky="we", pady=(0, 15))

        tk.Label(serial_frame, text="Port série :").grid(row=0, column=0)
        self.port_menu = tk.StringVar()
        self.port_dropdown = tk.OptionMenu(serial_frame, self.port_menu, "")
        self.port_dropdown.config(width=25)  
        self.port_dropdown.grid(row=0, column=1, padx=10)

        self.status_canvas = tk.Canvas(serial_frame, width=22, height=22, highlightthickness=0)
        self.status_canvas.grid(row=0, column=2)
        self.status_circle = self.status_canvas.create_oval(2, 2, 20, 20, fill="red")

        connect_btn = tk.Button(serial_frame, text="Connect", command=self.connect_serial)
        connect_btn.grid(row=2, column=0, padx=(0, 5), pady=(10, 0), sticky="w")

        disconnect_btn = tk.Button(serial_frame, text="Disconnect", command=self.disconnect_serial)
        disconnect_btn.grid(row=2, column=1, padx=(0, 5), pady=(10, 0), sticky="w")

        # === 
        select_servo = tk.LabelFrame(frame, text="Select Servo", font=("Arial", 16), padx=10, pady=10)
        select_servo.grid(row=1, column=0, columnspan=1, sticky="we", pady=(0, 15))

        self.selected_servo = tk.IntVar(value=1)

        for i in range(1, 5):
            btn = tk.Radiobutton(
                select_servo,
                text=f"Servo {i}",
                variable=self.selected_servo,
                value=i,
                indicatoron=0,
                width=10,
                bg="yellow",  
                command=lambda i=i: self.update_image(i)
            )
            btn.grid(row=0, column=i-1, padx=5)

        pedro_frame = tk.LabelFrame(frame, padx=1, pady=1)
        pedro_frame.grid(row=2, column=0, columnspan=1, sticky="we", pady=(0, 15))

        try:
            image = Image.open("pedro1.png")
            image = image.resize((300, 300), Image.ANTIALIAS)  
            photo = ImageTk.PhotoImage(image)

            self.img_label = tk.Label(pedro_frame, image=photo)
            self.img_label.image = photo  
            self.img_label.pack()
        except Exception as e:
            error_label = tk.Label(pedro_frame, text=f"Erreur de chargement image: {e}")
            error_label.pack()

        move_servo = tk.LabelFrame(frame, text="Move servo", font=("Arial", 16), padx=12, pady=12)
        move_servo.grid(row=3, column=0, columnspan=1, pady=(0, 15))  

        self.direction = tk.IntVar(value=1)

        self.slider = tk.Scale(
            move_servo,
            from_=0,
            to=2,
            orient="horizontal",
            resolution=1,
            variable=self.direction,
            showvalue=False,
            length=100,             
            command=self.direction_changed,
            bg="yellow",
            troughcolor="gold"
        )

        self.slider.grid(row=0, column=0, padx=10)

    def list_serial_ports(self):
        ports = [port.device for port in serial.tools.list_ports.comports()]
        if ports:
            self.port_menu.set(ports[0])
            self.port_dropdown['menu'].delete(0, 'end')
            for port in ports:
                self.port_dropdown['menu'].add_command(label=port, command=lambda p=port: self.port_menu.set(p))
        else:
            self.port_menu.set("Aucun port")

    def update_image(self, index):
        print(f"Servo sélectionné: {index}")
        try:
            image = Image.open(f"pedro{index}.png")
            image = image.resize((300, 300), Image.ANTIALIAS)
            photo = ImageTk.PhotoImage(image)

            self.img_label.configure(image=photo)
            self.img_label.image = photo

            self.direction.set(1)
            servo = self.selected_servo.get()
            self.send_command(servo, "I")
        except Exception as e:
            print(f"Erreur lors de la mise à jour de l'image : {e}")

    def connect_serial(self):
        port = self.port_menu.get()
        try:
            self.serial_port = serial.Serial(port, 9600, timeout=1)
            print(f"Connecté à {port}")
            self.status_canvas.itemconfig(self.status_circle, fill="green")
        except:
            print("Erreur de connexion")
            self.status_canvas.itemconfig(self.status_circle, fill="red")

    def disconnect_serial(self):
        if self.serial_port and self.serial_port.is_open:
            self.serial_port.close()
            print("Déconnecté.")
            self.status_canvas.itemconfig(self.status_circle, fill="red")

    def next_servo(self):
        self.selected_servo.set((self.selected_servo.get() % 4) + 1)
        servo = self.selected_servo.get()
        self.send_command(servo, "I")

    def direction_changed(self, value):
        direction_map = {0: "L", 1: "I", 2: "R"}
        direction = direction_map.get(int(value), "I")
        servo = self.selected_servo.get()
        self.send_command(servo, direction)

    def send_command(self, servo, direction):
        if self.serial_port and self.serial_port.is_open:
            command = f"{servo}{direction}\n"
            self.serial_port.write(command.encode())
            print(f"Envoyé : {command.strip()}")
        else:
            print("Port série non connecté")

if __name__ == "__main__":
    root = tk.Tk()
    app = PedroApp(root)
    root.mainloop()



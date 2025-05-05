import tkinter as tk
import serial
import serial.tools.list_ports
from PIL import Image, ImageTk

class PedroApp:
    def __init__(self, root):
        self.root = root
        self.root.title("Pedro Robot Controller")
        self.root.geometry("850x650")
        self.selected_servo = tk.IntVar(value=1)
        self.serial_port = None

        self.build_ui()
        self.list_serial_ports()

    def build_ui(self):
        frame = tk.Frame(self.root, padx=20, pady=20)
        frame.pack()

        # ==== Cadre gris clair pour le port série ====
        serial_frame = tk.LabelFrame(frame, text="Connexion série", padx=12, pady=12)
        serial_frame.grid(row=0, column=0, columnspan=1, sticky="we", pady=(0, 15))

        tk.Label(serial_frame, text="Port série :").grid(row=0, column=0)
        self.port_menu = tk.StringVar()
        self.port_dropdown = tk.OptionMenu(serial_frame, self.port_menu, "")
        self.port_dropdown.grid(row=0, column=1, padx=10)

        connect_btn = tk.Button(serial_frame, text="Connecter", command=self.connect_serial)
        connect_btn.grid(row=0, column=2, padx=(0, 5))

        disconnect_btn = tk.Button(serial_frame, text="Déconnecter", command=self.disconnect_serial)
        disconnect_btn.grid(row=0, column=3, padx=(0, 5))

        self.status_canvas = tk.Canvas(serial_frame, width=22, height=22, highlightthickness=0)
        self.status_canvas.grid(row=0, column=4)
        self.status_circle = self.status_canvas.create_oval(2, 2, 20, 20, fill="red")


        # === Suite du layout comme avant ===

        # Frame pour sélection du servo
        select_servo = tk.LabelFrame(frame, text="Select Servo", padx=10, pady=10)
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
                command=lambda i=i: self.update_image(i)
            )
            btn.grid(row=0, column=i-1, padx=5)


        pedro_frame = tk.LabelFrame(frame, padx=1, pady=1)
        pedro_frame.grid(row=2, column=0, columnspan=1, sticky="we", pady=(0, 15))
        #tk.Label(pedro_frame, text=" ").grid(row=0, column=0)

        # Charger l'image
        try:
            image = Image.open("pedro1.png")
            image = image.resize((300, 300), Image.ANTIALIAS)  # Ajuste selon ton besoin
            photo = ImageTk.PhotoImage(image)

            self.img_label = tk.Label(pedro_frame, image=photo)
            self.img_label.image = photo  # Nécessaire pour éviter le garbage collection
            self.img_label.pack()
        except Exception as e:
            error_label = tk.Label(pedro_frame, text=f"Erreur de chargement image: {e}")
            error_label.pack()

        move_servo = tk.LabelFrame(frame, text="Move servo", padx=12, pady=12)
        move_servo.grid(row=3, column=0, columnspan=1, sticky="we", pady=(0, 15))

  #      tk.Label(frame, text="Direction :").grid(row=3, column=0, pady=(10, 5))
        self.direction = tk.IntVar(value=1)
        self.slider = tk.Scale(move_servo, from_=0, to=2, orient="horizontal",
                               resolution=1, variable=self.direction,
                               showvalue=False, length=200,
                               command=self.direction_changed)
        self.slider.grid(row=4, column=1, columnspan=2)

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
            self.img_label.image = photo  # Important pour garder la référence
        except Exception as e:
            print(f"Erreur lors de la mise à jour de l'image : {e}")

    def connect_serial(self):
        port = self.port_menu.get()
        try:
            self.serial_port = serial.Serial(port, 9600, timeout=1)
            print(f"Connecté à {port}")
            # Cercle vert si succès
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
        self.send_command("I")

    def direction_changed(self, value):
        direction_map = {0: "L", 1: "I", 2: "R"}
        direction = direction_map.get(int(value), "I")
        self.send_command(direction)

    def send_command(self, direction):
        if self.serial_port and self.serial_port.is_open:
            servo = self.selected_servo.get()
            command = f"{servo}{direction}\n"
            self.serial_port.write(command.encode())
            print(f"Envoyé : {command.strip()}")
        else:
            print("Port série non connecté")

if __name__ == "__main__":
    root = tk.Tk()
    app = PedroApp(root)
    root.mainloop()
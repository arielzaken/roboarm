import serial
import time

# Windows serial port, straight outta 2004
port = 'COM5'
baud = 115200

# G-code menu options
menu = {
    "1": "G1 A90 B0 C0 D0",
    "2": "G1 A0 B90 C0 D0",
    "3": "G1 A0 B0 C90 D0",
    "4": "G1 A0 B0 C0 D90",
    "x": "exit"
}

def show_menu():
    print("\n=== GCODE CONTROL MENU ===")
    for key, cmd in menu.items():
        print(f"{key}: {cmd}")
    print("==========================")

try:
    with serial.Serial(port, baud, timeout=1) as ser:
        time.sleep(2)  # Give ESP32 a moment to reset
        while True:
            show_menu()
            choice = input("Enter choice: ").strip()
            if choice == 'x':
                print("Exiting.")
                break
            cmd = menu.get(choice)
            if cmd:
                ser.write((cmd + '\n').encode('utf-8'))
                print(f"> Sent: {cmd}")
            else:
                print("Invalid option.")
except serial.SerialException as e:
    print(f"[ERROR] Could not open port {port}: {e}")

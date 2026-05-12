import serial
import matplotlib.pyplot as plt

SERIAL_PORT = "/dev/cu.usbserial-10"
BAUD_RATE = 9600 # Need to match baud rate to the QUTY serial setup

ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1) # Open connection to QUTY 

# Create empty lists for data input 
times = []
positions = []
velocities = []
accelerations = []
modes = []

print("Reading QUTy telemetry. Press Ctrl+C to stop and plot.") # Print instructions 

# Infinite loop 
try:
    while True: # keep reading forever (unless you manually stop it)
        line = ser.readline().decode("utf-8", errors="ignore").strip() # Read line from serial port and convert into eligble text 

        if not line: # skip the rest of this and go back to the top 
            continue

        print(line) # Showcases raw data 

        if line.startswith("t,"): # Skip the header line the QUTY prints 
            continue

        parts = line.split(",") # Split line into seperate parts allowing for each value being accessed seperately 

        if len(parts) != 5: # If there are not 5 parts it means it is an incorrect print from the QUTY, skip. 
            continue

        # Convert text into numbers 
        try:
            t = int(parts[0])
            position = int(parts[1])
            velocity = int(parts[2])
            acceleration = int(parts[3])
            mode = int(parts[4])
        except ValueError:
            continue
        
        # Store the values 
        times.append(t)
        positions.append(position)
        velocities.append(velocity)
        accelerations.append(acceleration)
        modes.append(mode)


except KeyboardInterrupt: # Stop reading with CTRL + C 
    print("\nStopped reading.")

finally: # Close the serial port 
    ser.close()
    print("Serial port closed.")

if len(times) == 0: # For when no value is collected and something had gone wrong 
    print("No valid data received.")
else: # Create the plot 
    plt.plot(times, positions, label="Position")
    plt.plot(times, velocities, label="Velocity")
    plt.plot(times, accelerations, label="Acceleration")

    plt.xlabel("Time step")
    plt.ylabel("Value")
    plt.title("QUTy Flight Simulator Telemetry")
    plt.legend()
    plt.grid(True)
    plt.show()
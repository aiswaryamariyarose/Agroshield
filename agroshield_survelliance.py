import cv2
import serial
import time
from ultralytics import YOLO

# ------------------------
# Arduino setup
# ------------------------
arduino = serial.Serial('COM3', 9600, timeout=1)
time.sleep(2)  # Wait for Arduino reset

# ------------------------
# YOLO setup
# ------------------------
model = YOLO("yolov8n.pt")  # Replace with your model path

cap = cv2.VideoCapture(0)
if not cap.isOpened():
    print("Cannot open webcam")
    exit()

PERSON_THRESHOLD = 2  # Only trigger intrusion if more than 2 people

# ------------------------
# Main loop
# ------------------------
try:
    while True:
        ret, frame = cap.read()
        if not ret:
            print("Failed to grab frame")
            break

        # Run YOLO detection
        results = model.predict(frame)
        person_count = 0

        for r in results:
            for label in r.boxes.cls:  # class labels
                if int(label) == 0:  # YOLO 'person' class is 0
                    person_count += 1

        print(f"{person_count} persons detected")

        # Intrusion logic
        if person_count > PERSON_THRESHOLD:
            print(f"INTRUSION detected: {person_count} persons")
            arduino.write(b'INTRUSION\n')  # H = intrusion
        else:
            arduino.write(b'CLEAR\n')  # C = clear

        # Show frame
        cv2.imshow("AgroShield Surveillance", frame)
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

except KeyboardInterrupt:
    print("Stopped by user")

finally:
    cap.release()
    cv2.destroyAllWindows()
    arduino.close()
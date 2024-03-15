import cv2

# Initialize video capture object with webcam ID (usually 0)
cap = cv2.VideoCapture(0)

# Check if webcam is opened successfully
if not cap.isOpened():
    print("Error opening webcam")
    exit()

# Capture frame-by-frame
while True:
    # Capture frame from webcam
    ret, frame = cap.read()

    if not ret:
        print("Can't receive frame (stream end?). Exiting...")
        break

    # Display the resulting frame
    cv2.imshow('Webcam', frame)

    # Wait for 'q' key to press to stop
    if cv2.waitKey(1) == ord('q'):
        break

# Release the capture and close all windows
cap.release()
cv2.destroyAllWindows()

print("Taking picture...")

# Capture a frame to save as an image
ret, frame = cap.read()

# Check if frame is captured successfully
if not ret:
    print("Can't capture frame for image")
    exit()

# Save the frame as an image (replace 'image.jpg' with your desired filename)
cv2.imwrite("image.jpg", frame)

print("Picture captured and saved as image.jpg")

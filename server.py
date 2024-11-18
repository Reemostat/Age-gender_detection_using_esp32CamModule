from flask import Flask, request, jsonify
import cv2
import numpy as np
import matplotlib.pyplot as plt

app = Flask(__name__)

face_net = cv2.dnn.readNetFromTensorflow('opencv_face_detector_uint8.pb', 'opencv_face_detector.pbtxt')

# Load the pre-trained models
age_net = cv2.dnn.readNetFromCaffe('age_deploy.prototxt', 'age_net.caffemodel')
gender_net = cv2.dnn.readNetFromCaffe('gender_deploy.prototxt', 'gender_net.caffemodel')

# Define model attributes
AGE_LIST = ['(0-2)', '(4-6)', '(8-12)', '(15-20)', '(25-32)', '(38-43)', '(48-53)', '(60-100)']
GENDER_LIST = ['Male', 'Female']

@app.route('/')
def home():
    return "Flask server is running!"


@app.route('/classify', methods=['POST'])
def classify_image():
    file = request.data
    nparr = np.frombuffer(file, np.uint8)
    
    img = cv2.imdecode(nparr, cv2.IMREAD_COLOR)
    cv2.imwrite("Sample1.jpeg", img)
    print(img)
    # Prepare the blob for face detection
    blob = cv2.dnn.blobFromImage(img, scalefactor=1.0, size=(300, 300), mean=(104.0, 177.0, 123.0), swapRB=False, crop=False)

    print("BLOB: ", blob)
    # Perform face detection
    face_net.setInput(blob)
    detections = face_net.forward()

    # Iterate through detected faces
    for i in range(detections.shape[2]):
        confidence = detections[0, 0, i, 2]
        if confidence > 0.5:  # Confidence threshold
            box = detections[0, 0, i, 3:7] * np.array([img.shape[1], img.shape[0], img.shape[1], img.shape[0]])
            (startX, startY, endX, endY) = box.astype("int")

            # Extract the face region
            face = img[startY:endY, startX:endX]

            # Preprocess the face for age and gender detection
            face_blob = cv2.dnn.blobFromImage(face, scalefactor=1.0, size=(227, 227), mean=(78.4263377603, 87.7689143744, 114.895847746),
                                              swapRB=False, crop=False)
            print("Detected Face (BLOB): ", face_blob)
            # Predict gender 
            # Face blob input is now given to the gender_net and age_net models.
            gender_net.setInput(face_blob)
            gender_preds = gender_net.forward()
            gender = GENDER_LIST[gender_preds[0].argmax()]

            print()

            # Predict age
            age_net.setInput(face_blob)
            age_preds = age_net.forward()
            age = AGE_LIST[age_preds[0].argmax()]

            return jsonify({'age': age, 'gender': gender})

    return jsonify({'error': 'No face detected'})

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)

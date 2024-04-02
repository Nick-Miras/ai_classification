from flask import Flask, request, make_response
import os

app = Flask(__name__)

# Define a directory to save uploaded images
ALLOWED_EXTENSIONS = {'png', 'jpg', 'jpeg'}


def allowed_file(filename):
    """
    This function checks if the uploaded file extension is allowed.
    """
    return '.' in filename and filename.rsplit('.', 1)[1].lower() in ALLOWED_EXTENSIONS


@app.route('/api/upload', methods=['POST'])
def upload_image():
    """
    This route handles the POST request for image upload.
    """
    # Check if request has a file part
    if 'image' not in request.files:
        return make_response({'message': 'No file uploaded'}, 400)

    file = request.files['image']

    # Check if the file is empty
    if file.filename == '':
        return make_response({'message': 'No selected file'}, 400)

    if file and allowed_file(file.filename):
        # Create upload folder if it doesn't exist
        os.makedirs(UPLOAD_FOLDER, exist_ok=True)

        # Generate a unique filename
        filename = f"{file.filename}"

        # Save the file
        file.save(os.path.join(UPLOAD_FOLDER, filename))

        # Send a success response
        return make_response({'message': f'Image uploaded successfully as {filename}'}, 200)

    # Return error if file type is not allowed
    return make_response({'message': 'Allowed file types: png, jpg, jpeg, gif'}, 400)

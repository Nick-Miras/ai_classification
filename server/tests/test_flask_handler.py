from flask import Flask, request, make_response, jsonify
import io
from PIL import Image

app = Flask(__name__)


@app.route('/upload', methods=['POST'])
def receive_framebuffer():
    # Get the raw data from the request
    framebuffer_data = request.get_data()
    try:
        image = Image.open(io.BytesIO(framebuffer_data))
        image.save('file.jpg')
        return make_response({'message': {"isWasteBiodegradable": True}}, 200)
    except Exception as e:
        return make_response({'message': e}, 400)


@app.route('/upload', methods=['POST'])
def upload_image():
    """
    This route handles the POST request for image upload.
    """
    # Check if request has a file part
    if 'image' not in request.files:
        return make_response({'message': 'No file uploaded'}, 400)

    image = request.files['image']
    # Check if the file is empty
    if image.filename == '':
        return make_response({'message': 'No selected file'}, 400)

    try:
        image.save('file.jpg')
        return make_response({'message': 'Image Received'}, 200)
    except Exception as e:
        return make_response({'message': e}, 400)


@app.after_request
def after(response):
    print(response.get_data())
    return response


if __name__ == "__main__":
    app.run(debug=True, host='0.0.0.0')

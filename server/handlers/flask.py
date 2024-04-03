from flask import Flask, request, make_response
import io

from image_handler import NodeMcuAIImageHandler

app = Flask(__name__)


@app.route('/api/upload', methods=['POST'])
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
        image_handler = NodeMcuAIImageHandler(image)
        return make_response({'message': image_handler.get_type_of_waste().value}, 200)
    except Exception as e:
        return make_response({'message': e}, 400)


def main():
    app.run(debug=True)

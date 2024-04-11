from flask import Flask, request, make_response
import io
from PIL import Image

app = Flask(__name__)


@app.route('/receive_framebuffer', methods=['POST'])
def receive_framebuffer():
    # Get the raw data from the request
    framebuffer_data = request.get_data()
    try:
        image = Image.open(io.BytesIO(framebuffer_data))
        image.save('file.jpg')
        return make_response({'message': 'Image Received'}, 200)
    except Exception as e:
        return make_response({'message': e}, 400)


if __name__ == "__main__":
    app.run(debug=True)

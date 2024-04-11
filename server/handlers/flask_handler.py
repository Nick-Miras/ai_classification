from flask import Flask, request, make_response
import io

from image_handler import NodeMcuAIImageHandler

app = Flask(__name__)


@app.route('/receive_framebuffer', methods=['POST'])
def receive_framebuffer():
    # Get the raw data from the request
    framebuffer_data = request.get_data()
    try:
        image_handler = NodeMcuAIImageHandler(io.Bytes(framebuffer_data))
        return make_response({'message': image_handler.get_type_of_waste().value}, 200)
    except Exception as e:
        return make_response({'message': e}, 400)


def main():
    app.run(debug=True)

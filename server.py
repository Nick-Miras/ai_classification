"""
HTTP server for receiving images from ESP32 Camera
"""
from http.server import BaseHTTPRequestHandler, HTTPServer
from PIL import Image
import io
from abc import ABC, abstractmethod
from image_handler import NodeMcuAIImageHandler
import json


PORT = 8000


class ServerCommands(ABC):

    @staticmethod
    @abstractmethod
    def execute(handler: BaseHTTPRequestHandler):
      ...


class PostCommand(ServerCommands):
    @staticmethod
    def execute(handler: BaseHTTPRequestHandler):
        # Get the content length from the header
        content_length = int(handler.headers.get('Content-Length'))

        # Read the image data
        image_data = handler.rfile.read(content_length)

        # process image
        try:
            image_handler = NodeMcuAIImageHandler(Image.open(io.BytesIO(image_data)))
            image_handler.execute()

            handler.send_response(200)
            handler.send_header('Content-Type', 'application/json')
            handler.end_headers()
            handler.wfile.write(json.dumps({'waste type': image_handler.get_type_of_waste().value}).encode())
        except Exception as e:
            print(f"Error saving image: {e}")
            handler.send_error(500, "Internal Server Error")      

    
class ImageHandler(BaseHTTPRequestHandler):

  def do_POST(self):
    PostCommand.execute(self)


def start_server():
    print(f"Server listening on port {PORT}")
    server_address = ('', PORT)
    httpd = HTTPServer(server_address, ImageHandler)
    httpd.serve_forever()

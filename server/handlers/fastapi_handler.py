import io
from fastapi import FastAPI, Request
from image_handler import NodeMcuAIImageHandler
from ai import Resnet152, Resnet50

app = FastAPI()


@app.post('/uploads')
async def process_image(request: Request):
    # Get the raw data from the request
    raw_bytes = await request.body()
    image_handler = NodeMcuAIImageHandler(io.BytesIO(raw_bytes), Resnet50())
    return {'message': {'isWasteBiodegradable': image_handler.get_type_of_waste().value == 'Biodegradable'}}

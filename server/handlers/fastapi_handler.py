from fastapi import FastAPI, HTTPException, Request
from PIL import Image
import io

from dataclass import TypeOfWaste, is_waste_biodegradable
from exceptions import BaseWasteClassificationException
from image_handler import NodeMcuAIImageHandler

app = FastAPI()


@app.post("/upload")
async def create_file(request: Request):
    try:
        raw_bytes = await request.body()
        image = Image.open(io.BytesIO(raw_bytes))
        image_handler = NodeMcuAIImageHandler(image)
        type_of_waste: TypeOfWaste = image_handler.get_type_of_waste()
        return {'message': {"isWasteBiodegradable": is_waste_biodegradable(type_of_waste)}}
    except BaseWasteClassificationException as e:
        raise HTTPException(400, e.error_message)

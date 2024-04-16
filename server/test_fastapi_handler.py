from fastapi import FastAPI, HTTPException, Request
from PIL import Image
import io

from exceptions import BaseWasteClassificationException

app = FastAPI()


@app.post("/upload")
async def create_file(request: Request):
    try:
        raw_bytes = await request.body()
        image = Image.open(io.BytesIO(raw_bytes))
        image.save("tests/file.jpg")
        return {'message': {"isWasteBiodegradable": True}}
    except BaseWasteClassificationException as e:
        raise HTTPException(400, e.error_message)

from ultralytics import YOLO
from PIL import Image

model = YOLO('../models/yolov8m-seg.pt') 
result, = model.predict(Image.open('image.jpg'), save=False, stream=True)
print(result)

if (probabilities := result.probs) is None:
    raise ValueError('NULL Result')
else:
    match(result.probs.top1):
        case 0 | 1 | 3:
            print('Non-Biodegradable')
        case 2 | 4:
            print('Biodegradable')
        case _:
            raise ValueError('Invalid Waste Type')

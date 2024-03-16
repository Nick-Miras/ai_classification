"""
Mediator For Image Handling
"""
from dataclass import TypeOfWaste
from PIL import Image
from ultralytics import YOLO
from exceptions import NULLResult, InvalidResult
from typing import final

class NodeMcuAIImageHandler:
    def __init__(self, image: Image) -> None:
        self.image = image

    def _determine_type_of_waste(self) -> TypeOfWaste:
        """Uses AI To Determine Waste"""
        model = YOLO('models/yolov8m-seg.pt') 
        result, = model.predict(self.image, save=False, stream=True)
        if (probabilities := result.probs) is None:
            raise NULLResult()
        else:
            match(probabilities):
                case 0 | 1 | 3:
                    return TypeOfWaste.NON_BIODEGRADABLE
                case 2 | 4:
                    return TypeOfWaste.BIODEGRADABLE
                case _:
                    raise InvalidResult()
    
    @final
    def get_type_of_waste(self) -> TypeOfWaste:
        return self._determine_type_of_waste()

    def execute(self):
        ...

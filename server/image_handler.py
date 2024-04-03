"""
Mediator For Image Handling
"""
from ai import TensorFlowModel, YoloV8Model
from dataclass import TypeOfWaste
from PIL import Image
from exceptions import NULLResult
from typing import final


class NodeMcuAIImageHandler:
    def __init__(self, image) -> None:
        self.image: Image = Image.open(image)

    def _determine_type_of_waste(self) -> TypeOfWaste:
        """Uses AI To Determine Waste"""
        if YoloV8Model.predict(self.image) is True:
            return TensorFlowModel.predict(self.image)
        raise NULLResult()
    
    @final
    def get_type_of_waste(self) -> TypeOfWaste:
        return self._determine_type_of_waste()

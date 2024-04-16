"""
Mediator For Image Handling
"""
from ai import TensorFlowModel, YoloV8Model
from dataclass import TypeOfWaste
from PIL import Image
from exceptions import NULLResult
from typing import final


class NodeMcuAIImageHandler:
    def __init__(self, image: Image) -> None:
        self.image: Image = image

    def _determine_type_of_waste(self) -> TypeOfWaste:
        """Uses AI To Determine Waste"""
        return TensorFlowModel.predict(self.image)
    
    @final
    def get_type_of_waste(self) -> TypeOfWaste:
        return self._determine_type_of_waste()

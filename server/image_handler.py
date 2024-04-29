"""
Mediator For Image Handling
"""
from ai import AIModel
from dataclass import TypeOfWaste
from PIL import Image
from typing import final


class NodeMcuAIImageHandler:
    def __init__(self, image: Image, model_handler: AIModel) -> None:
        self.image: Image = image
        self.model_handler = model_handler

    def _determine_type_of_waste(self) -> TypeOfWaste:
        """Uses AI To Determine Waste"""
        return self.model_handler.predict(self.image)
    
    @final
    def get_type_of_waste(self) -> TypeOfWaste:
        return self._determine_type_of_waste()

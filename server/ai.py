from abc import ABC, abstractmethod
from PIL import Image

from dataclass import TypeOfWaste, CLASSES

import tensorflow as tf


class AIModel(ABC):
    def __init__(self):
        self.model = tf.keras.models.load_model(self.model_path)

    @property
    @abstractmethod
    def model_path(self) -> str:
        ...

    @abstractmethod
    def predict(self, data: Image) -> TypeOfWaste:
        ...


class BinaryClassificationCNNModel(AIModel, ABC):

    def predict(self, data: Image) -> TypeOfWaste:
        """
        determines type of waste in the image
        Returns:
            :TypeOfWaste: classification of the input Image.
        """
        img_array = tf.expand_dims(data, 0)
        predictions = self.model.predict(img_array)
        return TypeOfWaste(CLASSES[int(tf.round(predictions[0]).numpy()[0])])


class Resnet50(BinaryClassificationCNNModel):

    @property
    def model_path(self) -> str:
        return 'models/resnet50.keras'


class Resnet152(BinaryClassificationCNNModel):
    @property
    def model_path(self) -> str:
        return 'models/resnet152.keras'

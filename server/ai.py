from abc import ABC, abstractmethod
from PIL import Image
import os

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
        img_array = tf.keras.preprocessing.image.img_to_array(data)
        img_array_resized = tf.image.resize(img_array, (224, 224), tf.image.ResizeMethod.NEAREST_NEIGHBOR)
        img = tf.expand_dims(img_array_resized, 0)
        predictions = self.model.predict(img)
        return TypeOfWaste(CLASSES[int(tf.round(predictions[0]).numpy()[0])])


class Resnet50(BinaryClassificationCNNModel):
    @property
    def model_path(self) -> str:
        return 'models/resnet50.keras'


class Resnet152(BinaryClassificationCNNModel):
    @property
    def model_path(self) -> str:
        return 'models/resnet152.keras'

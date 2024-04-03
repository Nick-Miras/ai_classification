from abc import ABC, abstractmethod
from PIL import Image
from ultralytics import YOLO

from dataclass import TypeOfWaste

import tensorflow as tf
import numpy as np

classes = ["Biodegradable", "Non-Biodegradable"]


class AIModel(ABC):

    @property
    @abstractmethod
    def model(self):
        ...

    @classmethod
    @abstractmethod
    def predict(cls, data: Image):
        ...


class TensorFlowModel(AIModel):
    model = tf.keras.models.load_model('models/model.keras')

    @classmethod
    def predict(cls, data: Image):
        """determines type of waste in the image"""
        img_array = tf.expand_dims(data, 0)
        predictions = cls.model.predict(img_array)
        return TypeOfWaste(classes[np.argmax(predictions)])


class YoloV8Model(AIModel):
    model = YOLO('models/yolov8m-segmentation-turhancan.pt')

    @classmethod
    def predict(cls, data: Image) -> bool:
        """determines whether waste is present in the image"""
        results = cls.model.predict(data, imgsz=256, show=False, save=False)
        return not (len(results) == 1 and results[0].boxes.data.shape[0] == 0)

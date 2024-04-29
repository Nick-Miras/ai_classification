from enum import Enum

CLASSES = ["Biodegradable", "Non-Biodegradable"]


class TypeOfWasteFromModel(Enum):
    GLASS = 0
    METAL = 1
    PAPER = 2
    PLASTIC = 3
    WASTE = 4


class TypeOfWaste(Enum):
    BIODEGRADABLE = 'Biodegradable'
    NON_BIODEGRADABLE = 'Non-Biodegradable'


def is_waste_biodegradable(type_of_waste: TypeOfWaste) -> bool:
    return type_of_waste is TypeOfWaste.BIODEGRADABLE



from enum import Enum


class TypeOfWasteFromModel(Enum):
    GLASS = 0
    METAL = 1
    PAPER = 2
    PLASTIC = 3
    WASTE = 4


class TypeOfWaste(Enum):
    BIODEGRADABLE = 'Biodegradable'
    NON_BIODEGRADABLE = 'Non-Biodegradable'

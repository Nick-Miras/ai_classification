

class BaseWasteClassificationException(Exception):
    pass


class AIException(BaseWasteClassificationException):
    pass


class NULLResult(AIException):
    pass


class InvalidResult(AIException):
    pass

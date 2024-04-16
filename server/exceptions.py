

class BaseWasteClassificationException(Exception):
    def __init__(self, error_message):
        self.error_message = error_message
        super().__init__(error_message)


class AIException(BaseWasteClassificationException):
    pass


class NULLResult(AIException):
    def __init__(self):
        super().__init__("NULL Result")


class InvalidResult(AIException):
    def __init__(self):
        super().__init__("Invalid Result")

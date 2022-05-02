class Coordinates:
    def __init__(self, x, y):
        self.x = x
        self.y = y

    def __eq__(self, other):
        if isinstance(other, Coordinates):
            if self.x == other.x:
                if self.y == other.y:
                    return True
        return False

    def __hash__(self):
        return hash((self.x, self.y))

    def __sub__(self, other):
        x = self.x - other.x
        y = self.y - other.y
        return Coordinates(x, y)

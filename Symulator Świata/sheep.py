from animal import Animal


class Sheep(Animal):

    def __init__(self, world, coord=None):
        self.name = 'o'
        self._strength = 4
        self._age = 0
        self._initiative = 4
        self.sprite = (255, 255, 255)
        self.world = world
        if coord is not None:
            world.board[coord] = self
        else:
            self.spawn()

    def createChild(self, coord):
        sheep = Sheep(self.world, coord)

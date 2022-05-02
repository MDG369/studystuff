from animal import Animal


class Wolf(Animal):

    def __init__(self, world, coord=None):
        self.name = 'w'
        self._strength = 7
        self._initiative = 5
        self._age = 0
        self.world = world
        self.sprite = (130, 130, 150)
        if coord is not None:
            world.board[coord] = self
        else:
            self.spawn()

    def createChild(self, coord):
        wolf = Wolf(self.world, coord)

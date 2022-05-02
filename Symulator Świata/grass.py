from plant import Plant


class Grass(Plant):

    def __init__(self, world, coord=None):
        self.name = 'g'
        self._strength = 0
        self._age = 0
        self._initiative = 0
        self.sprite = (0, 250, 0)
        self.world = world
        if coord is not None:
            world.board[coord] = self
        else:
            self.spawn()

    def createChild(self, coord):
        grass = Grass(self.world, coord)

from plant import Plant


class Nightshade(Plant):

    def __init__(self, world, coord=None):
        self.name = 'n'
        self._strength = 99
        self._initiative = 0
        self._age = 0
        self.sprite = (50, 50, 120)
        self.world = world
        if coord is not None:
            self.world.board[coord] = self
        else:
            self.spawn()

    def poisonous(self, coord, newCoord):
        self.world.commentator.addComment("{0} zjada trujaca rosline".format(self.world.board[coord].name))
        self.world.board.pop(coord)
        self.world.board.pop(newCoord)
        return True

    def createChild(self, coord):
        n = Nightshade(self.world, coord)
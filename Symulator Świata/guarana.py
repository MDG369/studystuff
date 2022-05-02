from plant import Plant
STRENGTH_INCREASE = 3


class Guarana(Plant):

    def __init__(self, world, coord=None):
        self.name = 'u'
        self._strength = 0
        self._initiative = 0
        self._age = 0
        self.sprite = (190, 30, 35)
        self.world = world
        if coord is not None:
            self.world.board[coord] = self
        else:
            self.spawn()

    def hasIncreasedStrength(self, coord):
        self.world.commentator.addComment("{0} zwieksza sile o {1}".format(self.world.board[coord].name,
                                                                           STRENGTH_INCREASE))
        self.world.board[coord].strength += STRENGTH_INCREASE
        return True

    def createChild(self, coord):
        g = Guarana(self.world, coord)
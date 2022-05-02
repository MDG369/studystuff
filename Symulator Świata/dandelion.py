import random
from plant import Plant
GROW_CHANCE = 30


class Dandelion(Plant):

    def __init__(self, world, coord=None):
        self.name = 'd'
        self._strength = 0
        self._initiative = 0
        self._age = 0
        self.sprite = (235, 235, 145)
        self.world = world
        if coord is not None:
            self.world.board[coord] = self
        else:
            self.spawn()

    def action(self, coord):
        i = 0
        while i < 3:
            i += 1
            growRoll = random.randint(0, 100)
            if growRoll <= GROW_CHANCE:
                self.multiply(coord)

    def createChild(self, coord):
        d1 = Dandelion(self.world, coord)
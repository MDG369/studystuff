import nightshade as ns
import cybersheep
from animal import Animal
import random
GROW_CHANCE = 30


class Sosnowsky(ns.Nightshade):

    def __init__(self, world, coord=None):
        self.name = 's'
        self._strength = 10
        self._initiative = 0
        self._age = 0
        self.sprite = (125, 150, 25)
        self.world = world
        if coord is not None:
            self.world.board[coord] = self
        else:
            self.spawn()

    def action(self, coord):
        self.destroySides(coord)
        growRoll = random.randint(0, 100)
        if growRoll <= GROW_CHANCE:
            self.multiply(coord)

    def destroySides(self, coord):
        newCoord = coord
        newCoord.x += 1
        self.destroySide(newCoord)
        newCoord.x -= 2
        self.destroySide(newCoord)
        newCoord.x += 1
        newCoord.y -= 1
        self.destroySide(newCoord)
        newCoord.y += 2
        self.destroySide(newCoord)
        newCoord.y -= 1

    def destroySide(self, newCoord):
        if newCoord in self.world.board:
            if isinstance(self.world.board[newCoord], Animal):
                if not isinstance(self.world.board[newCoord], cybersheep.Cybersheep):
                    self.world.commentator.addComment("Barszcz sosnowskiego zabija {0}"
                                                      .format(self.world.board[newCoord].name))
                    self.world.board.pop(newCoord)

    def createChild(self, coord):
        s = Sosnowsky(self.world, coord)
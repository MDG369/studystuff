from animal import Animal
import random
DEFLECT_STRENGTH = 5
MOVE_CHANCE = 25


class Turtle(Animal):

    def __init__(self, world, coord=None):
        self.name = 't'
        self._strength = 2
        self._initiative = 1
        self._age = 0
        self.sprite = (105, 180, 150)
        self.world = world
        if coord is not None:
            self.world.board[coord] = self
        else:
            self.spawn()

    def hasDeflectedAttack(self, coord, newCoord):
        if self.world.board[coord].strength < DEFLECT_STRENGTH:
            self.world.commentator.addComment("{0} odbija atak".format(self.world.board[newCoord].name))
            return True
        else:
            return False

    def Action(self, coord):
        n = random.randint(0, 100)
        if n < MOVE_CHANCE:
            newCoord = self.move(coord, 1)
            self.collision(coord, newCoord)

    def createChild(self, coord):
        turtle = Turtle(self.world, coord)
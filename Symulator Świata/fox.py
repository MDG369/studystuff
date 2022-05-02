from animal import Animal
from coordinates import Coordinates
import random


class Fox(Animal):

    def __init__(self, world, coord=None):
        self.name = 'f'
        self._strength = 3
        self._initiative = 7
        self._age = 0
        self.sprite = (255, 140, 40)
        self.world = world
        if coord is not None:
            self.world.board[coord] = self
        else:
            self.spawn()

    def move(self, coord, speed):
        x = coord.x
        y = coord.y
        cycled = False
        newCoord = Coordinates(x, y)
        dir = random.randint(0, 3)
        while True:
            if dir == 0:
                newCoord.y -= speed
                if newCoord.y < 0:
                    newCoord.y += speed * 2
            elif dir == 1:
                newCoord.x += speed
                if newCoord.x >= self.world.getColumns():
                    newCoord.x -= speed * 2
            elif dir == 2:
                newCoord.y += speed
                if newCoord.y >= self.world.getRows():
                    newCoord.y -= speed * 2
            elif dir == 3:
                newCoord.x -= speed
                if newCoord.x < 0:
                    newCoord.x += speed * 2
            if not self.checkIfStronger(newCoord):
                return newCoord
            newCoord = coord
            if dir < 3:
                dir += 1
            else:
                dir = 0
                if cycled:
                    break
                else:
                    cycled = True
        return coord

    def checkIfStronger(self, newCoord):
        if newCoord in self.world.board.keys():
            if not isinstance(self.world.board[newCoord], type(self)):
                if self.world.board[newCoord].strength > self._strength:
                    return True
                else:
                    return False
        else:
            return False

    def createChild(self, coord):
        fox = Fox(self.world, coord)
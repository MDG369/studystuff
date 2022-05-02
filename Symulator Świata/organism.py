from coordinates import Coordinates
import directions
import random
from abc import abstractmethod


class Organism:

    def __init__(self, orgWorld, coord=None):
        self._strength = 0
        self._initiative = 0
        self._age = 0
        self._sprite = None
        self.world = orgWorld

    @abstractmethod
    def action(self, coord):
        pass

    def endTurn(self):
        self._age += 1

    @property
    def initiative(self):
        return self._initiative

    @property
    def strength(self):
        return self._strength

    @property
    def age(self):
        return self._age

    @initiative.setter
    def initiative(self, value):
        self._initiative = value

    @strength.setter
    def strength(self, value):
        self._strength = value

    @age.setter
    def age(self, value):
        self._age = value

    def ranAway(self, coord, newCoord):
        return False

    def hasDeflectedAttack(self, coord, newCoord):
        return False

    def hasIncreasedStrength(self, coord):
        return False

    def poisonous(self, coord, newCoord):
        return False

    def poisonResistant(self):
        return False

    def spawn(self):
        if len(self.world.board) <= self.world.getColumns() * self.world.getRows():
            while True:
                coord = Coordinates(random.randint(0, self.world.getRows()-1), random.randint(0, self.world.getColumns() - 1))
                if coord not in self.world.board:
                    self.world.board[coord] = self
                    break

    def checkSurroundings(self, coord):
        x = coord.x
        y = coord.y
        newCoord = Coordinates(x, y)
        nClear = True
        eClear = True
        wClear = True
        sClear = True
        dir = random.randint(0, 3)
        while nClear & eClear & wClear & sClear:
            if dir == directions.N:
                newCoord.y -= 1
                if newCoord.y < 0:
                    newCoord.y += 1
                    nClear = False
                elif newCoord not in self.world.board.keys():
                    return newCoord
                else:
                    newCoord.y += 1
                    nClear = False
                    dir += 1
            if dir == directions.E:
                newCoord.x += 1
                if newCoord.x >= self.world.getColumns():
                    newCoord.x -= 1
                    eClear = False
                elif newCoord not in self.world.board.keys():
                    return newCoord
                else:
                    newCoord.x -= 1
                    eClear = False
                    dir += 1
            if dir == directions.S:
                newCoord.y += 1
                if newCoord.y >= self.world.getRows():
                    newCoord.y -= 1
                    sClear = False
                elif newCoord not in self.world.board.keys():
                    return newCoord
                else:
                    newCoord.y -= 1
                    sClear = False
                    dir += 1
            if dir == directions.W:
                newCoord.x -= 1
                if newCoord.x < 0:
                    newCoord.x += 1
                    wClear = False
                elif newCoord not in self.world.board.keys():
                    return newCoord
                else:
                    newCoord.x += 1
                    wClear = False
                    dir = 0
        return coord

    def multiply(self, coord):
        childCoord = self.checkSurroundings(coord)
        if childCoord != coord:
            child = self.createChild(childCoord)

    @abstractmethod
    def createChild(self, coord):
        pass

    def findCoordinates(self):
        if self in self.world.board.values():
            for key in self.world.board:
                if key in self.world.board.keys():
                    if self is self.world.board[key]:
                        return key

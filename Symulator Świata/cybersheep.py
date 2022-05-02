from animal import Animal
from coordinates import Coordinates
import sosnowsky


class Cybersheep(Animal):

    def __init__(self, world, coord=None):
        self.name = 'c'
        self._strength = 11
        self._initiative = 4
        self._age = 0
        self.sprite = (90, 90, 90)
        self.world = world
        if coord is not None:
            self.world.board[coord] = self
        else:
            self.spawn()

    def move(self, coord, speed):
        x = coord.x
        y = coord.y
        newCoord = Coordinates(x, y)
        sosnowskyCoord = self.findClosest(coord)
        if sosnowskyCoord:
            if coord.y > sosnowskyCoord.y:
                newCoord.y -= speed
                if newCoord.y < 0:
                    newCoord.y += speed * 2
            elif coord.x < sosnowskyCoord.x:
                newCoord.x += speed
                if newCoord.x >= self.world.getColumns():
                    newCoord.x -= speed * 2
            elif coord.y < sosnowskyCoord.y:
                newCoord.y += speed
                if newCoord.y >= self.world.getRows():
                    newCoord.y -= speed * 2
            elif coord.x > sosnowskyCoord.x:
                newCoord.x -= speed
                if newCoord.x < 0:
                    newCoord.x += speed * 2
        else:
            return super().move(coord, speed)
        return newCoord

    def findClosest(self, coord):
        sosnowskyExists = False
        cSheepCoord = Coordinates(coord.x, coord.y)
        minDistance: int = self.world.getRows() + self.world.getColumns()
        closest = Coordinates(self.world.getRows, self.world.getColumns)
        for key, value in self.world.board.items():
            if isinstance(value, sosnowsky.Sosnowsky):
                sosnowskyExists = True
                if abs(cSheepCoord.x - key.x) + abs(cSheepCoord.y - key.y) < minDistance:
                    minDistance = abs(cSheepCoord.x - key.x) + abs(cSheepCoord.y - key.y)
                    closest = key
        if sosnowskyExists:
            return closest
        else:
            return False

    def createChild(self, coord):
        c = Cybersheep(self.world, coord)

    def poisonResistant(self):
        return True

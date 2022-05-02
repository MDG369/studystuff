from animal import Animal
import random
RUN_CHANCE = 50


class Antelope(Animal):

    def __init__(self, world, coord=None):
        self.name = 'a'
        self._strength = 4
        self._initiative = 4
        self._age = 0
        self.sprite = (135, 70, 35)
        self.world = world
        if coord is not None:
            self.world.board[coord] = self
        else:
            self.spawn()

    def createChild(self, coord):
        a = Antelope(self.world, coord)

    def action(self, coord):
        newCoord = self.move(coord, 2)
        if newCoord != coord:
            self.collision(coord, newCoord)

    def ranAway(self, coord, newCoord):
        tmpCoord = newCoord
        free = False
        n = random.randint(0, 100)
        if n < RUN_CHANCE:
            newCoord2 = self.checkSurroundings(newCoord)
            if newCoord2 == newCoord:
                self.world.commentator.addComment("Antylopa nie ma gdzie uciec")
                return False
            else:
                self.world.commentator.addComment("Antylopa ucieka!")
                self.collision(tmpCoord, newCoord2)
                self.world.board[newCoord] = self.world.board[coord]
                self.world.board.pop(coord)
                return True
        else:
            self.world.commentator.addComment("Antylopie nie udalo sie uciec")
            return False

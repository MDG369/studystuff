import random
from organism import Organism
from coordinates import Coordinates


class Animal(Organism):

    def move(self, coord, speed):
        x = coord.x
        y = coord.y
        newCoord = Coordinates(x, y)
        dir = random.randint(0, 3)
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
        return newCoord

    def action(self, coord):
        newCoord = self.move(coord, 1)
        if newCoord != coord:
            self.collision(coord, newCoord)

    def collision(self, coord, newCoord):
        if newCoord in self.world.board.keys():  # jesli napotkano inny organizm
            if isinstance(self.world.board[newCoord], type(self)):  # sprawdzenie czy nastapi rozmnozenie
                if self.age != 0 and self.world.board[newCoord].age != 0:  # musi byc spelniony warunek wieku
                    self.multiply(newCoord)
            else:
                self.fight(coord, newCoord)
        else: # jesli nie napotkano innego organizmu
            self.world.board.pop(coord)
            self.world.board[newCoord] = self

    def fight(self, coord, newCoord):
        enemy = self.world.board[newCoord]
        if isinstance(self.world.board[newCoord], Animal):  # jeśli wrogim organizmem jest zwierzę to następuje walka
            if not enemy.ranAway(coord, newCoord):
                if not enemy.hasDeflectedAttack(coord, newCoord):
                    if self.strength >= enemy.strength:
                        self.world.commentator.addComment("{0} pokonal {1}".format(self.name, enemy.name))
                        self.world.board.pop(coord)
                        self.world.board[newCoord] = self
                    else:
                        self.world.commentator.addComment("{0} pokonal {1}".format(enemy.name, self.name))
                        self.world.board.pop(coord)
        else:  # jeśli wrogi organizm to roślina to jest ona zjadana i rozpatrywane są efekty jej zjedzenia
            if self.poisonResistant():  # odporność na truciznę
                enemy.hasIncreasedStrength(coord)
                self.world.commentator.addComment('{0} zjada {1}'.format(self.name, enemy.name))
                self.world.board[newCoord] = self.world.board[coord]
                self.world.board.pop(coord)
            elif not enemy.poisonous(coord, newCoord):
                enemy.hasIncreasedStrength(coord)
                self.world.commentator.addComment("{0} zjada {1}".format(self.name, enemy.name))
                self.world.board[newCoord] = self.world.board[coord]
                self.world.board.pop(coord)


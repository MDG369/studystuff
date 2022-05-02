from animal import Animal
from coordinates import Coordinates
import pygame
import random
SUPER_SPEED = 2
NORMAL_SPEED = 1


class Human(Animal):

    def __init__(self, world, coord=None):
        self.name = 'h'
        self._strength = 5
        self._initiative = 4
        self._age = 0
        self.sprite = (250, 225, 200)
        self.world = world
        self._superSpeedTimer = -5
        self.world.humanAlive = True
        if coord is not None:
            self.world.board[coord] = self
        else:
            self.spawn()

    def action(self, coord, keyPress):
        x = coord.x
        y = coord.y
        newCoord = Coordinates(x, y)
        speed = self.superSpeed()
        if keyPress == pygame.K_UP:
            newCoord.y -= speed
            if newCoord.y < 0:
                newCoord.y = 0
        elif keyPress == pygame.K_RIGHT:
            newCoord.x += speed
            if newCoord.x > self.world.getColumns() - 1:
                newCoord.x = self.world.getColumns() - 1
        elif keyPress == pygame.K_DOWN:
            newCoord.y += speed
            if newCoord.y > self.world.getRows() - 1:
                newCoord.y = self.world.getRows() - 1
        elif keyPress == pygame.K_LEFT:
            newCoord.x -= speed
            if newCoord.x < 0:
                newCoord.x = 0
        elif keyPress == pygame.K_SPACE:
            self.activateSuperSpeed()
        self.collision(coord, newCoord)

    def superSpeed(self):
        # Funkcja ustawiajaca odpowiednią szybkość człowieka
        speed = 1
        if self._superSpeedTimer > 2: # pierwsze trzy tury po użyciu umiejętności
            speed = SUPER_SPEED
        elif self._superSpeedTimer > 0: # ostatnie dwie tury po użyciu
            n = random.randint(0, 1)
            if n:
                speed = SUPER_SPEED
            else:
                speed = NORMAL_SPEED
        elif self._superSpeedTimer == 0: # koniec prędkości
            self.world.commentator.addComment("Szybkość antylopy sie skonczyła")
            speed = NORMAL_SPEED
        self._superSpeedTimer -= 1
        if self._superSpeedTimer < -6:
            self._superSpeedTimer = -5
        return speed

    def activateSuperSpeed(self):
        if self._superSpeedTimer < -5:
            self.world.commentator.addComment("Aktywowano szybkość antylopy")
            self._superSpeedTimer = 5
        elif self._superSpeedTimer < 0:
            self.world.commentator.addComment("Nie można aktywować zdolności")
            self.world.commentator.addComment("Pozostało/y {0} tur/y".format((self._superSpeedTimer + 6)))

    def createChild(self, coord):
        pass

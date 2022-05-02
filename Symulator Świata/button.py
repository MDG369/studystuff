import pygame


class Button:

    def __init__(self, box, textSurface, textPos, font):
        self.box = pygame.Rect(box)
        self.textSurface = font.render(textSurface[0], textSurface[1], textSurface[2])
        self.textPos = textPos

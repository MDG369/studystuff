import world
import pygame
TEXT_DISTANCE = 30 # odleglość między liniami tekstu w pikselach
# Zadaniem komentatora jest wypisywanie logów na ekran w trakcie rozgrywki
class Commentator:

    def __init__(self, world):
        self.world = world
        self.comments = []

    def commentate(self):
        font = pygame.font.Font(None, 28)
        j = 50
        pygame.draw.rect(self.world.screen, (0, 0, 0), (world.SCREEN_WIDTH- 350, 0, 350, world.SCREEN_HEIGHT))
        for i in self.comments:
            text = font.render(i, True, (255, 255, 255))
            textRect = text.get_rect()
            textRect.center = (world.SCREEN_WIDTH - 170, j)
            self.world.screen.blit(text, textRect)
            j += TEXT_DISTANCE
        self.comments.clear()

    def addComment(self, comment):
        self.comments.append(comment)
import random
import pygame
from commentator import Commentator
from grass import Grass
from sheep import Sheep
from wolf import Wolf
from fox import Fox
from guarana import Guarana
from nightshade import Nightshade
from sosnowsky import Sosnowsky
from antelope import Antelope
from human import Human
from dandelion import Dandelion
from turtle import Turtle
from cybersheep import Cybersheep
from coordinates import Coordinates
from button import Button

SCREEN_WIDTH = 1400
SCREEN_HEIGHT = 900


class World:

    def __init__(self, w=20, h=20):
        self._columns = w
        self._rows = h
        self.board = {}
        self.screen = pygame.display.set_mode([SCREEN_WIDTH, SCREEN_HEIGHT])
        self.commentator = Commentator(self)
        self.humanAlive = False
        random.seed()

    def drawWorld(self, w, h):
        self.commentator.commentate()
        for key in self.board:
            if key in self.board.keys():
                pygame.draw.rect(self.screen, self.board[key].sprite,
                                 ((SCREEN_WIDTH-100) * key.x * 0.8 / self._columns, SCREEN_HEIGHT * key.y / self._rows,
                                  (SCREEN_WIDTH - 600) / self._columns, (SCREEN_HEIGHT - 100) / self._rows))

    def gameMenu(self):
        pygame.init()
        pygame.display.set_caption("Symulator Świata- Michał Zieliński 184372")
        font = pygame.font.Font(None, 32)
        # Start
        # Load
        # Save
        # Quit
        startButton = Button(((SCREEN_WIDTH / 2)-70, SCREEN_HEIGHT / 6, 140, 100), ('Nowa gra', True, (255, 255, 255)),
                             ((SCREEN_WIDTH / 2) - 45, (SCREEN_HEIGHT / 6) + 40, 140, 100), pygame.font.Font(None, 32))
        loadButton = Button(((SCREEN_WIDTH / 2)-70, SCREEN_HEIGHT / 3, 140, 100), ('Wczytaj', True, (255, 255, 255)),
                            ((SCREEN_WIDTH / 2) - 40, (SCREEN_HEIGHT / 3) + 40, 140, 100), pygame.font.Font(None, 32))
        saveButton = Button(((SCREEN_WIDTH / 2)-70, SCREEN_HEIGHT / 2, 140, 100), ('Zapisz', True, (255, 255, 255)),
                            ((SCREEN_WIDTH / 2) - 40, (SCREEN_HEIGHT / 2) + 40, 140, 100), pygame.font.Font(None, 32))
        quitButton = Button(((SCREEN_WIDTH / 2)-70, SCREEN_HEIGHT / 1.5, 140, 100), ('Wyjdź', True, (255, 255, 255)),
                            ((SCREEN_WIDTH / 2) - 35, (SCREEN_HEIGHT / 1.5) + 40, 140, 100), pygame.font.Font(None, 32))
        title = Button(((SCREEN_WIDTH / 2)-70, SCREEN_HEIGHT / 20, 140, 100), ('Michał Zieliński 184372', True, (255, 255, 255)),
                            ((SCREEN_WIDTH / 2) - 120, (SCREEN_HEIGHT / 20) + 40, 140, 100), pygame.font.Font(None, 32))
        color = pygame.Color('lightskyblue3')
        done = False
        text = ''
        while not done:
            for event in pygame.event.get():
                self.screen.fill((30, 30, 30))
                if event.type == pygame.QUIT:
                    done = True
                elif event.type == pygame.MOUSEBUTTONDOWN:
                    if startButton.box.collidepoint(event.pos):
                        self.initializeBoard()
                        self.doTurn()
                    elif loadButton.box.collidepoint(event.pos):
                        self.loadOrSaveMenu(font, color, 'load')
                    elif saveButton.box.collidepoint(event.pos):
                        self.loadOrSaveMenu(font, color, 'save')
                    elif quitButton.box.collidepoint(event.pos):
                        pygame.quit()
                pygame.draw.rect(self.screen, color, quitButton.box, 4)
                pygame.draw.rect(self.screen, color, saveButton.box, 4)
                pygame.draw.rect(self.screen, color, startButton.box, 4)
                pygame.draw.rect(self.screen, color, loadButton.box, 4)
                self.screen.blit(saveButton.textSurface, saveButton.textPos)
                self.screen.blit(loadButton.textSurface, loadButton.textPos)
                self.screen.blit(startButton.textSurface, startButton.textPos)
                self.screen.blit(quitButton.textSurface, quitButton.textPos)
                self.screen.blit(title.textSurface, title.textPos)
                pygame.display.flip()
        pygame.quit()

    def loadOrSaveMenu(self, fontType, boxColor, loadOrSave):  # menu z możliwością wpisania nazwy pliku
        inputBox = Button(((SCREEN_WIDTH / 2)-70, SCREEN_HEIGHT / 6, 140, 30), ('', True, (255, 255, 255)),
                          ((SCREEN_WIDTH / 2) - 25, (SCREEN_HEIGHT / 6) + 40, 140, 100), pygame.font.Font(None, 32))
        color = boxColor
        font = fontType
        done = False
        text = ''
        titleText = ''
        if loadOrSave == 'load':
            titleText = 'Podaj nazwę pliku z którego ma być wczytana gra'
        elif loadOrSave == 'save':
            titleText = 'Podaj nazwę pliku do którego ma być zapisana gra'
        title = Button(((SCREEN_WIDTH / 2) - 70, SCREEN_HEIGHT / 20, 140, 100),
                       (titleText, True, (255, 255, 255)),
                       ((SCREEN_WIDTH / 2) - 220, (SCREEN_HEIGHT / 20) + 40, 140, 100), pygame.font.Font(None, 32))
        while not done:
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    done = True
                if event.type == pygame.KEYDOWN:
                    if event.key == pygame.K_RETURN:
                        done = True
                        if loadOrSave == 'load':  # przejście do odpowiedniej funkcji
                            self.loadState(text)
                        elif loadOrSave == 'save':
                            self.saveState(text)
                    elif event.key == pygame.K_BACKSPACE:
                        text = text[:-1]
                    else:
                        text += event.unicode
                self.screen.fill((30, 30, 30))
                txt_surface = font.render(text, True, color)
                width = max(200, txt_surface.get_width() + 10)
                inputBox.box.w = width
                self.screen.blit(txt_surface, (inputBox.box.x + 5, inputBox.box.y + 5))
                pygame.draw.rect(self.screen, color, inputBox.box, 4)
                self.screen.blit(title.textSurface, title.textPos)
                pygame.display.flip()
        pygame.quit()

    def getColumns(self):
        return self._columns

    def getRows(self):
        return self._rows

    def readInput(self, file):
        pass

    def doTurn(self):
        running = True
        while running:
            self.screen.fill((33, 50, 63))
            self.drawWorld(self.getColumns(), self.getRows())
            pygame.display.flip()
            moved = True
            while moved:  # zebranie inputu od gracza
                for event in pygame.event.get():
                    if event.type == pygame.QUIT:
                        running = False
                    if event.type == pygame.KEYDOWN:
                        if self.humanAlive:
                            if event.key == pygame.K_UP:
                                keyPress = event.key
                                moved = False
                            elif event.key == pygame.K_RIGHT:
                                keyPress = event.key
                                moved = False
                            elif event.key == pygame.K_DOWN:
                                keyPress = event.key
                                moved = False
                            elif event.key == pygame.K_LEFT:
                                keyPress = event.key
                                moved = False
                            elif event.key == pygame.K_SPACE:
                                keyPress = event.key
                                moved = False
                            elif event.key == pygame.K_ESCAPE:
                                self.gameMenu()
                        else:
                            if event.key == pygame.K_RETURN:
                                moved = False
                            elif event.key == pygame.K_ESCAPE:
                                self.gameMenu()
            moveOrder = list(self.board.values())  # utworzenie listy kolejności wykonywania tur
            moveOrder.sort(key=lambda x: (x.initiative, x.age), reverse=True)  # sortowanie listy
            for i in moveOrder:
                if i in self.board.values():
                    coord = i.findCoordinates()
                    if coord in self.board:
                        if isinstance(self.board[coord], Human):
                            self.board[coord].action(coord, keyPress)
                        else:
                            self.board[coord].action(coord)
            self.checkForHuman()
        pygame.quit()

    def initializeBoard(self):
        self.board.clear()
        g1 = Grass(self)
        g2 = Grass(self)
        o1 = Sheep(self)
        o2 = Sheep(self)
        o3 = Sheep(self)
        w1 = Wolf(self)
        w2 = Wolf(self)
        w3 = Wolf(self)
        w4 = Wolf(self)
        a1 = Antelope(self)
        a2 = Antelope(self)
        a3 = Antelope(self)
        f1 = Fox(self)
        f2 = Fox(self)
        f3 = Fox(self)
        g1 = Guarana(self)
        t1 = Turtle(self)
        t3 = Turtle(self)
        t2 = Turtle(self)
        h = Human(self)
        d1 = Dandelion(self)
        n1 = Nightshade(self)
        s1 = Sosnowsky(self)
        s2 = Sosnowsky(self)
        c1 = Cybersheep(self)
        c2 = Cybersheep(self)

    def loadState(self, fileName):
        # w pierwszych dwóch linijkach odpowiednio liczba rzędów i kolumn, w następnych poszczególne organizmy
        try:
            file = open(fileName, "r")
            i = 0
            self.board.clear()
            for line in file:
                if i == 0:
                    self._rows = int(line)
                    i += 1
                    continue
                if i == 1:
                    self._columns = int(line)
                    i += 1
                    continue
                else:
                    line = line.split()
                    if line[0] == 'h':
                        self.loadHuman(line)
                    else:
                        if line[0] == 'w':
                            self.loadOrganism(Wolf, line)
                        elif line[0] == 't':
                            self.loadOrganism(Turtle, line)
                        elif line[0] == 'o':
                            self.loadOrganism(Sheep, line)
                        elif line[0] == 'a':
                            self.loadOrganism(Antelope, line)
                        elif line[0] == 'c':
                            self.loadOrganism(Cybersheep, line)
                        elif line[0] == 'f':
                            self.loadOrganism(Fox, line)
                        elif line[0] == 'g':
                            self.loadOrganism(Grass, line)
                        elif line[0] == 's':
                            self.loadOrganism(Sosnowsky, line)
                        elif line[0] == 'd':
                            self.loadOrganism(Dandelion, line)
                        elif line[0] == 'n':
                            self.loadOrganism(Nightshade, line)
                        elif line[0] == 'u':
                            self.loadOrganism(Guarana, line)
        except IOError:
            print("nie ma takiego pliku")
            self.gameMenu()
        finally:
            file.close()
            self.doTurn()

    def saveState(self, fileName):
        try:
            file = open(fileName, "w+")
            file.write(str(self._rows) + "\n")
            file.write(str(self._columns) + "\n")
            for key, value in self.board.items():
                file.write(str(value.name) + ' ' + str(value.age) + ' ' + str(value.strength) + ' ' + str(key.x) + ' '
                           + str(key.y))
                if value.name == 'h':
                    file.write(' ' + value.superSpeedtimer)
                file.write("\n")
        except IOError:
            print("Taki plik juz istnieje")
            self.saveState()
        finally:
            file.close()
            self.doTurn()

    def loadOrganism(self, orgName, line):
        coord = Coordinates(int(line[3]), int(line[4]))
        org = orgName(self, coord)
        org.strength = int(line[2])
        org.age = int(line[1])

    def loadHuman(self, line):
        coord = Coordinates(int(line[3]), int(line[4]))
        org = Human(self, coord)
        org.strength = int(line[2])
        org.age = int(line[1])
        org._superSpeedTimer = int(line[5])

    def checkForHuman(self):
        self.humanAlive = False
        for value in self.board.values():  # sprawdzenie czy człowiek wciąż jest na planszy
            value.age += 1
            if not self.humanAlive:
                if isinstance(value, Human):
                    self.humanAlive = True
        if not self.humanAlive:
            self.commentator.addComment("Wcisnij Return by przejść ")
            self.commentator.addComment("do następnej tury")
            self.commentator.addComment("Wcisnij Escape aby wyjść do menu")
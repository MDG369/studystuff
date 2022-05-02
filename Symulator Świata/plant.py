from organism import Organism
import random
GROW_CHANCE = 30


class Plant(Organism):

    def action(self, coord):
        growRoll = random.randint(0, 100)
        if growRoll <= GROW_CHANCE:
            self.multiply(coord)

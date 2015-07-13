from random import sample
class team:
    def __init__(self):
        self.energy = 1
        pass

class state:
    def __init__(self):
        self.period = 0
        self.time = 0.0
        self.step_size = 0.5
        self.home_team = team()
        self.away_team = team()

    def next_period(self):
        self.period += 1
        self.time = 0.0


    def run(self):
        while self.period<3:
            self.faceoff_center()
            while self.time<20:
                self.compute_next_puck_potential()
                self.tire_teams()
                self.time += self.step_size
            self.next_period()

    def tire_teams(self):
        self.home_team.energy -= 1/(20*3*step_size)

    def compute_next_puck_potential(self):
        self.puck_potential += (sample([0.01,0.1,0.15, 0.25]) * sample([-1,-0.5,0,0.6,0.99]))
                               /self.step_size
        self.puck_potential = max(min(1,self.puck_potential),-1);

    def faceoff_center(self):
        self.puck_potential = 0 # -1 + 1 to indicate scoring potential
        self.posessing_team = self.faceoff(self.home_team, self.away_team)

    def faceoff(self, home_team, away_team ):
        return sample([home_team,away_team])



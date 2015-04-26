from .game                       import game

Game = None

def init():
    global Game
    Game = game()

def tick():
    Game.update()

def render():
    Game.render()


def finalize():
    pass

def configure( configuration ):
    pass

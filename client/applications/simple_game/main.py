from .game                       import game

Game = None

def init():
    global Game
    Game = game()
    print("-----INITIALIZED THE SIMPLE GAME------")

def tick():
    print("----- RUNNING THE UPDATE CYCLE -----")
    Game.update()

def render():
    print("----RENDERING---")
    Game.render()


def finalize():
    pass

def configure( configuration ):
    pass

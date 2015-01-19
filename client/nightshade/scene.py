import hwgfx
import json

class renderBrush(object):
    def __init__(self):
        self.r = [0.0,0.0,0,0.0]
        self.texture = ""
        self.colour = [0.0,0.0,0.0,0.0]


        pass

class collisionObject(object):
    def __init__(self):
        self.position = [0.0,0.0]
        self.vector = [0.0,0.0]
        pass


def loadScene(filename):
    try:
        with open(filename, 'r') as f:
            json_data = f.read()
            json_parsed = json.loads(json_data)
            print( json_parsed)
    except Exception as Error:
        print("error while loading scene:")
        print(Error)
        hwgfx.debug_displaykill()

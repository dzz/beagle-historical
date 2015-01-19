import hwgfx
import json

def loadScene(filename):
    try:
        with open(filename, 'r') as f:
            json = f.read()
            print(json)
        return json.loads(json)
    except:
        hwgfx.debug_displaykill()

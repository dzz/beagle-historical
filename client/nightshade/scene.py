import hwgfx
import json

class renderBrush(object):
    def __init__(self):
        self.r = [0.0,0.0,0,0.0]
        self.layer = 0
        self.texture = ""
        self.colour = [0.0,0.0,0.0]
        self.parralax = 1.0
        self.uv_mode = 0
        self_origin_mode = 0
        self.billboard = False
        self.shader = None
        self.sortOrder = 0
        self.uv_origin = [0.0,0.0]

class collisionObject(object):
    def __init__(self):
        self.position = [0.0,0.0]
        self.vector = [0.0,0.0]
        self.ramp_direction = 0
        pass


def buildRenderables(parsed):
    renderables = []
    for brush in parsed["brushes"]:

        if(brush["type"] == 0):

            renderable = renderBrush()
            renderable.r = [ brush["x"],brush["y"],brush["w"],brush["h"] ]
            renderable.billboard = brush["billboard"]
            renderable.layer = brush["z"]
            renderable.sortOrder = brush["sortOrder"]

            try:
                style = parsed["styles"][brush["styleName"]]
                renderable.texture  = style["texture"]
                renderable.parallax = style["parallax"]
                renderable.shader = style["shader"]
                renderable.colour = [ style["UIC_R"], style["UIC_G"],style["UIC_B"] ]
                renderable.uv_mode = style["uv_mode"]
                renderable.origin_mode = style["origin_mode"]
                renderable.uv_origin = [ style["origin_x"], style["origin_y"] ]

            except:
                pass

def loadScene(filename):
        with open(filename, 'r') as f:
            json_data = f.read()
            json_parsed = json.loads(json_data)
            buildRenderables(json_parsed)
            return json_parsed

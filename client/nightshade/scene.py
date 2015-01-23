import hwgfx
import json
import client.gfx.texture                   as Texture
import client.gfx.local_image               as LocalImage


class renderBrush(object):
    def __init__(self):
        self.r = [0.0,0.0,0,0.0]
        self.layer = 0
        self.texture = ""
        self.colour = [0.0,0.0,0.0,0.0]
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


def buildTextures(parsed):
    textures = {}
    styles = parsed["styles"]

    for styleKey in styles:
        print("nightshade parsing style:" + styleKey)
        style = styles[styleKey]
        textureKey = style["texture"]
        if textureKey is not None:
            if textureKey not in textures:
                print("nightshade loading tex:" + textureKey)
                img = LocalImage.local_image.from_file(textureKey)
                #textures[textureKey] = None
                textures[textureKey] = Texture.texture.from_local_image(img)


        



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
                renderable.colour = [ style["UIC_R"]/255., style["UIC_G"]/255.,style["UIC_B"]/255.,1.0 ]
                renderable.uv_mode = style["uv_mode"]
                renderable.origin_mode = style["origin_mode"]
                renderable.uv_origin = [ style["origin_x"], style["origin_y"] ]

            except:
                pass
            renderables.append( renderable )
    return renderables

class shadeScene(object):
    def __init__(self):
        self.renderables = []
        self.textures   = []
        self.unit_size = 32

    def applyScale(self):
        self.renderables= sorted( self.renderables, 
                key = lambda x: ( x.layer, x.r[1],x.sortOrder ) )

        for renderable in self.renderables:
            renderable.r[0]*=self.unit_size
            renderable.r[1]*=self.unit_size
            renderable.r[1]-=renderable.layer * self.unit_size
            renderable.r[2]*=self.unit_size
            renderable.r[3]*=self.unit_size


def loadScene(filename):
        with open(filename, 'r') as f:
            json_data = f.read()
            json_parsed = json.loads(json_data)
            newScene = shadeScene() 
            newScene.renderables = buildRenderables(json_parsed)
            newScene.textures = buildTextures(json_parsed)
            newScene.unit_size = json_parsed["world_unit_size"]
            newScene.applyScale()
            return newScene

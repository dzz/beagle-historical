import hwgfx
import json
import client.gfx.texture                   as Texture
import client.gfx.local_image               as LocalImage


class renderBrush(object):
    def __init__(self):
        self.r = [0.0,0.0,0,0.0]
        self.layer = 0
        self.textureKey = None
        self.texture = None
        self.colour = [0.0,0.0,0.0,0.0]
        self.parralax = 1.0
        self.uv_mode = 0
        self.origin_mode = 0
        self.billboard = False
        self.shader = None
        self.sortOrder = 0
        self.uv_origin = [0.0,0.0]
        self.scene = None
        self.uvs = None # note: format is x,y,w,h not x1,y1,x2,y2


    def normalizeUVs(self):

        if( self.texture is None):
           return

        # this operation assumes self.r is already 
        # transformed into screen/pixel space

        MODE_LOCAL = 0
        MODE_GLOBAL = 1
        
        MODE_WRAP = 0
        MODE_SCALE = 1

        if( self.origin_mode == MODE_GLOBAL ):
            pix_origin_x = self.uv_origin[0] + self.r[0] 
            pix_origin_y = self.uv_origin[0] + self.r[1]
        elif( self.origin_mode == MODE_LOCAL ):
            pix_origin_x = self.uv_origin[0]
            pix_origin_y = self.uv_origin[1]

        pix_span_x = self.r[2]
        pix_span_y = self.r[3]

        if( self.uv_mode == MODE_WRAP ):
            self.uvs = [
                           pix_origin_x / self.texture.w,
                           pix_origin_y / self.texture.h,
                           pix_span_x / self.texture.w,
                           pix_span_y / self.texture.h
                       ]
        elif( self.uv_mode == MODE_SCALE ):
            self.uvs = [
                           pix_origin_x / self.texture.w,
                           pix_origin_y / self.texture.h,
                           1,
                           1
                       ]


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
                textures[textureKey] = Texture.texture.from_local_image(img)


    return textures

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
                renderable.textureKey  = style["texture"]
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

    def linkScene(self):
        self.applyScale()
        for renderable in self.renderables:
            renderable.texture = self.textures[renderable.textureKey]
            renderable.scene = self
            renderable.normalizeUVs()


def loadScene(filename):
        with open(filename, 'r') as f:
            json_data = f.read()
            json_parsed = json.loads(json_data)
            newScene = shadeScene() 
            newScene.renderables = buildRenderables(json_parsed)
            newScene.textures = buildTextures(json_parsed)
            newScene.unit_size = json_parsed["world_unit_size"]
            newScene.linkScene()
            return newScene

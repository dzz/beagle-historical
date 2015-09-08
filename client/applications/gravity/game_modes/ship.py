from client.ctt2.assets import assets
from client.gfx.primitive import primitive

class ship_game:
    def __init__(self):
        pass
        #assets.load_package("intro")
        #self.star_texture = assets.get("intro/texture/star_background")
        #self.star_shader = assets.get("common/shader/no_transform")
        #self.primitive = primitive.get_unit_uv_primitive()

    def tick(self,context):
        #print("HI SHIPGAME")
        pass

    def render(self,context):
        pass
        #self.primitive.render_shaded( self.star_shader, { "texBuffer" : self.star_texture } )


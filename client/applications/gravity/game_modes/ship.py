from client.ctt2.assets import assets
from client.gfx.primitive import primitive

class ship_game:
    def __init__(self):
        assets.load_packages(["intro","Ship"])
        self.star_texture = assets.get("intro/texture/star_background")
        self.star_shader = assets.get("common/shader/passthru")
        self.primitive = primitive.get_unit_uv_primitive()

    def tick(self,context):
        pass

    def render(self,context):
        self.primitive.render_shaded( self.star_shader, { "texBuffer" : self.star_texture } )


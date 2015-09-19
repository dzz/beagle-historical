from client.ctt2.assets import assets

class poster:
    def __init__(self, view, postername, player, pos = [0.0,0.0]):
        self.shader = assets.get("common/shader/default_2d")
        self.texture = assets.get("sylab/texture/posters/" + postername)
        self.primitive = assets.get("core/primitive/unit_uv_square")
        self.view = view
        self.pos = pos
        self.player = player

    def get_shader_params(self,texture, color = [1,1,1,1]):
        return {
                 "texBuffer" : texture,
                 "translation_local"    : [0,0,0],
                 "scale_local"          : [1.0,2.0],
                 "translation_world"    : self.player.relative_point( self.pos, 1.0 ),
                 "scale_world"          : [1.0,1.0],
                 "view"                 : self.view,
                 "rotation_local"       : 0.0 ,
                 "filter_color"         : color,
                 "uv_translate"         : [0,0] }

    def render(self, color = [1,1,1,1]):
        self.primitive.render_shaded( self.shader, self.get_shader_params( self.texture, color ))


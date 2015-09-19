from client.ctt2.assets import assets

class chamber:
    def __init__(self,primitive, view):
        self.shader = assets.get("common/shader/default_2d")
        self.base_texture = assets.get("station/texture/chamber_base")
        self.door_texture = assets.get("station/texture/chamber_door")
        self.primitive = primitive
        self.view = view

    def get_shader_params(self,texture, color = [1,1,1,1]):
        return {
                 "texBuffer" : texture,
                 "translation_local"    : [0,0,0],
                 "scale_local"          : [1.2,1.5],
                 "translation_world"    : [0.0,1.3],
                 "scale_world"          : [1.0,1.0],
                 "view"                 : self.view,
                 "rotation_local"       : 0.0 ,
                 "filter_color"         : color,
                 "uv_translate"         : [0,0] }

    def render_base(self, color = [1,1,1,1]):
        self.primitive.render_shaded( self.shader, self.get_shader_params( self.base_texture, color ))

    def render_door(self, color = [1,1,1,1]):
        self.primitive.render_shaded( self.shader, self.get_shader_params( self.door_texture, color ))

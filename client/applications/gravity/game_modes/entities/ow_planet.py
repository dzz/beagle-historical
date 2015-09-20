from client.ctt2.assets import assets
from math import sin

class ow_planet:
    def __init__(self, view, player, enviro ):
        self.shader = assets.get("common/shader/default_2d")
        self.texture = assets.get("sylab/texture/bgplanet")
        self.primitive = assets.get("core/primitive/unit_uv_square")
        self.view = view
        self.player = player
        self.enviro = enviro
        self.seen = False

    def get_shader_params(self,texture, color = [1,1,1,1]):
        phase = 30.0
        scl = 1.25
        sway = -16


        color[3] =  (1.0 + self.enviro.get_rotated(self.player.uw_x, 1.0,phase))/2.0;
    
        return {
                 "texBuffer" : texture,
                 "translation_local"    : [-0.0,0],
                 "scale_local"          : [
                                            (10.0+(5.0+self.enviro.get_rotated(self.player.uw_x,-5.0,phase)))*scl,
                                            (10.0+(5.0+self.enviro.get_rotated(self.player.uw_x,-5.0,phase)))*scl
                                          ],
                 "translation_world"    : [self.enviro.get_rotated(self.player.uw_x, sway,phase),0.0],
                 "scale_world"          : [1.0,1.0],
                 "view"                 : self.view,
                 "rotation_local"       : 0.0 ,
                 "filter_color"         : color,
                 "uv_translate"         : [0,0] }

    def tick(self):
        if (self.player.x>15) or (self.player.x<-15):
            self.visible = True
            self.seen = True
        else:
            self.visible = False

    def render(self, color = [1,1,1,1]):
        if(self.visible):
            self.primitive.render_shaded( self.shader, self.get_shader_params( self.texture, [1.0,1.0,1.0,1.0] ))


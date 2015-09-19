from client.ctt2.assets import assets
from random import uniform, choice
from math import sin

class poster:
    def __init__(self, view, postername, player, pos = [0.0,0.0], trigger_thresh = 8):
        self.shader = assets.get("common/shader/default_2d")
        self.trigger_thresh = trigger_thresh
        self.texture = assets.get("sylab/texture/posters/" + postername)
        self.primitive = assets.get("core/primitive/unit_uv_square")
        self.view = view
        self.pos = pos
        self.player = player
        self.charge = 1.0
        self.charge_decay = 0.983
        self.quantum_charge_index = 0.0
        self.t = 0

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

    def tick(self):
        self.t+=1
        d = self.player.relative_point( self.pos, 1.0)[0]
        d = d*d
        if d < self.trigger_thresh:
            self.charge*=self.charge_decay

    def render(self, color = [1,1,1,1]):

        if (self.t % choice([24,30,60])) < 30:
            t = 1.0 - self.charge + uniform(-0.03,0.01)
        else:
            t = 1.0 - self.charge + (0.02*sin(self.t/40.0))

        color = [t,t*t*t,t*t,t] 
        self.primitive.render_shaded( self.shader, self.get_shader_params( self.texture, color ))


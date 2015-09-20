from client.ctt2.assets import assets
from random import uniform, choice
from math import sin,floor

class poster:
    def __init__(self, view, postername, player, pos = [0.0,0.0], trigger_thresh = 8, active = True):
        self.active = active
        self.shader = assets.get("common/shader/default_2d")
        self.trigger_thresh = trigger_thresh
        self.texture = assets.get("sylab/texture/posters/" + postername)
        self.inactive_texture = assets.get("sylab/texture/posters/inactive")
        self.primitive = assets.get("core/primitive/unit_uv_square")
        self.view = view
        self.pos = pos
        self.player = player
        self.charge = 1.0
        self.charge_decay = 0.953
        self.quantum_charge_index = 0.0
        self.t = 0
        self.viz_idktr = 0.0

    def get_shader_params(self,texture, color = [1,1,1,1],scale=1,prlx =1.0):

        return {
                 "texBuffer" : texture,
                 "translation_local"    : [0,0],
                 "scale_local"          : [1.0*scale*(1.0-self.charge),2.0*scale],
                 "translation_world"    : self.player.relative_point( self.pos, prlx ),
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
            self.pos[0]*=1.0 + (self.charge*0.0013)

    def render(self, color = [1,1,1,1]):

        if (self.t % choice([24,30,60])) < 30:
            t = 1.0 - self.charge + uniform(-0.03,0.01)
        else:
            t = 1.0 - self.charge + (0.02*sin(self.t/40.0))

        self.viz_idktr = max(0.0,min(floor(t*12) / 10.0,1.0))

        color = [t,t*t*t,t*t,t] 

        if self.active:
            texture = self.texture
        else:
            texture = self.inactive_texture

        if self.active:
            self.primitive.render_shaded( self.shader, self.get_shader_params( texture, [0.1*t*0.2,0.3*t*0.2,0.1*t*0.4,0.1*t], 2.5, 0.2 ))
        else:
            color = [ 0.0,t*0.25,t,1.0]

        self.primitive.render_shaded( self.shader, self.get_shader_params( texture, color, 1 ))


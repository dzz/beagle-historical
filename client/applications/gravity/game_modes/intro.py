from client.ctt2.assets import assets
from client.gfx.blend            import blendstate,blendmode
from client.math.helpers import lerp_vec
from client.math.curve import curve

class intro_game:
        def __init__(self):
            self.fg_texture = assets.get("intro/texture/planet_background")
            self.bg_texture = assets.get("intro/texture/fire_gradient")
            self.shader     = assets.get("common/shader/default_2d")
            self.view       = assets.get("common/coordsys/unit_square")
            self.config     = assets.get("intro/dict/sunrise_config")

            self.curves = {}

            for key in self.config["curves"]:
                self.curves[key] = curve(self.config["curves"][key])

            self.t          = 0.0

        def tick(self, context):
            self.t += 1.0/60.0

        def get_lerped(self,key):
            r = self.curves[key].value_at(self.t)
            return r

        def render(self, context):
            self.bg_texture.render_processed( self.shader, [], {
                                                            "translation_local" : [0,0],
                                                            "scale_local" : [1,1],
                                                            "translation_world": [0,0],
                                                            "scale_world" : [ 1,1],
                                                            "view"        : self.view,
                                                            "filter_color" : self.get_lerped("background_fade") } )

            with blendstate(blendmode.alpha_over):
                self.fg_texture.render_processed( self.shader, [], {
                                                                "translation_local" : [0,0],
                                                                "scale_local" : [1,1],
                                                                "translation_world": [0,0],
                                                                "scale_world" : [ 1,1],
                                                                "view"        : self.view,
                                                                "filter_color" : self.get_lerped("foreground_fade") } )

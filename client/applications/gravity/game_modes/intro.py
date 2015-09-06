from client.ctt2.assets import assets

class intro_game:
        def __init__(self):
            self.texture = assets.get("intro/texture/planet_background")
            self.shader  = assets.get("common/shader/default_2d")
            self.view    = assets.get("common/coordsys/unit_square")

        def tick(self, context):
            pass

        def render(self, context):
            self.texture.render_processed( self.shader, [], {
                                                            "translation_local" : [0,0],
                                                            "scale_local" : [1,1],
                                                            "translation_world": [0,0],
                                                            "scale_world" : [ 1,1],
                                                            "view"        : self.view,
                                                            "filter_color" : [1,1,1,1] } )

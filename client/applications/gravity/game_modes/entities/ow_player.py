from client.ctt2.assets import assets

class ow_player:
    def __init__(self, view ):
        self.config = assets.get("sylab/dict/player_config")
        self.walk_sequencer = assets.get("overworld_player/curve_sequence/walk_left")
        self.intro_sequencer = assets.get("overworld_player/curve_sequence/intro_float") 
        self.x = 0
        self.y = 0
        self.uw_x = 0
        self.walk_cfg = self.config["walk"]
        self.vx = 0.0

        self.primitive = assets.get("core/primitive/unit_uv_square")
        self.view = view
        self.shader = assets.get("common/shader/default_2d")
        self.walk_sequencer.tick() 
        self.mirror_walk = True
        self._is_walking = False

    def get_mirror_scale(self):
        if( self.mirror_walk):
            return [ -1.0,1.0 ]
        else:
            return [1.0, 1.0]

    def tick(self):
        if not self.intro_sequencer.is_finished():
            self.intro_sequencer.tick()
        else:
            self.handle_input()
            if self.is_walking():
                self.walk_sequencer.tick()

    def relative_point( self, point, parallax = 1.0 ):
        return [  point[0] - self.x, point[1] ]

    def handle_input(self):
        gp = assets.exec("core/queries/gamepad/find_primary")


        if(abs(gp.leftStick[0])>0.4):
            self.vx += gp.leftStick[0] * self.walk_cfg["smoothing"]
        self.vx = max( -1* self.walk_cfg["speed"], min( self.walk_cfg["speed"], self.vx) )

        self.vx*= self.walk_cfg["decay"]

        self.x += self.vx
        self.uw_x += self.vx
        if(abs(self.vx)>0.05):
            self._is_walking = True
        else:
            self._is_walking = False

        
        if(gp.leftStick[0]>0.25):
            self.mirror_walk = True
        elif(gp.leftStick[0]<-0.25):
            self.mirror_walk = False
        #else:
        #    self._is_walking = False
    
    def is_walking(self):
        return self._is_walking

    def get_shader_params(self):
        return {
            "texBuffer"            : assets.get( self.walk_sequencer.animated_value("texture_asset") ),
            "translation_local"    : [0.0,0.0],
            "scale_local"          : [1.2/2,1.5/2],
            "translation_world"    : [0,self.y + self.intro_sequencer.animated_value("float_down")[1] ],
            "scale_world"          : self.get_mirror_scale(),
            "view"                 : self.view,
            "rotation_local"       : 0.0 ,
            "filter_color"         : [1.0,1.0,1.0,1.0],
            "uv_translate"         : [0,0] }

    def render(self):
        self.primitive.render_shaded(self.shader, self.get_shader_params() )

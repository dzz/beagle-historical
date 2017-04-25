from client.beagle.assets import assets
from .terminals.saver import eaos_saver
from random import uniform

class ow_terminal:
    def __init__(self,view, ow_player,registers,modBuffer):
        assets.load_package("sylab")
        self.registers = registers
        self.modBuffer = modBuffer
        self.ow_player = ow_player
        self.view = view
        self.primitive = assets.get("core/primitive/unit_uv_square")
        self.sequencer = assets.get("sylab/curve_sequence/disp_warp") 
        self.parallax = 1.0
        self.application = eaos_saver(self.registers,self)
        self.uses_cursor = True
        self.handling_input = False
        self.was_learned = False
        self.t = 0.0
        self.x = 0.0
        self.help_decay = 1.0
        self.help_ticker = 0.0

    def signal_was_learned(self):
        self.was_learned = True

    def render_termapp(self):
        if self.application is not None:
            self.application.render()

    def get_terminal_buffer(self):
        return assets.reusable_from_factory( 
                                            factory ="core/factory/framebuffer/[w,h]",
                                            args = [384,256],
                                            key = "overworld/terminal_buffer" )

    def play_dist_scale(self):
        d = abs(self.ow_player.x - self.x ) 
        max_d = 30
        d_t = min( max( max_d - d, 0.0 ) / max_d, 1.0 )
        d_t*=d_t

        d_t*=0.1
        d_t+=0.9
        return d_t

    def get_head_shader_param(self):
        return {
            "texBuffer"            : self.get_terminal_buffer(),
            "modBuffer"            : self.modBuffer,
            "translation_local"    : [0.0,0.0],
            "scale_local"          : self.sequencer.animated_value("terminal_scale"),
            "translation_world"    : self.ow_player.relative_point( [self.x,0.7] ),
            "scale_world"          : [1.5*0.95,-1.0*0.95],
            "view"                 : self.view,
            "rotation_local"       : 0.0,
            "warp"                 : self.sequencer.animated_value("warp")[0],
            "term_cam"             : [self.x-(self.ow_player.x*self.parallax),0.0],
            "time"                 : self.t,
            "filter_color"         : self.sequencer.animated_value("filter_color"),
            "uv_translate"         : [0,0] }

    def render_help(self):
        if self.was_learned:
            return

        if self.application is not None and self.application.uses_cursor and self.handling_input:
            self.help_ticker += 1
            fbuf = assets.lazy_reusable(    factory = "core/factory/framebuffer/[w,h]", 
                                            args = [ 256,128],
                                            key = "terminal_tutorial" )
            with fbuf.as_render_target():
                with assets.get("core/hwgfx/blendmode/alpha_over"):
                    assets.exec_range("core/lotext/print(rows)[txt,[x,y],[r,g,b]]",
                            [
                                    [ "When in Terminal:                ",  [0, 0], [1,1,1] ] ,
                                    [ "- - - - - - - - -                ",  [0, 1], [1,1,1] ] ,
                                    [ "     .rightStick   =>cursor.mov  ",  [0, 2], [1,0.5,0.5] ] ,
                                    [ "     .rightTrigger =>cursor.clk  ",  [0, 3], [1,0.5,0.5] ] ,
                                    [ "     .buttons('A') =>cntext.oky  ",  [0, 4], [0.5,0.5,1] ] ,
                                    [ "     .buttons('B') =>cntext.bck  ",  [0, 5], [0.5,0.5,1] ] 
                            ])

            with assets.get("core/hwgfx/blendmode/alpha_over"):
                assets.get("core/primitive/unit_uv_square" ).render_shaded(
                                                                assets.get("common/shader/default_2d"), 
                                                                {
                                                                    "texBuffer" : fbuf,
                                                                    "translation_local" :[0.0,0.0],
                                                                    "scale_local" : [3.0*0.7,-2.0*0.7],
                                                                    "scale_world" : [1.0,1.0],
                                                                    "translation_world" : [self.ow_player.x-5.0,4.65],
                                                                    "view"        : assets.get("common/coordsys/16:9"),
                                                                    "rotation_local" : 0.0,
                                                                    "filter_color" : [1.0,1.0,1.0,self.help_decay*(20.0-(self.ow_player.x*self.ow_player.x))/20.0],
                                                                    "uv_translate" : [0.0,0.0]
                                                                } )

    def render(self):
        self.primitive.render_shaded( assets.get("sylab/shader/terminal"), self.get_head_shader_param() )
                                                                     
    def tick(self):
        if(self.help_ticker> 240):
            self.help_decay*=0.993

        self.t += 0.1
        if self.application is not None:
            d = (self.x - self.ow_player.x)
            d = d*d
            self.application.tick()
            input_mdist_threshold = 25
            if( d< input_mdist_threshold ):
                self.handling_input = True
                self.application.handle_input()
            else:
                self.handling_input = False
            if(self.application.finalized):
                self.application = self.application.next_application

        self.sequencer.tick()



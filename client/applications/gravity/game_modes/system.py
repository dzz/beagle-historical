from client.gfx.text import render_text
from client.gfx.blend            import blendstate,blendmode
from client.gfx.framebuffer import *
from client.gfx.coordinates import centered_view,Y_Axis_Down
import client.gfx.context as gfxcontext
from random import choice,uniform
from math import sin,cos
from client.system.gamepad       import get_gamepad

# YOU ARE A RASTER

def rand_unit():
        return uniform(-1.0,1.0)


def make_name(choices):
        def rotate(strg):
            n = choice([1,2,3,4,5,6])
            return strg[n:] + strg[:n]

        return "{0}-{1}".format( rotate(choice(choices)),rotate(choice(choices)))

class vsystem:
        sys_names = [ "RYPNO","hartWad","modulo","encrt","#fx4111","xfrm();","_xhdr();","t_frntlode","____|","pipe","stream","indice","itagr","__ito","__itx","__itr" ]

        def __init__(self):
                self.ox = rand_unit()
                self.oy = rand_unit()
                self.x = self.ox
                self.y = self.oy
                self.r = 0.0
                self.name = make_name(vsystem.sys_names)

        def tick(self):
            point = [self.x,self.y]
            r = 0.01
            self.x = ( point[0] * cos(r)) - (point[1]*sin(r))
            self.y = (point[0] * sin(r)) + (point[1]*cos(r))


class system_game:
        world_names = ["PYGRON",
                    "MEIRSLF",
                    "BADF00D",
                    "DEADB33F",
                    "FFFFFF",
                    "aeaeae",
                    "REFTREE",
                    "SHDRBLK",
                    "BLENDNG",
                    "YKNIFE",
                    "GPU",
                    "CPU",
                    "INTRUPT",
                    "CALLSTK",
                    "ptrPtrP",
                    "ref_cnt" ]

        def generate_worldname(self):
                def rotate(strg):
                    n = choice([1,2,3,4,5,6])
                    return strg[n:] + strg[:n]

                #self.world_name = "{0}-{1}".format( rotate(choice(system_game.world_names)),rotate(choice(system_game.world_names)))
                self.world_name = make_name(system_game.world_names)

        def __init__(self):
                self.label_buffer = framebuffer.from_dims(512,256,False)
                self.sys_buffer = framebuffer.from_dims(256,256,False) 
                self.comp_buffer = framebuffer.from_dims(960,540,False)
                self.generate_worldname()
                self.vsystems = [ vsystem(),vsystem(),vsystem() ]

        def tick(self,context):
                
                for vsystem in self.vsystems:
                        vsystem.tick()

                pad = get_gamepad(0)

                if(pad.triggers[0]>0.5):
                    context.trigger_mode_switch("vortex")

        def render(self,context):
            coordsys = centered_view(16.0,19.0, Y_Axis_Down )
            worldname_shader = shaders.get_client_program("no_transform","postfx/passthru")
            vsysname_shader = shaders.get_client_program("2d_transform","postfx/passthru")

            with render_target(self.comp_buffer):
                 gfxcontext.set_clear_color(0.0,0.0,0.0,0.0)
                 gfxcontext.clear()
                

            with render_target(self.label_buffer):
                    render_text("_node:{0}".format(self.world_name),0,0,[1.0,1.0,1.0])

            with render_target(self.comp_buffer):
                with blendstate(blendmode.alpha_over):
                    self.label_buffer.render_processed( worldname_shader )

            for vsystem in self.vsystems:
                with render_target(self.sys_buffer):
                        render_text(".[]:{0}".format(vsystem.name),90,124,[0.0,1.0,1.0])
                with render_target(self.comp_buffer):
                    with blendstate(blendmode.alpha_over):
                            self.sys_buffer.render_processed( vsysname_shader, [], [ 
                                                                                    ("translation_local",[0.0,0.0]),
                                                                                    ("scale_local", [1.0,2.0] ) ,
                                                                                    ("translation_world",[vsystem.x,vsystem.y]),
                                                                                    ("scale_world", [5.0,5.0] ),
                                                                                    ("view", coordsys ),
                                                                                    ("rotation_local", [vsystem.x] ),
                                                                                    ] )

            self.comp_buffer.render_processed( worldname_shader )


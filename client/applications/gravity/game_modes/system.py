from client.gfx.text import render_text
from client.gfx.blend            import blendstate,blendmode
from client.gfx.framebuffer import *
from client.gfx.coordinates import centered_view,Y_Axis_Down
from client.math.helpers import distance
import client.gfx.context as gfxcontext
from random import choice,uniform
from math import sin,cos,atan2
from client.system.gamepad       import get_gamepad

# YOU ARE A RASTER


def rand_unit():
        return uniform(-1.0,1.0)

class starfield:
        def __init__(self):
                self.num_stars = 250
                self.stars = []
                self.speed = 0.001
                self.sprd = 15
                for i in range(0,self.num_stars):
                        self.stars.append([rand_unit()*self.sprd,rand_unit()*self.sprd,uniform(0.0,1.0)])
                self.image = framebuffer.from_dims(8,8,False)
                with render_target(self.image):
                        gfxcontext.set_clear_color(0.0,0.0,0.0,0.2)
                        gfxcontext.clear()
                        render_text(".",0,0,[1.0,1.0,1.0])

        def tick(self):
                for star in self.stars:
                        star[2]+=self.speed
                        if(star[2]>1.0):
                                star[0]=rand_unit()*self.sprd
                                star[1]=rand_unit()*self.sprd
                                star[2]=0.0

        def render(self):
                coordsys = centered_view(16.0,9.0, Y_Axis_Down )
                star_shader = shaders.get_client_program("2d_transform","postfx/passthru_filter")
                for star in self.stars:
                    a = 15
                    b = 14.5
                    z = star[2]
                    z_pf = 0.001+z*100
                    x = (star[0]/z_pf)*16
                    y = (star[1]/z_pf)*9
                    z = star[2]
                    s_p = 10.0
                    scl = 0.001+(s_p-(z*s_p))
                    with blendstate(blendmode.alpha_over):
                        self.image.render_processed( star_shader, [ 
                                                                                ("translation_local",[0,0]),
                                                                                ("scale_local", [scl,scl] ) ,
                                                                                ("translation_world",[x,y]),
                                                                                ("scale_world", [1.0,1.0] ),
                                                                                ("view", coordsys ),
                                                                                ("rotation_local", [0.0] ),
                                                                                ("filter_color" , [1.0*z,1.0*z,1.0*z,1.0*z] )
                                                                                ] )



def make_name(choices):
        def rotate(strg):
            n = choice([1,2,3,4,5,6])
            return strg[n:] + strg[:n]

        return "{0}-{1}".format( rotate(choice(choices)),rotate(choice(choices)))

class vsystem:
        sys_names = [ "RYPNO","hartWad","modulo","encrt","#fx4111","xfrm","_xhdr","t_frntlode","___|","pipe","stream","indice","itagr","__ito","__itx","__itr" ]

        def __init__(self):
                self.color = choice([ [0.0,1.0,1.0],[1.0,0.0,0.0],[1.0,1.0,0.0] ])
                self.ox = rand_unit()
                self.oy = rand_unit()
                self.x = self.ox
                self.y = self.oy
                self.r = 0.0
                self.size = uniform(1.0,3.0)
                self.name = make_name(vsystem.sys_names)

        def tick(self):
            point = [self.x,self.y]
            d = (point[0]*point[0])+(point[1]*point[1])*3
            r = 0.001*d
            self.x = ( point[0] * cos(r)) - (point[1]*sin(r))
            self.y = (point[0] * sin(r)) + (point[1]*cos(r))


class sg_player:
        def __init__(self):
                self.x = 0.0
                self.y = 0.0
                self.vx = 0.0
                self.vy = 0.0
                self.image = framebuffer.from_dims(24,8,True)
                with render_target(self.image):
                        gfxcontext.set_clear_color(0.0,0.0,0.0,0.2)
                        gfxcontext.clear()
                        render_text("0x0",0,0,[1.0,1.0,1.0])


        def tick(self):
                pad = get_gamepad(0)

                r=0.01

                point = [self.x,self.y]
                self.x = ( point[0] * cos(r)) - (point[1]*sin(r))
                self.y = (point[0] * sin(r)) + (point[1]*cos(r))

                self.vx+=((pad.triggers[1]+1.0)*pad.left_stick[0])*0.02
                self.vy+=((pad.triggers[1]+1.0)*pad.left_stick[1])*0.02

                self.x+=self.vx*0.01
                self.y+=self.vy*0.01

                self.x*=0.999
                self.y*=0.999
                
                self.vx*=0.96
                self.vy*=0.96

        def render(self):
                coordsys = centered_view(16.0,9.0, Y_Axis_Down )
                player_shader = shaders.get_client_program("2d_transform","postfx/passthru_filter")
                with blendstate(blendmode.alpha_over):
                    r = atan2(self.vx,self.vy)
                    r = 0.0
                    self.image.render_processed( player_shader, [], [ 
                                                                            ("translation_local",[0,0]),
                                                                            ("scale_local", [0.3,0.1] ) ,
                                                                            ("translation_world",[self.x,self.y]),
                                                                            ("scale_world", [4.0,4.0] ),
                                                                            ("view", coordsys ),
                                                                            ("rotation_local", [r] ),
                                                                            ("filter_color" , [1.0,1.0,1.0,1.0] )
                                                                            ] )


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
                self.world_name = make_name(system_game.world_names)

        def __init__(self):
                self.player = sg_player()
                self.starfield = starfield()
                self.label_buffer = framebuffer.from_dims(512,256,False)
                self.sys_buffer = framebuffer.from_dims(64,64,False) 
                self.comp_buffer = framebuffer.from_screen();
                self.generate_worldname()
                self.vsystems = [ vsystem(),vsystem(),vsystem() ]

        def tick(self,context):
                self.player.tick()
                self.starfield.tick()
                
                for vsystem in self.vsystems:
                        vsystem.tick()

                pad = get_gamepad(0)

                #if(pad.triggers[0]>0.5):
                #    context.trigger_mode_switch("vortex")

        def render(self,context):

            
            coordsys = centered_view(16.0,9.0, Y_Axis_Down )
            worldname_shader = shaders.get_client_program("no_transform","postfx/passthru")
            comp_shader = shaders.get_client_program("no_transform","postfx/overworld_warp")
            vsysname_shader = shaders.get_client_program("2d_transform","postfx/passthru_filter")

            with render_target(self.comp_buffer):
                 gfxcontext.set_clear_color(0.0,0.0,0.0,0.0)
                 #gfxcontext.clear()
                 with blendstate(blendmode.alpha_over):
                    self.starfield.render()
                    self.player.render()
                

            with render_target(self.label_buffer):
                    render_text("_node:{0}".format(self.world_name),0,0,[1.0,1.0,1.0])

            with render_target(self.comp_buffer):
                with blendstate(blendmode.alpha_over):
                    self.label_buffer.render_processed( worldname_shader )

            for vsystem in self.vsystems:
                with render_target(self.sys_buffer):
                        render_text("();".format(vsystem.name),128-8,124,vsystem.color)
                        render_text("{0}".format(vsystem.name),128-32,140,vsystem.color)
                with render_target(self.comp_buffer):
                    self.player.render()
                    with blendstate(blendmode.alpha_over):
                            self.sys_buffer.render_processed( vsysname_shader, [ 
                                                                                    ("translation_local",[0.0,0.0]),
                                                                                    ("scale_local", [vsystem.size,vsystem.size])  ,
                                                                                    ("translation_world",[vsystem.x,vsystem.y]),
                                                                                    ("scale_world", [4.0,4.0] ),
                                                                                    ("view", coordsys ),
                                                                                    ("rotation_local", [vsystem.x*0.2] ),
                                                                                    ("filter_color",[vsystem.x,vsystem.y,1.0,1.0])
                                                                                    ] )

            dists = []
            for vsystem in self.vsystems:
                    dists.append( [vsystem.color, distance(vsystem.x,vsystem.y, self.player.x, self.player.y)])

            sum_d = 0.0
            for calc in dists:
                    sum_d+=calc[1]
            for calc in dists:
                    calc[1]/=sum_d+0.00000000001
            bg_color = [0.0,0.0,0.0,1.0]
            for calc in dists:
                    bg_color[0]+=calc[0][0]*calc[1]
                    bg_color[1]+=calc[0][1]*calc[1]
                    bg_color[2]+=calc[0][2]*calc[1]

            self.comp_buffer.render_processed( comp_shader, [], [ ("frequency",[12.0]), ("color",bg_color)] )

            #for vsystem in self.vsystems:
            #    with render_target(self.sys_buffer):
            #            render_text("();".format(vsystem.name),128-8,124,vsystem.color)
            #            render_text("{0}".format(vsystem.name),128-32,140,vsystem.color)
            #    with blendstate(blendmode.alpha_over):
            #            self.sys_buffer.render_processed( vsysname_shader, [], [ 
            #                                                                    ("translation_local",[0.0,0.0]),
            #                                                                    ("scale_local", [vsystem.size,vsystem.size])  ,
            #                                                                    ("translation_world",[vsystem.x,vsystem.y]),
            #                                                                    ("scale_world", [4.0,4.0] ),
            #                                                                    ("view", coordsys ),
            #                                                                    ("rotation_local", [vsystem.x*0.3]),
            #                                                                    ("filter_color",[1.0,1.0,1.0,1.0])
            #                                                                    ] )

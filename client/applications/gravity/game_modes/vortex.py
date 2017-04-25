from ..particle import particle
from random import choice, uniform, sample
from client.system.gamepad       import get_gamepad
from math import sqrt,atan2,cos,sin,floor
from client.math.helpers import distance
from client.gfx.texture import texture
from client.gfx.framebuffer import *
from client.gfx.blend            import blendstate,blendmode
from client.gfx import context
from client.gfx.text import render_text
import client.gfx.shaders as shaders

def tick_particles(particles,vortex, t):
    max_particles = 300
    tickset = t%4
    if(len(particles)>max_particles):
        particles = sample(particles,max_particles)
    new_particles = []

    idx = 0
    for part in particles:
        idx+=1
        if(idx%4==tickset):
            part.tick(vortex)
        if(part.ttl>0):
            new_particles.append(part)
    return new_particles

class vortex_game:
    def tick(this, self):
       self.player.points *= 0.9994
       particle.tick_sprites()
       self.t +=1
       pad = get_gamepad(0)
       for sprite in self.radar_sprites:
           sprite.tick()

       if(self.t%choice([3,5,9])==0 and self.player.firing>0):
           self.particles.append( particle( self.player.x - self.player.vx, self.player.y - self.player.vy, 0,0, self.player.r, "sprinkles"))

       if(self.t%choice([3,5,5,7])==0):
           # comet tail
           p_vec_x = self.pickup.x - self.player.x
           p_vec_y = self.pickup.y - self.player.y
           r = atan2(p_vec_x,p_vec_y)+uniform(-0.1,0.1)
           dist = distance( self.player.x, self.player.y, self.pickup.x,self.pickup.y )
           d = uniform((dist/12), (dist/220) )
           vx = -1*sin(r)*d
           vy = -1*cos(r)*d
           self.particles.append( particle( self.pickup.x, self.pickup.y, vx,vy, self.player.r, "comet_tail"))

       self.pickup.tick(self.particles, self.sprite_renderer, self.music_system )

       if(self.pickup.levelled):
           self.active_player_sprite = choice([self.player_sprite, self.alternate_player_sprite])
           self.jitter_radar_shows = choice([True,False, False])

       self.particles = tick_particles(self.particles,self.vortex, self.t)
       for bg in self.backgrounds:
           bg.tick(self.vortex.td_current, self.player.r)
           bg.x = self.player.x
           bg.y = self.player.y

       self.priming_sprite.tick()
       self.emerald_sprite.tick()
       self.music_system.tick()
       self.player.tick(pad,self.vortex,self.music_system)
       self.fire_sprite.tick()

       self.alternate_player_sprite.tick()

       a=0.98
       b=0.02
       self.world_zoom_current = (self.world_zoom_current*a)+(pad.triggers[0]*b)

    def render(this,self):

        hca = (sin (self.t/12.0)+1)/2.0
        hcb = (sin (self.t/24.0)+1)/2.0
        hcc = (sin (self.t/9.0)+1)/2.0

        wobble = (sin (self.t/12)+1)/2.0
        world_zoom_min = 0.2
        world_zoom_max = 0.8
        target_x = self.pickup.x-self.player.x
        target_y = self.pickup.y-self.player.y
        dist = sqrt(target_x*target_x+target_y*target_y)
        reticle_r = 3.14-atan2(target_x,target_y)
        world_zoom = world_zoom_max -(self.world_zoom_current*(world_zoom_max-world_zoom_min))

        # set up some batches 
        primary_batch       = [];
        distortion_batch    = []
        particle_batch      = []

        if(dist>10):
            #add our 'radar' sprites to the distortion batch
            (x1,y1,x2,y2) = (self.player.x,self.player.y,self.pickup.x,self.pickup.y)
            normalize_factor = 1.0 / (max(1,distance(x1,y1,x2,y2)) / 350)
            (x1,y1,x2,y2) = (x1*normalize_factor,y1*normalize_factor,x2*normalize_factor,y2*normalize_factor)

            radar_world_zoom = 1.0
            
            if self.pickup.radars_wobble:
                radar_world_zoom = wobble

            distortion_batch.append([self.radar_sprites[0], [0,-8], 12+wobble*2, -wobble, [x1,y1], radar_world_zoom ])
            distortion_batch.append([self.radar_sprites[1], [-8,-8], 8+wobble*4, wobble, [x2,y2], radar_world_zoom ])
            distortion_batch.append([self.alternate_player_sprite, [-8,-8], 20, reticle_r, [x1,y1], -1*radar_world_zoom ])

            if( self.jitter_radar_shows):
                distortion_batch.append([self.radar_sprites[ choice([0,0,0,0,0,1,1,1,2,2,3])], [0,-8], -50, 0-self.player.r, [x2,y2], -1*radar_world_zoom ])

        # add particles to a batch
        for part in self.particles:
            particle_batch.append([ part.sprite, [-8,-8], 4+(part.r*2), part.r, [part.x-self.player.x,part.y-self.player.y], world_zoom ]) 

        # add the target to the primary batch
        primary_batch.append([self.emerald_sprite, [-8,-8], (3+(wobble*3))*2, atan2(self.pickup.x,self.pickup.y), [self.pickup.x-self.player.x,self.pickup.y-self.player.y], world_zoom ])

        # add some state dependent sprites to the primary batch
        if(self.player.firing>0):
            if(self.player.real_acc>0.98):
                primary_batch.append([ self.fire_sprite, [-8,-2+self.player.acc*3], 6 + self.player.real_acc, self.player.eng_r, [0.0,0.0], world_zoom ])
            else:
                primary_batch.append([ self.priming_sprite, [-8,-6+self.player.acc], 16, self.player.eng_r, [0.0,0.0], world_zoom ])
            primary_batch.append([ self.engine_sprite, [-8,-8+self.player.acc], 4, self.player.eng_r, [0.0,0.0], world_zoom ])

        # add the player to the primary batch
        primary_batch.append([   self.active_player_sprite, [-8,-8], 10+(wobble*3)-(self.player.firing*2), self.player.r, [0.0,0.0], world_zoom ])

        # add the reticle to the primary batch
        if(dist>100):
            primary_batch.append([   self.reticle_sprite, [-8,-32], 4, reticle_r, [0,0], world_zoom ])

        ############################################
        # now we actually blast out the pixels...  #
        ############################################



        self.primary_buffer.bind_as_texture(texture.units[0])
        with render_target(self.distortion_buffer):
            with blendstate(blendmode.alpha_over):
                self.backgrounds[0].render(world_zoom)
                self.part_sprite_renderer.render(particle_batch)
            with blendstate(blendmode.add):
                self.radar_sprite_renderer.render(distortion_batch)
            with blendstate(blendmode.darken):
                self.hud_buffer.render_processed ( shaders.get_client_program( "no_transform","postfx/passthru") )
                
        with render_target(self.primary_buffer):
            with blendstate(blendmode.alpha_over):
                self.distortion_buffer.bind_as_texture(texture.units[0])
                for bg in self.backgrounds:
                    bg.render(world_zoom)
            with blendstate(blendmode.add):
                self.hud_buffer.render_processed ( shaders.get_client_program( "no_transform","postfx/passthru") )

            with blendstate(self.pickup.particle_blend_mode):
                self.part_sprite_renderer.render(particle_batch)

            with blendstate(blendmode.alpha_over):
                self.sprite_renderer.render(primary_batch)
                with render_target(self.distortion_buffer ):
                    with blendstate(blendmode.darken):
                        self.sprite_renderer.render(primary_batch)

        self.primary_buffer.render_processed( self.postfx_shader,
                shader_inputs = (self.postfx_shader_inputs + [ ("vx", [ self.player.vx ]), ("vy", [self.player.vy]) ]) ) 

        with render_target(self.hud_buffer):
            with blendstate(blendmode.alpha_over):
                context.clear()    
                render_text("{0}Pph".format(floor(self.player.speed)),1,1,[hca,hcb,hcc])
                render_text("L:{:>2}".format(floor(self.pickup.display_level)),(16*8),1,[hcc,hca,hcb])
                render_text("score:{0}".format(floor(self.player.points)),wobble,85+wobble,[hcb,hca,hcc])


        #with render_target(self.hud_buffer):
        #    with blendstate(blendmode.alpha_over):
        #        render_text("{0}".format(self.player.speed),0,0,[1.0,1.0,1.0])

        #with blendstate(blendmode.alpha_over):
        #    self.hud_buffer.render_processed( shaders.get_client_program("no_transform","postfx/passthru") )
        

        ## commenting this out because it seems broken?? the great rebuild, 2017....

        #with blendstate(blendmode.add):
        #    self.player_overlay.render_processed( shaders.get_client_program( "no_transform", "postfx/passthru" ),
        #                [ self.distortion_buffer ] )
        with blendstate(blendmode.alpha_over):
            self.hud_buffer.render_processed( shaders.get_client_program("no_transform","postfx/passthru") )

        if(self.t%4==0):
            with render_target(self.player_overlay):
                context.set_clear_color(0.0,0.0,0.0,0.0)
                context.clear()    
                with blendstate(blendmode.alpha_over):
                    #self.sprite_renderer.render(distortion_batch)
                    self.sprite_renderer.render(primary_batch)

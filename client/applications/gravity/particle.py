from client.gfx.sprite           import sprite

class particle:
    sprites = {}
    def tick_sprites():
        for sprite in particle.sprites:
            particle.sprites[sprite].tick()

    def flush_classes():
        particle.sprites = {}

    def create_particle_class( name, renderer, frames = [4,5,6] ):
        particle.sprites[name] = sprite(
                                        sprite_renderer = renderer,
                                        named_animations = {
                                                                "default" : frames
                                                                },
                                        current_animation = "default",
                                        ticks_per_frame = 12
                                     )

    def __init__(self, x, y, vx, vy, r, classname):
        self.x = x
        self.y = y
        self.vx = vx
        self.vy = vy
        self.r = r
        self.ttl = 60
        self.vdecay = 0.99
        try:
            self.sprite = particle.sprites[classname]
        except KeyError:
            raise ValueError("no particle class matching:{0}".format(classname))


    def tick(self, vortex):
        self.r += 0.01
        self.x += self.vx
        self.y += self.vy
        self.vx*=self.vdecay
        self.vy*=self.vdecay
        self.ttl -= 1
        [ self.x, self.y ] = vortex.transform( [ self.x, self.y] )

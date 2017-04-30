from client.beagle.assets import assets
from client.gfx.primitive import primitive
from client.gfx.text import render_text
from client.gfx.framebuffer import render_target
from client.gfx.context import gfx_context

## Ultimately all official API features need to have entry points from here, presently migrating
## on as-needed basis from old mechanisms which either necessitated tons of import statements, or
## using the hash in the asset manager

class beagle_api():
    """ High Level Aggregate API for Beagle """
    class primitive():
        """ Primitive API
        
            Attributes:
                unit_uv_square: a unit square (-1.0,1.0) primitive useful as input data for a shader
        """
        unit_uv_square = primitive.get_unit_uv_primitive()

    class context():
        """ Graphics Context API """
        def clear(r,g,b,a):
            """ clear context to specific color """
            return gfx_context.clear_rgba(r,g,b,a)

        def render_target(buffer):
            """ returns a context manger, render calls in context will apply to the passed in framebuffer """
            return render_target(buffer)


    class framebuffer():
        """ Framebufer API """
        def from_dims(w,h):
            """  Create a frame buffer with specified dimensions """
            return assets.exec("core/factory/framebuffer/[w,h]",[w,h])

    class blendmode():
        """ Blendmode API 

            Attributes:
                alpha_over: blendstate for standard alpha channel blending
                add: blendstate for additions
        """
        alpha_over = assets.get("core/hwgfx/blendmode/alpha_over")
        add = assets.get("core/hwgfx/blendmode/add")

    class lotext():
        """ Lotext API, low level text rendering. """
        def render_text_pixels( txt, x, y, color ):
            """ Render text at an arbitrary pixel position with color in form [r,g,b] """
            return render_text(txt,x,y, color)

        def render_text_grid( txt, x, y, color):
            """ Render text at an arbitrary (8x8) character grid position with color in form [r,g,b] """
            return render_text(txt,x*8,y*8, color)

api = beagle_api


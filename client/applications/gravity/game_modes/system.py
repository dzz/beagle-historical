from client.gfx.text import render_text
from client.gfx.blend            import blendstate,blendmode
class system_game:

        def __init__(self):
                pass

        def tick(self,context):
                context.trigger_mode_switch("vortex")

        def render(self,context):
            with blendstate(blendmode.alpha_over):
                render_text("HELLO WORLD",0,0,[1.0,1.0,1.0])


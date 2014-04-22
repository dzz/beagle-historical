import shaders
import hwgfx

BLENDMODE_OVER              = 0
BLENDMODE_BRUSH_COMPOSITE   = 1
BLENDMODE_DAB_RENDERING     = 2
BLENDMODE_BLACKHOLE         = 600

class label(object):
    def __init__(self, text, color, fontsize ):
        self._label = hwgfx.label_generate()
        self.color = color;
        hwgfx.label_set_text(self._label,text)

    def __del__(self):
        print "PY: delete label %x" % self._label
        hwgfx.label_drop(self._label)

    def set_text(text):
        hwgfx.label_set_text(self._label,text)

    def draw(self,x,y ):
        hwgfx.label_render(self._label,x,y,1,1,1);

def rect_solid(r,color):
    shader = shaders.get(  "hwgfx/rect", 
                           "hwgfx/rect_solid" )

    shader.bind         ( [( "color", color )] )
    hwgfx.rect_draw( r[0],r[1],r[2],r[3] )

def rect_vgrad(r,color0,color1):
    shader = shaders.get(  "hwgfx/rect", 
                           "hwgfx/rect_vgrad" )

    shader.bind         ( [( "color0", color0 )] )
    shader.bind         ( [( "color1", color1 )] )
    hwgfx.rect_draw( r[0],r[1],r[2],r[3] )



def blend_enter(mode):
    hwgfx.blend_enter(mode)

def blend_exit():
    hwgfx.blend_exit();


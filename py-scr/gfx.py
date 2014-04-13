# mock gfx api methods
def hwgfx_genlabel(text,color,fontsize):
    return [0,0,0]

def hwgfx_draw_texquad( x, y, w, h ):
    pass

def hwgfx_draw_solidquad(x, y, w, h, r, g, b):
    pass

def hwgfx_draw_solidtri(x1,y1,x2,y2,x3,y3,r,g,b):
    pass
#

class label(object):
    def __init__(self, text, color, fontsize ):
       [ self.w,
         self.h,
         self.tex_id] = hwgfx_genlabel(text, color, fontsize)

    def __del__(self):
        hwgfx_drop_texture(self.tex_id)

    def draw(self,r):
        hwgfx_draw_texquad( r[0], r[1], self.w, self.h )

        
def solid_rect(r,color):
    hwgfx_draw_solidquad( r[0],r[1],r[2],r[3],
               color[0],color[1],color[2] )

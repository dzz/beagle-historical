import hwgfx
import client.gfx.shaders as shaders
import client.gfx.texture as texture

def rect_solid(r,color):
    shader = shaders.get(  "hwgfx/rect", 
                           "hwgfx/rect_solid" )

    shader.bind         ( [( "color", color )] )
    hwgfx.rect_draw( r[0],r[1],r[2],r[3] )

def rect_vgrad(r,color0,color1):
    shader = shaders.get(  "hwgfx/rect", 
                           "hwgfx/rect_vgrad" )

    shader.bind         ([ ( "color0", color0 ),
                           ( "color1", color1 ) ])
    hwgfx.rect_draw( r[0],r[1],r[2],r[3] )

def rect_brush(brush, view):

    if(brush.texture is None):
        return

    shader = shaders.get(  "hwgfx/rect_brush", 
                           "hwgfx/rect_text" )

    shader.bind         ( [( "color0", brush.colour )] )
    shader.bind         ( [( "color1", [1.0,1.0,1.0,0.5] )] )


    brush.texture.bind( texture.units[0] )


    pix_x = brush.r[0] - int(view.cam[0])
    pix_y = brush.r[1] - int(view.cam[1])
    
    par_x = (pix_x * brush.parallax)
    par_y = (pix_y * brush.parallax)

    dif_x = pix_x - par_x
    dif_y = pix_y - par_y

    dif_x /= brush.texture.w
    dif_y /= brush.texture.h

    hwgfx.rect_draw_tex( 
                         int(par_x),
                         int(par_y),
                         brush.r[2],
                         brush.r[3],
                         brush.uvs[0],# + dif_x, 
                         brush.uvs[1],# + dif_y, 
                         brush.uvs[2], 
                         brush.uvs[3] )

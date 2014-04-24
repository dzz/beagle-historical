import hwgfx
import client.gfx.shaders as shaders

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

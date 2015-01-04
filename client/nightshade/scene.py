from enum import Enum

class brush_orientation(Enum):
    north   = 0
    east    = 1
    south   = 2
    west    = 3

class surface(Enum):
    floor = 0
    north = 1
    east = 2



class decorations:
    def __init__(self):
        self.surfaces = { 
                            surface.floor       : {  "type" : "rgba", "data" : ( 0.0, 0.0, 1.0 ) },
                            surface.north       : None,
                            surface.east        : None,
                         };



class scene_brush:
    def __init__(self,x = 0 ,y = 0,w = 1,h = 1,z = (0,0), wall_heights = ( 1,1,1,1), o = brush_orientation.north, decorations = decorations()):
        self.x              = x;
        self.y              = y;
        self.w              = w;
        self.h              = h;
        self.z              = z;
        self.wall_heights   = wall_heights
        self.orientation    = o
        self.decorations    = decorations
       

class scene:
    def __init__(self):
        self.brushes = [  scene_brush( x = 0, y = 0,    w = 20, h = 10,    z = (1,1), wall_heights( 1,1,0,1), o = brush_orientation.south ),
                          scene_brush( x = 9, y = 11,   w = 2,  h = 9,     z = (1,0), wall_heights( 0,0,0,0), o = brush_orientation.south ),
                          scene_brush( x = 0, y = 30,   w = 20, h = 10,    z = (0,0), wall_heights( 0,1,1,1), o = brush_orientation.south ) ]


import json
from client.gfx.tileset         import tileset
from client.gfx.rect            import rect_tile, rect_tile_start, rect_tile_raw, rect_tile_end
from client.gfx.primitive       import primitive
from client.gfx.primitive       import draw_mode
import client.gfx.texture

import client.ctt2.host_config  as host_config
import client.gfx.shaders       as shaders

class tilemap:
    def __init__(self, configuration, img_path, filtered = False ):
        self.tilesets = []
        self.gid_tileset_map = {}  
        self.layers = []
        self.tileheight = configuration["tileheight"]
        self.primitive = None
        self.primaryTileset = None
        self.shader = shaders.get_client_program( "quad_vertex", "quad_fragment" )

        for tileset_definition in configuration["tilesets"]:
            ts = tileset( tileset_definition, img_path) 
            for gid in range( ts.firstgid, ts.gidcount):
                self.gid_tileset_map[gid] = ts
            self.tilesets.append(ts)

        for layer_definition in configuration["layers"]:
            layer = {}
            layer["height"] = layer_definition["height"]
            layer["width"] = layer_definition["width"]
            layer["data"] = layer_definition["data"]
            self.layers.append(layer)

        if(len(self.tilesets)>0):
             self.primaryTileset = self.tilesets[0]
        else:
             raise ValueError("Input JSON for tilemap must have at least one tileset")

        self.compile()

    def compile(self):

        tile_coords = []
        tile_uvs    = []

        for layer in self.layers:
            rows = range(0,layer["height"])
            columns = range(0,layer["width"])
            layer_data = layer["data"]

            gid_idx = 0
            for y in rows:
                for x in columns:
                    gid_id = layer_data[gid_idx]
                    if(gid_id>0):
                        ts = self.gid_tileset_map[gid_id]
                        tile = ts.get_gid(gid_id)
                        if(tile):
                            tx = float(x)
                            ty = float(y)
                            tile_coords.extend(   [ [ tx,  ty   ], 
                                                    [ tx+1,ty   ], 
                                                    [ tx+1,ty+1 ], 

                                                    [ tx+1,ty+1 ], 
                                                    [ tx,  ty+1 ], 
                                                    [ tx,  ty   ] ] )

                            tile_uvs.extend( [ 
                                               [ tile[0],         tile[1]         ],
                                               [ tile[0]+tile[2], tile[1]         ],
                                               [ tile[0]+tile[2], tile[1]+tile[3] ],
                                               [ tile[0]+tile[2], tile[1]+tile[3] ],
                                               [ tile[0]        , tile[1]+tile[3] ],
                                               [ tile[0],         tile[1]         ]
                                               ] )
                        
                    gid_idx+=1


        self.primitive = primitive( draw_mode.TRIS, tile_coords, tile_uvs )
          
    def render(self,org_x,org_y,scale, debug=False ):
        
        self.primaryTileset.texture.bind(0)
        self.shader.bind([])
        self.primitive.render()
        return


        active_ts = None

    #    for layer in self.layers:
    #        rows = range(0,layer["height"])
    #        columns = range(0,layer["width"])
    #        layer_data = layer["data"]

    #        gid_idx = 0
    #        for y in rows:
    #            for x in columns:
    #                gid_id = layer_data[gid_idx]
    #                if(gid_id>0):
    #                    ts = self.gid_tileset_map[gid_id]
    #                    if(ts is not active_ts):
    #                        if active_ts is not None:
    #                            rect_tile_end(active_ts)
    #                        else:
    #                            rect_tile_start(ts)
    #                        active_ts = ts
    #                    if not debug:
    #                        rect_tile_raw( ts, gid_id, org_x+((x*self.tileheight))*scale, org_y+((y*self.tileheight))*scale, scale)
    #                gid_idx+=1

    def gid_via_coord(self,x,y,layer):
        i = x+(y*self.layers[layer]["width"])
        gid_id = self.layers[layer]["data"][i]
        return gid_id

    def tile_prop_via_coord(self,x,y,layer,key):
        i = x+(y*self.layers[layer]["width"])
        gid_id = self.layers[layer]["data"][i]
        ts = self.gid_tileset_map[gid_id]
        return ts.tile_prop(gid_id,key)

    @classmethod 
    def from_json_file(cls, path, img_path, filtered=False ):
        root = host_config.get_config("app_dir")
        json_parsed = {}
        with open("{0}{1}".format(root,path)) as f:
            json_data = f.read()
            json_parsed = json.loads(json_data)
        return cls(json_parsed, img_path, filtered)


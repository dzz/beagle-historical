import os
import hwgfx
import client.ctt2.host_config  as host_config
from client.gfx.texture import texture
from client.gfx.local_image import local_image

class tileset:
    def __init__(self, configuration, img_path = "" ):

        # tiled will write out tilemap defs with
        # a FQ path, or path relative to the TEX file,
        # so, remap the filename to a directory passed
        # in explicitly which itself is a subdir of the application
        # container directory

        tail,head = os.path.split( configuration["image"] )
        self.image = host_config.get_config("app_dir") + img_path + head
        self.imageheight = configuration["imageheight"]
        self.imagewidth = configuration["imagewidth"]
        self.margin = configuration["margin"]
        self.spacing = configuration["spacing"]
        self.properties = configuration["properties"]
        self.firstgid = configuration["firstgid"]
        self.tileheight = configuration["tileheight"]
        self.tileproperties = configuration["tileproperties"]
        self.gids = []
        self.gidproperties = {}
        self.gidcount=0
        self.texture = None
        self.compile()

    def compile(self):
        self.texture     = texture.from_local_image( local_image.from_file(self.image)  )

        uPix = self.margin
        vPix = self.margin
        tH = self.tileheight
        tS = self.spacing
        while(vPix<self.imageheight):
            while(uPix<self.imagewidth):
                self.gids.append( [uPix, vPix, uPix+tH, vPix+tH, {} ] )
                uPix+= tS
                uPix+= tH
            uPix = 0
            vPix += tH
            vPix += tS
        #normalize gids
        for gid in self.gids:
            gid[0]/=self.imagewidth
            gid[2]/=self.imagewidth
            gid[1]/=self.imageheight
            gid[3]/=self.imageheight

        self.gidcount = len(self.gids)

        print("loaded tileset")
        print("--------------")
        print("gidcount:{0}".format(self.gidcount) )

        for key in self.tileproperties:
            ikey = int(key)
            self.gidproperties[ikey] = self.tileproperties[key]

    def bind():
        self.texture.bind(0)

    def drawTile(gid):
        gid = gid - self.firstgid;
        if (gid<0) or (gid>self.gidcount):
            #this tileset doesn't contain the requested gid
            return

    def tileProps(gid):
        return self.gidproperties[gid]        

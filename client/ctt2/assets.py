import json
from client.gfx.texture import texture
import client.ctt2.host_config  as host_config
from client.gfx.local_image import local_image
from client.gfx.tileset import tileset
import os


def cvt_path(relpath):
        r = host_config.get_config("app_dir") + relpath
        return r

class resource_manager:
        def __init__(self, config):
            self.resource_map = {}
            self.adapters = { "texture"     : tex_adapter,
                              "tileset"     : tileset_adapter,
                              "audio_clip"  : audioclip_adapter}
            for pkg in config["packages"]:
                pkg_def = config["packages"][pkg]
                for resource_definition in pkg_def["resources"]:
                    self.load_resource(pkg, resource_definition)

        def load_resource(self, pkgname, resdef):
            if resdef["type"] in self.adapters:
                key = "{0}/{1}".format(pkgname, resdef["name"])
                self.resource_map[key] = self.adapters[resdef["type"]].load(resdef)

        def get_resource(self, path):
            return self.resource_map[path]

class tex_adapter:
    def load(tex_def):
        imagename = cvt_path(tex_def["filename"])
        return texture.from_local_image( local_image.from_file(imagename), tex_def["filtered"])

class tileset_adapter:
    def load(ts_def):
        return tileset( ts_def, "", ts_def["filtered"] ) 


class audioclip_adapter:
    def load(ac_def):
        pass

instance = None
class assets:
        def get(path):
            global instance
            return instance.get_resource(path)

class asset_manager:
        def get(path):
            global instance
            return instance.get_resource(path)
    
        def compile(json_file):
            path = cvt_path(json_file)
            with open(path, "r") as resources_file:
                    data = json.load(resources_file)
                    global instance
                    instance = resource_manager(data) 


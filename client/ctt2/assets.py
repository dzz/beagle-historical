import json
from client.gfx.texture import texture
import client.ctt2.host_config  as host_config
from client.gfx.local_image import local_image
from client.gfx.tileset import tileset
import client.gfx.shaders as shaders
from client.gfx.coordinates import centered_view,Y_Axis_Down, Y_Axis_Up
import os
import audio


def cvt_path(relpath):
        r = host_config.get_config("app_dir") + relpath
        return r

class resource_manager:
        def __init__(self, config):
            self.resource_map = {}
            self.adapters = { "texture"     : tex_adapter,
                              "tileset"     : tileset_adapter,
                              "audio_clip"  : audioclip_adapter,
                              "shader"      : shader_adapter,
                              "coordsys"    : coordsys_adapter,
                              "dict"        : dict_adapter }

            for pkg in config["packages"]:
                pkg_def = config["packages"][pkg]
                for resource_definition in pkg_def["resources"]:
                    self.load_resource(pkg, resource_definition)

        def load_resource(self, pkgname, resdef):
            if resdef["type"] in self.adapters:
                key = "{0}/{1}/{2}".format(pkgname, resdef["type"], resdef["name"])
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
        return audio.clip_create(host_config.get("app_dir") + ac_def["filename"])

class coordsys_adapter:
    def load(cs_def):
        if cs_def["mode"] == "centered_view":
            if cs_def["y_axis"] == "down":
                y_axis = Y_Axis_Down
            if cs_def["y_axis"] == "up":
                y_axis = Y_Axis_Up

            return centered_view(cs_def["width"],cs_def["height"], y_axis )

class dict_adapter:
    def load(dict_def):
            return dict_def["dict"]

class shader_adapter:
    def load(shd_def):
        return shaders.get_client_program( shd_def["vertex_program"], shd_def["pixel_program"] )

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


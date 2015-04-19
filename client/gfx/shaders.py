import client.ctt2.host_config  as host_config
import hwgfx

_shaders = {}

def get(vert,frag, path = None ):
    global _shaders
    if (vert,frag,path) in _shaders.keys():
        return _shaders[ (vert, frag, path) ]
    else:
        loaded                  = shader(vert,frag,path) 
        _shaders[( vert, frag, path )] = loaded
        return loaded

def get_client_program( vert, frag ):
    return get(vert,frag, host_config.get("app_dir") + "shaders/")


class shader(object):
    def __init__(self,vert,frag, path = None ):

        if( path is None):
            self._shader = hwgfx.shader_load(
                    "shaders/" + vert + ".vert.glsl",
                    "shaders/" + frag + ".frag.glsl")
        else:
            self._shader = hwgfx.shader_load(
                    path + vert + ".glsl",
                    path + frag + ".glsl")

        print("PY: loaded shader ",self._shader)

    def bind(self,uniforms):
        hwgfx.shader_bind(self._shader);


        for u in uniforms:
            name    = u[0]
            vector  = u[1]
            vlen    = len(vector)

            if vlen == 1:
                hwgfx.shader_bind_float (self._shader, name, 
                        vector[0])
            elif vlen ==2:
                hwgfx.shader_bind_vec2  (self._shader, name, 
                        vector[0],
                        vector[1])
            elif vlen ==3:
                hwgfx.shader_bind_vec3  (self._shader, name, 
                        vector[0],
                        vector[1],
                        vector[2])
            elif vlen ==4:
                hwgfx.shader_bind_vec4  (self._shader, name, 
                        vector[0],
                        vector[1],
                        vector[2],
                        vector[3])

    def __del__(self):
        print("PY:deleting shader", self._shader)
        hwgfx.shader_drop(self._shader)

    @classmethod
    def from_core_library( cls, vert, frag):
        return cls( vert, frag , "shaders/" )

    @classmethod
    def from_application_library( cls, vert, frag):
        return cls( vert, frag, host_config.get("app_dir") + "shaders/" )


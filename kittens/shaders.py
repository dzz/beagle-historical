import hwgfx

_shaders = {}

def get(vert,frag):
    if name in _shaders.keys():
        return _shaders[ [vert, frag] ]
    else:
        loaded                  = shader(vert,frag) 
        _shaders[[ vert, frag]] = loaded
        return loaded



class shader(object):
    def __init__(self,vert,frag):
        self._shader = hwgfx.shader_load(
                "shaders/" + vert + ".vert.glsl",
                "shaders/" + frag + ".frag.glsl");

    def bind(self,uniforms):
        hwgfx.shader_bind(self._shader)
        for u in uniforms
            name    = u[0]
            vector  = u[1]
            vlen    = len(vector)

            if vlen == 1:
                hwgfx.shader_bind_float(name, 
                        vector[0])
            elif vlen ==2:
                hwgfx.shader_bind_vec2(name, 
                        vector[0],
                        vector[1])
            elif vlen ==3:
                hwgfx.shader_bind_vec3(name, 
                        vector[0],
                        vector[1],
                        vector[2])
            elif vlen ==4:
                hwgfx.shader_vind_vec4(name, 
                        vector[0],
                        vector[1],
                        vector[2],
                        vector[3])

    def __del__(self):
        hwgfx.shader_drop(self._shader)


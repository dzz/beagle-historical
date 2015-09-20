from client.ctt2.assets import assets
from client.gfx.text import render_text

class eaos_status:
    def __init__(self):
        self.finalized = False
        self.next_application = None
        self.scale = 0.0
        self.scale_delta = 0.01
        self.crsr = [0.0,0.0]

    def render(self):
        with assets.get("core/blendmode/add"):
            assets.get("core/primitive/unit_uv_square").render_shaded(
                    assets.get("common/shader/default_2d"),{
                        "texBuffer" : assets.get("sylab/texture/termlay"),
                         "translation_local"    : [0.0,0],
                         "scale_local"          : [1,self.scale],
                         "translation_world"    : [0.0,0.0],
                         "scale_world"          : [1.0,1.0],
                         "view"                 : assets.get("common/coordsys/unit_square"),
                         "rotation_local"       : 0.0 ,
                         "filter_color"         : [1.0,1.0,1.0,1.0],
                         "uv_translate"         : [0,0] })

        with assets.get("core/blendmode/alpha_over"):
            assets.get("core/primitive/unit_uv_square").render_shaded(
                    assets.get("common/shader/default_2d"),{
                        "texBuffer" : assets.get("sylab/texture/crsr"),
                         "translation_local"    : [0,0],
                         "scale_local"          : [0.075,0.1],
                         "translation_world"    : [self.crsr[0],self.crsr[1]],
                         "scale_world"          : [1.0,1.0],
                         "view"                 : assets.get("common/coordsys/unit_square"),
                         "rotation_local"       : 0.0 ,
                         "filter_color"         : [1.0,1.0,1.0,1.0],
                         "uv_translate"         : [0,0] })



        render_text(" modrept",0,1*8,[1,1,1])
        render_text(" ----------",0,2*8,[1,1,1])
        render_text("  00. [A   ]",0,3*8,[1,1,1])
        render_text("  01. [A   ]",0,4*8,[1,1,1])
        render_text("  10. [   F]",0,5*8,[1,0,0])
        render_text("  11. [   F]",0,6*8,[1,0,0])



    def handle_input(self):
        gp = assets.exec("core/queries/gamepad/find_primary")

        if(abs(gp.rightStick[0])>0.2):
            self.crsr[0]+=gp.rightStick[0]*0.04
        if(abs(gp.rightStick[1])>0.2):
            self.crsr[1]+=gp.rightStick[1]*0.04


        max_crsr = 0.9
        min_crsr = -0.9
        if self.crsr[0]<min_crsr:
            self.crsr[0] = min_crsr
        if self.crsr[0]> max_crsr:
            self.crsr[0] = max_crsr
        if self.crsr[1]<min_crsr:
            self.crsr[1] = min_crsr
        if self.crsr[1]> max_crsr:
            self.crsr[1] = max_crsr



    def tick(self):
        if self.scale < 1.0:
            self.scale += self.scale_delta
            self.scale_delta *= 1.25
        else:
            self.scale = 1.0

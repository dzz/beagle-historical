from client.ctt2.assets import assets
from client.gfx.text import render_text

class eaos_status:
    def switch_to_binmod(self):
        self.render_proc = self.render_binmod_report

    def __init__(self):
        self.finalized = False
        self.next_application = None
        self.scale = 0.0
        self.scale_delta = 0.01
        self.crsr = [0.0,0.0]

        self.proclist =  [
                { "type" : "decoration", "name" : "proclist", "selected" : False },
                { "type" : "decoration", "name" : "--------", "selected" : False },
                { "type" : "decoration", "name" : "", "selected" : False },
                { "type" : "runnable", "name" : "binmod_cfg", "selected" : False, "action": self.switch_to_binmod }
                     ]

        self.render_proc = self.render_command_select

    def render_cursor(self):
        with assets.get("core/blendmode/alpha_over"):
            assets.get("core/primitive/unit_uv_square").render_shaded(
                    assets.get("common/shader/default_2d"),{
                        "texBuffer" : assets.get("sylab/texture/crsr"),
                         "translation_local"    : [0,0],
                         "scale_local"          : [0.075*0.75,0.1*0.75],
                         "translation_world"    : [self.crsr[0],self.crsr[1]],
                         "scale_world"          : [1.0,1.0],
                         "view"                 : assets.get("common/coordsys/unit_square"),
                         "rotation_local"       : 0.0 ,
                         "filter_color"         : [1.0,1.0,1.0,1.0],
                         "uv_translate"         : [0,0] })



    def render_command_select(self):
        commands = self.proclist
        with assets.get("core/blendmode/add"):
            assets.get("core/primitive/unit_uv_square").render_shaded(
                    assets.get("common/shader/default_2d"),{
                        "texBuffer" : assets.get("sylab/texture/infra_overview"),
                         "translation_local"    : [0.0,0],
                         "scale_local"          : [1,self.scale],
                         "translation_world"    : [0.0,0.0],
                         "scale_world"          : [1.0,1.0],
                         "view"                 : assets.get("common/coordsys/unit_square"),
                         "rotation_local"       : 0.0 ,
                         "filter_color"         : [1.2,1.8,0.8,0.9],
                         "uv_translate"         : [0,0] })

        row = 0
        for command in commands:

            color = [1,1,0]

            if command["type"] is not "decoration":
                strg = "{0:x}:{1}".format(row*65535,command["name"])
            else:
                strg = command["name"]

            idx = int((self.crsr[1]*18))+11

            if(self.crsr[0]>0):
                if(idx==row) and command["type"] is not "decoration":
                    strg = "[{0}]".format(strg.upper())
                    color = [1,1,1]
                    command["selected"] = True
            else:
                command["selected"] = False

            render_text( strg, 230,(8*4)+(row*8), color )
            row+=1

    def render_binmod_report(self):
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



        render_text(" modrept",0,1*8,[1,1,1])
        render_text(" ----------",0,2*8,[1,1,1])
        render_text("  00. [A   ]",0,3*8,[1,1,1])
        render_text("  01. [A   ]",0,4*8,[1,1,1])
        render_text("  10. [   F]",0,5*8,[1,0,0])
        render_text("  11. [   F]",0,6*8,[1,0,0])

    def render(self):
        self.render_proc()
        self.render_cursor()

    def handle_click(self):
        if(self.render_proc == self.render_command_select):
            for command in self.proclist:
                if command["type"] is not "decoration" and command["selected"]:
                    command["action"]()

    def handle_input(self):
        gp = assets.exec("core/queries/gamepad/find_primary")


        if(self.render_proc!=self.render_command_select):
            if(gp.button_down( assets.get("core/gamepad/buttons").B ) ):
                self.render_proc = self.render_command_select

        if( gp.triggers[1] > 0.5):
            self.handle_click()

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

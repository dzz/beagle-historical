from client.ctt2.assets import assets
from math import sin

class eaos_binmods():
    def __init__(self):
        self.powersave_mode = True

class eaos_status:
    def switch_to_binmod(self):
        self.render_proc = self.render_binmod_report

    def switch_to_ident(self):
        self.render_proc = self.render_ident

    def init_scale(self):
        self.scale = 0
        self.scale_delta = 0.01

    def __init__(self,terminal = None, parent_app = None):
        self.parent_app = parent_app
        self.terminal = terminal
        self.finalized = False
        self.next_application = None
        self.scale = 0.0
        self.scale_delta = 0.01
        self.crsr = [0.0,0.0]
        self.scr_count = 0.0
        self.uses_cursor = True
        self.timeout = 0.0

        self.binmods = eaos_binmods()

        self.proclist =  [
                { "type" : "decoration", "name" : "*PROCS*", "selected" : False },
                { "type" : "decoration", "name" : "*******", "selected" : False },
                { "type" : "decoration", "name" : "", "selected" : False },
                { "type" : "runnable", "name" : "binmod_cfg", "selected" : False, "action": self.switch_to_binmod },
                { "type" : "runnable", "name" : "identtool", "selected" : False, "action": self.switch_to_ident }
                     ]

        self.render_proc = self.render_command_select

    def render_cursor(self):
        with assets.get("core/hwgfx/blendmode/alpha_over"):
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
        with assets.get("core/hwgfx/blendmode/add"):
            assets.get("core/hwgfx/primitive/unit_uv_square").render_shaded(
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

            color = [0.2,1,0]

            if command["type"] is not "decoration":
                strg = command["name"]
                color = [1,1,1]
            else:
                strg = command["name"]

            idx = int((self.crsr[1]*18.1))+11

            if(self.crsr[0]>0):
                if(idx==row) and command["type"] is not "decoration":
                    strg = "[{0}]".format(strg.upper())
                    color = [0,1,1]
                    command["selected"] = True
                else:
                    command["selected"] = False
            else:
                command["selected"] = False

            with(assets.get("core/hwgfx/blendmode/alpha_over")):
                assets.exec("core/lotext/print(pixels)[txt,[x,y],[r,g,b]]", [ strg, [230,(8*4)+(row*8)], color ] )
            row+=1

    def render_ident(self):
            assets.get("core/primitive/unit_uv_square").render_shaded(
                    assets.get("common/shader/default_2d"),{
                        "texBuffer" : assets.get("sylab/texture/ident"),
                         "translation_local"    : [0.0,0],
                         "scale_local"          : [1,self.scale],
                         "translation_world"    : [0.0,0.0],
                         "scale_world"          : [1.0,1.0],
                         "view"                 : assets.get("common/coordsys/unit_square"),
                         "rotation_local"       : 0.0 ,
                         "filter_color"         : [1.0,1.0,1.0,1.0],
                         "uv_translate"         : [0,0] })
            with(assets.get("core/hwgfx/blendmode/alpha_over")):
                assets.exec("core/lotext/print(rows)[txt,[x,y],[r,g,b]]",["QRS 9.10.4.2",[28,18],[1,1,1] ] )
                assets.exec("core/lotext/print(rows)[txt,[x,y],[r,g,b]]",
                        [ assets.exec("core/stringfx/scroll[txt,offset]",["MAKING IDENT..........",int(self.scr_count)]), [28,20], [1,1,0] ])

    def render_binmod_report(self):
        with assets.get("core/hwgfx/blendmode/add"):
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


        with assets.get("core/hwgfx/blendmode/alpha_over"):
            assets.exec_range("core/lotext/print(rows)[txt,[x,y],[r,g,b]]",
                    [
                        [ assets.exec("core/stringfx/scroll[txt,offset]", ["binmods ", int(self.scr_count) ]), 
                                        [0,1], [1,1,1] ],
                        [" ----------" ,[0,2], [1,1,1] ],
                        ["  00. [A   ]",[0,3], [1,1,1] ],
                        ["  01. [A   ]",[0,4], [1,1,1] ],
                        ["  10. [   F]",[0,5], [1,0,0] ],
                        ["  11. [   F]",[0,6], [1,0,0] ] ])

            if(self.binmods.powersave_mode):
                assets.exec_range("core/lotext/print(rows)[txt,[x,y],[r,g,b]]", 
                        [
                            [ "LO-VOLT MODE", [16,11],[1,1,1] ],
                            [ "requires:charge", [21,3],[1,1,1] ],
                            [ "requires:mod_start proc", [21,4],[1,1,1] ]
                        ]) 

    def render(self):
        self.render_proc()
        self.render_cursor()

    def handle_click(self):
        if(self.render_proc == self.render_command_select):
            for command in self.proclist:
                if command["type"] is not "decoration" and command["selected"]:
                    self.init_scale()
                    command["action"]()

    def handle_input(self):
        gp = assets.exec("core/queries/gamepad/find_primary")


        if not (gp.button_down( assets.get("core/gamepad/buttons").B ) ):
            self.timeout = -1

        if(self.timeout<0):
            if(self.render_proc!=self.render_command_select):
                if(gp.button_down( assets.get("core/gamepad/buttons").B ) ):
                    self.init_scale()
                    self.render_proc = self.render_command_select
                    #self.terminal.signal_was_learned()
                    self.timeout = 60.0

            elif(gp.button_down( assets.get("core/gamepad/buttons").B ) ):
                self.timeout = 60.0
                self.parent_app.finalized = False
                self.next_application = self.parent_app
                self.parent_app.help_ticker = 0.0
                self.finalized = True



        if( gp.triggers[1] > 0.5):
            self.handle_click()

        if(abs(gp.rightStick[0])>0.2):
            self.crsr[0]+=gp.rightStick[0]*0.04
        if(abs(gp.rightStick[1])>0.2):
            self.crsr[1]+=gp.rightStick[1]*0.04


        self.scr_count += 0.1

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
        if(self.timeout>-1):
            self.timeout -= 1

        if self.scale < 1.0:
            self.scale += self.scale_delta
            self.scale_delta *= 1.25
        else:
            self.scale = 1.0

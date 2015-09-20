from random import uniform
from math import floor
from client.gfx.text import render_text
from client.ctt2.assets import assets
from .status import eaos_status

class eaos_saver:
    def __init__(self, registers = []):
        self.t = 925234
        self.registers = registers
        self.reg_sum = 0.0
        self.solved = False
        self.finalized = assets.get("sylab/dict/debug_vars")["eaos_saver_finalized"]
        self.next_application = eaos_status()
        self.uses_cursor = False
        return

    def render(self):
        def scroll_text(strg,idx):
            idx = idx % len(strg)
            r = ""
            for i in range(0,len(strg)):
                r = r + strg[(i+idx)%len(strg)]
            return r
        render_text("{0:x}".format(self.t),8,8,[1.0,1.0,1.0])
        render_text("{0:d}".format(self.t//24),8*42,26*8,[1.0,1.0,1.0])

        if( uniform(0.0,1.0) > 0.99):
            render_text("0:xxx|||_____]".format(self.t//24),8*33,29*8,[1.0,1.0,1.0])
            render_text("1:xx|||||____]".format(self.t//24),8*33,(30*8)+2,[1.0,1.0,1.0])
        else:
            render_text("0:xx|||______]".format(self.t//24),8*33,29*8,[1.0,1.0,1.0])
            render_text("1:xxx||||____]".format(self.t//24),8*33,(30*8)+2,[1.0,1.0,1.0])

        row_left = 0
        row_right = 0
        for register in self.registers:

            if(register.pos[0]>0):
                x =  40 * 8
                row_right = row_right+1
                row = row_right
            else:
                row_left = row_left + 1
                row = row_left
                x = 1 * 8

            render_text(" |....| ", x, (row+12)*8, [ 1.0,
                                                         register.viz_idktr,
                                                         register.viz_idktr ] )
            render_text("[ {0:x} ]".format(int(register.viz_idktr*32)), x, (row+12)*8 )
            row+=1

        render_text("SV:9.0.8.9${0:.2f}]".format(self.reg_sum),17*8,13*8,[1.0,0.8,0.0])


            
        if(self.solved):
            render_text("[            ]",17*8,18*8,[0.0,1.0,1.0])
            render_text("   ******* ",17*8,17*8,[1.0,1.0,0.0])
            render_text("   ******* ",17*8,19*8,[1.0,1.0,0.0])
            render_text("   *     * ",17*8,18*8,[1.0,1.0,0.0])
            render_text(scroll_text(" ~ push `A` ~  ",self.t//10),17*8,22*8,[1.0,1.0,1.0])
            if(self.t%20 > 10 ):
                render_text("    READY  ",17*8,18*8,[0.0,1.0,0.0])




    def handle_input(self):
        buttons = assets.get("core/gamepad/buttons")
        if(self.solved):
            gp = assets.exec("core/queries/gamepad/find_primary")
            if(gp.button_down(buttons.A)):
                    self.finalized = True


    def tick(self):
        self.t += 1
        if not self.solved:
            self.reg_sum = 0
            for register in self.registers:
                self.reg_sum += (1.0-register.charge) / len(self.registers)
            if(self.reg_sum > 0.9 ):
                self.solved = True
                self.uses_cursor = True


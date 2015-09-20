from random import uniform
from client.gfx.text import render_text

class eaos_saver:
    def __init__(self, registers = []):
        self.t = 925234
        self.registers = registers
        self.reg_sum = 0.0
        return

    def render(self):
        render_text("{0:x}".format(self.t),8,8,[1.0,1.0,1.0])
        render_text("{0:d}".format(self.t//24),8*42,26*8,[1.0,1.0,1.0])

        if( uniform(0.0,1.0) > 0.99):
            render_text("0:xxx|||_____]".format(self.t//24),8*33,29*8,[1.0,1.0,1.0])
            render_text("1:xx|||||____]".format(self.t//24),8*33,(30*8)+2,[1.0,1.0,1.0])
        else:
            render_text("0:xx|||______]".format(self.t//24),8*33,29*8,[1.0,1.0,1.0])
            render_text("1:xxx||||____]".format(self.t//24),8*33,(30*8)+2,[1.0,1.0,1.0])

        row = 0
        for register in self.registers:
            render_text(" |....| ", (6*8), (row+12)*8, [ 0.0,
                                                         register.viz_idktr,
                                                         register.viz_idktr ] )
            render_text("[ {0:x} ]".format( int((52900+258*row)-register.viz_idktr*24*row) ), (6*8), (row+12)*8 )
            row+=((row%3)+1)

        render_text("SV:9.0.8.9${0:.2f}]".format(self.reg_sum),17*8,13*8,[1.0,0.8,0.0])

        if(self.reg_sum>0.96):
            render_text("[            ]",17*8,18*8,[0.0,1.0,1.0])
            if(self.t%10 > 5 ):
                render_text("  INVOKE-001  ",17*8,18*8,[1.0,1.0,0.0])




    def tick(self):
        self.t += 1
        self.reg_sum = 0
        for register in self.registers:
            self.reg_sum += (1.0-register.charge) / len(self.registers)

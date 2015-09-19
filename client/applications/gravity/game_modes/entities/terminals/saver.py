from client.gfx.text import render_text

class eaos_saver:
    def __init__(self):
        self.t = 925234
        return

    def render(self):
        render_text("{0:x}".format(self.t),8,8,[1.0,1.0,1.0])
        render_text("{0:d}".format(self.t//24),8*42,26*8,[1.0,1.0,1.0])
        render_text("0:||||||------".format(self.t//24),8*33,29*8,[1.0,1.0,1.0])
        render_text("1:||||||||----".format(self.t//24),8*33,(30*8)+2,[1.0,1.0,1.0])


        render_text("NA1TA 9.0.8.9",17*8,13*8,[1.0,1.0,1.0])

    def tick(self):
        self.t += 1

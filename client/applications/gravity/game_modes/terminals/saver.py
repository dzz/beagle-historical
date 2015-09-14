from client.gfx.text import render_text

class eaos_saver:
    def __init__(self):
        self.t = 925234
        return

    def render(self):
        render_text("ELEANOR OS 2.82",8,8,[1.0,1.0,1.0])
        render_text("---------------",8,16,[1.0,1.0,1.0])
        render_text("(sleeping)",8,24,[1.0,1.0,1.0])
        render_text("{0:2x}::{1:04x}".format(32500+int(0.2*self.t),int((4096+67*(self.t)))),64,64,[1.0,1.0,1.0])

    def tick(self):
        self.t += 1

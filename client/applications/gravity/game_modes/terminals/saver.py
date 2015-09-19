from client.gfx.text import render_text

class eaos_saver:
    def __init__(self):
        self.t = 925234
        return

    def render(self):
        render_text("LOCKED",8,8,[1.0,1.0,1.0])

    def tick(self):
        self.t += 1

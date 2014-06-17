import client.hockeyquest.interface as interface

class game:
    def __init__(self, scr_buf):
        self.scr_buf = scr_buf

    def init(self):
        interface.draw_interface(self.scr_buf, self)

    def render(self):
        interface.draw_interface(self.scr_buf, self)
        self.scr_buf.render()

    def tick(self):
        pass

    def finalize(self):
        pass



def get_game(scr_buf):
    return game(scr_buf)

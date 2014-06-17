import datetime
import client.hockeyquest.interface as interface
from client.system.timer import timer


class game:
    def __init__(self, scr_buf):
        self.timers = []
        self.scr_buf = scr_buf
        self.date = datetime.date( 1983, 9, 20 )
        self.hour = 9
        self.minute = 27

    def init(self):
        interface.draw_interface(self.scr_buf, self)
        self.timers = [ [ timer( 1.5 ), self.add_minute ] ]
        self.start_timers()

    def start_timers(self):
        for timer in self.timers:
            timer[0].start()

    def render(self):
        self.scr_buf.clear()
        interface.draw_interface(self.scr_buf, self)
        self.scr_buf.render()
         

    def tick(self):
        for timer_pair in self.timers:
            if timer_pair[0].tick():
                timer_pair[1]()

    def add_minute(self):
        self.minute = (self.minute+1)%60
        if(self.minute == 0):
            self.hour+=1
            if(self.hour == 24):
                self.date+= datetime.timedelta(days=1)
                self.hour = 0

    def finalize(self):
        pass



def get_game(scr_buf):
    return game(scr_buf)

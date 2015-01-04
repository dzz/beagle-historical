import datetime
import client.hockeyquest.interface as interface
from client.system.timer import timer
from client.hockeyquest.player import player


class game:
    def __init__(self, scr_buf):
        self.timers = []
        self.scr_buf = scr_buf
        self.date = datetime.date( 2003, 9, 20 )
        self.hour = 9
        self.minute = 0
        self._minute = 0

    def init(self):
        interface.draw_interface(self.scr_buf, self)
        self.timers = [ [ timer( 0.02 ), self.add_minute ], [ timer( 0.01 ), self.animate_minute ] ]
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
        player.tick(30)
        self._minute = (self._minute+30)%60
        if(self._minute == 0):
            self.hour+=1
            if(self.hour == 24):
                self.date+= datetime.timedelta(days=1)
                self.hour = 0
                player.signalDayChange( self.date.strftime("%A") )
        player._RG_HOUR = self.hour

    def animate_minute(self):
        if(self.minute< self._minute):
            self.minute+=1

        if(self.minute> self._minute):
            self.minute = (self.minute+1)%60


    def finalize(self):
        pass



def get_game(scr_buf):
    return game(scr_buf)

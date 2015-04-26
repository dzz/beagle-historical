from client.system.gamepad       import get_gamepad

class player:
    def __init__(self):
        self.x = 16.0
        self.y = 16.0
        self.player_speed = 0.2

    def update(self):
        pad = get_gamepad(0)
        self.x += pad.leftStick[0]*self.player_speed
        self.y += pad.leftStick[1]*self.player_speed

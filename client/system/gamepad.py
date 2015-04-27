import host

GamepadDeadzone = 0.1
GamepadFilter   = 0.5

gf_a = 1.0-GamepadFilter
gf_b = GamepadFilter


class gamepad:
    def __init__(self):
        self.leftStick = [0.0,0.0]
        self.rightStick = [0.0,0.0]
        #aliases
        self.left_stick = self.leftStick
        self.right_stick = self.rightStick
        self.triggers = [0.0,0.0]

gamepads = [];

def get_gamepad(i):
    return gamepads[i]

def init():
    max_pads = 8
    for i in range(0, max_pads ):
        gamepads.append( gamepad() );

def tick():
    if host.dequeue_gamepad_dirty() == 1:
        for i in range(0,host.get_gamepad_count() ):
            gp          = gamepads[i]

            axis_data   = host.get_gamepad_sticks(i)

            filtered_axis_data = [0.0,0.0,0.0,0.0,0.0,0.0]
            for ax in range(0,6):
                filtered_axis_data[ax] = axis_data[ax]
                if abs(axis_data[ax])<GamepadDeadzone:
                        filtered_axis_data[ax] = 0

            gp.leftStick[0]  = gp.leftStick[0]*gf_a + filtered_axis_data[0]*gf_b
            gp.leftStick[1]  = gp.leftStick[1]*gf_a + filtered_axis_data[1]*gf_b
            gp.rightStick[0]  = gp.rightStick[0]*gf_a + filtered_axis_data[2]*gf_b
            gp.rightStick[1]  = gp.rightStick[1]*gf_a + filtered_axis_data[3]*gf_b
            gp.triggers[0] = filtered_axis_data[4]
            gp.triggers[1] = filtered_axis_data[5]



import sys
import host
import hwgfx
import configparser
import client.system.log as log
import client.system.gamepad as gamepad
import client.system.keyboard as keyboard

from client.beagle.mouse_focus import mouse_focused_area
from client.beagle.status import render_status
from client.beagle.status import set_status

import client.beagle.beagle_environment  as beagle_environment
import client.apps
import client.ui.areas as ui_area
import client.gfx.blend as blend
import client.beagle.caret as caret
from   client.beagle.assets import asset_manager
import gc
import os

from client.system.telnet_console import *


__clickpos  = [0,0]
__mpos      = [0,0]

console = None

global app 

def init():
    def bool(v):
        if v is False: 
            return False
        return v.lower() in ("true","1")

    global app, console

    class output_redirect():
        def cvted(txt):
            lines = txt.split("\n")
            cvt_lines = []
            for line in lines:
                cvt_lines.append(line.rstrip())
            return cvt_lines

        def flush(self):
            return

        def _wr(txt, mode):
            for line in output_redirect.cvted(txt):
                log.write(mode, line )

        def write(self,txt):
            output_redirect._wr(txt,log.INFO)

    class error_redirect():
        def flush(self):
            return

        def write(self,txt):
            output_redirect._wr(txt,log.ERROR)
    
    sys.stdout = output_redirect()
    sys.stderr = error_redirect()

    config = configparser.ConfigParser()

    if(os.path.isfile("client/generated_application.ini")):
        config.read("client/generated_application.ini")
    else:
        config.read("client/application.ini")

    app_name = config["APPLICATION"]["name"]
    host.set_title(app_name)
    try:
        app_dir = config["APPLICATION"]["path"]
        log.write(log.INFO, "Loading application @:{0}".format(app_dir))
        app_module = config["APPLICATION"]["module"]
    except:
        app_dir = None
        app_module = None
    try:
        resource_json = config["APPLICATION"]["assets"]
    except:
        resource_json = None


    controller_enabled = bool( config["APPLICATION"]["controller_enabled"] );
    telnet_enabled = bool( config["APPLICATION"]["telnet_enabled"] );
    if telnet_enabled:
        telnet_port = int( config["APPLICATION"]["telnet_port"] )
        telnet_host = config["APPLICATION"]["telnet_host"]
    
    
    loaded_external = False
    if (app_dir is not None) and (app_module is not None):
        app = client.apps.get_app_from_path( app_dir, app_module )
        loaded_external = True
    else:
        app = client.apps.get_app(app_name) 

    app.controller_enabled = controller_enabled
    beagle_environment.set_config("app_name", app_name)
    if not loaded_external:
        beagle_environment.set_config("app_dir", "client/applications/" + app_name +"/")
    else:
        beagle_environment.set_config("app_dir", app_dir + "/app/")
    try:
        if config[app_name] is not None:
            app.configure( config[app_name] );
    except KeyError:
        pass
        log.write(log.INFO, "No configuration found, ignoring.")


    
    if resource_json:
            asset_manager.compile(resource_json)
    if(telnet_enabled):
        console = telnet_console(app, telnet_host, telnet_port)
    app.init()
    set_status("initialized application:" + app_name)
    if(app.controller_enabled):
        log.write(log.INFO, "{0} requesting controller input".format(app_dir))
        gamepad.init()

def finalize():
    global app
    app.finalize()

def configure( configuration ):
    pass

def tick():
    global app
    global console

    if console is not None:
        console.tick()

    if(app.controller_enabled):
        gamepad.tick()
    app.tick()
    gc.collect()

def render():
    app.render()
    
def _get_mf_area():
    return mouse_focused_area

def calculate_mouse_position(area,x,y):
    xt = min(max( 0, x - area.r[0] ),area.r[2])
    yt = min(max( 0, y - area.r[1] ),area.r[3])
    return [xt,yt] 

SIGNAL_HANDLED      = True
SIGNAL_DISCARDED    = False

def set_screensize(w,h):
    beagle_environment.set_config("screen_dims", [ w, h ] )

def dispatch_mouseup(button,x,y):
    global mouse_focused_area
    if mouse_focused_area is not None:
        m_pos = calculate_mouse_position(mouse_focused_area,x,y);
        mouse_focused_area.rcv_mouse_button(button,m_pos[0],m_pos[1], down = False)
        mouse_focused_area = None
        return SIGNAL_HANDLED
    return SIGNAL_DISCARDED

def dispatch_mousedown(button,x,y):
    global mouse_focused_area
    __clickpos[0] = x
    __clickpos[1] = y

    area = ui_area.find_ui_area(x,y)
    if area is not None:
        m_pos = calculate_mouse_position(area,x,y);
        area.rcv_mouse_button(button,m_pos[0],m_pos[1], down = True)
        mouse_focused_area = area
        return SIGNAL_HANDLED
    return SIGNAL_DISCARDED

def dispatch_mousemotion(x,y):
    global __mpos
    global mouse_focused_area
    __mpos = [x,y]
    ui_area.set_absolute_mpos([x,y])
    if mouse_focused_area is None:
        area = ui_area.find_ui_area(x,y)
        if area is not None:
            area.rcv_mousemotion(*(calculate_mouse_position(area,x,y)))
            return SIGNAL_HANDLED
    else:
        mouse_focused_area.rcv_mousemotion(*(calculate_mouse_position(mouse_focused_area,x,y)))
        return SIGNAL_HANDLED
    return SIGNAL_DISCARDED

def dispatch_key(key,down):
    keyboard.update_key(key,down)
    KEY_ESCAPE = 33
    caret_target = caret.get_caret()
    if down:
        if key == KEY_ESCAPE:
            caret.drop()
            return

    if caret_target is not None:
        caret_target.rcv_key(key,down)


def dispatch_root_keybindings(key,down):
    pass

def dispatch_text(text):
    caret_target = caret.get_caret()
    if caret_target is not None:
        caret_target.rcv_text(text)
    return

def map_keycode(code_definition):
    parsed = code_definition.split(":")
    keyboard.map_keycode_to_name(parsed[0],int(parsed[1]))
    return

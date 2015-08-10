import host

ERROR = 1
WARNING = 2
INFO = 4
DEBUG = 8
GFX_MSG = 16

def set_level(level):
    host.log_set_level(level)

def write(level,txt):
    host.log_client_message(level,txt)

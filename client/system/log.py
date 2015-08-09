import host

ERROR = 1
WARNING = 2
INFO = 4
DEBUG = 8

def write(level,txt):
    host.log_client_message(level,txt)


settings = {
        "render_status" : True
}


def set_config(key,value):
    global settings
    settings[key]=value

def get_config(key):
    global settings
    return settings[key]

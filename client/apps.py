import client.app.main
import client.hockeyquest.main
import client.nightshade.main

def get_app(app_name):
    apps = { 
            "ctt2"              : client.app.main,
            "hockeyquest"       : client.hockeyquest.main,
            "nightshade"        : client.nightshade.main,
            }

    return apps[app_name]


import client.applications.ctt2_environment.main
import client.applications.hockeyquest.main
import client.applications.nightshade.main

def get_app(app_name):
    apps = { 
            "ctt2"              : client.applications.ctt2_environment,
            "hockeyquest"       : client.applications.hockeyquest.main,
            "nightshade"        : client.applications.nightshade.main,
            }

    return apps[app_name]


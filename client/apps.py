import client.app.main
import client.hockeyquest.main

def get_app(app_name):
    apps = { 
            "ctt2"        : client.app.main,
            "hockeyquest" : client.hockeyquest.main }

    return apps[app_name]


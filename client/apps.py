import os
import importlib

def get_app(app_name):
    application_list = [f for f in os.listdir("client/applications/") if not os.path.isfile(f)]
    if(app_name in application_list):
        return importlib.import_module("client.applications." + app_name + ".main")

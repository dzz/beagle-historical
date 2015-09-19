from client.ctt2.assets import assets

class ow_enviro:
    def __init__(self,config ):
        self.x_range = [config["room_range"]["left"],config["room_range"]["right"]]
        self.children = []

    def register_child(self,child):
        self.children.append(child)

    def handle_wrap_around(self,child):
        if child.x < self.x_range[0]:
            child.x = self.x_range[1]
        if child.x > self.x_range[1]:
            child.x = self.x_range[0]

    def tick(self):
        for child in self.children:
            self.handle_wrap_around(child)

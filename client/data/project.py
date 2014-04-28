from client.data.image import image

class project:
    def __init__(self, 
            canvas_width, canvas_height):
        self.tracks         = []
        self.canvas_dims    = [ canvas_width, canvas_height ]
        self.brushes        = []

        self.init_default_brushlib()

    def add_track(self, track):
        self.tracks.append(track)

    def rem_track(self, track):
        self.tracks.remove(self, track)

    def get_empty_doc_image(self):
        img = image.from_dims(  self.canvas_dims[0], 
                                self.canvas_dims[1] )

    def init_default_brushlib(self):
        pass

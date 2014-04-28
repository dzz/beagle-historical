from localgfx import img_create
from localgfx import img_load
from localgfx import img_drop
from localgfx import img_dims

class image:
    def __init__(self, img, width, height):
        self._image     = img
        self.width      = width;
        self.height     = height;

    def __del__
        img_drop(self._img)

    @classmethod
    def from_dims(cls, width,height):
       return cls( img_create(width,height),width,height)

    @classmethod
    def from_name(cls, fname):
        img     = img_load(fname)
        (w,h)   = img_dims(img)
        return cls( img, w,h )


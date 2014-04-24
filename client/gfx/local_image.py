import localgfx

class local_image:
    def __init__(self,img):
        self._img           = img
        [ self.w, self.h ]  = localgfx.img_dims(self._img)
        print("PY: acquired local image:", self._img," with dims:", 
                [self.w, self.h])
        
    def __del__(self):
        localgfx.img_drop( self._img )
        print("PY: dopped local image", self._img)

    @classmethod
    def from_file(cls, filename):
        img         = localgfx.img_load(filename)
        return cls(img)

    @classmethod
    def from_dims(cls, w,h):
        img         = localgfx.img_create(w,h)
        return cls(img)


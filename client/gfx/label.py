import hwgfx

class label:
    def __init__(self, text, color, fontsize ):
        self._label = hwgfx.label_generate()
        self.color = color;
        hwgfx.label_set_text(self._label,text)

    def __del__(self):
        hwgfx.label_drop(self._label)

    def set_text(text):
        hwgfx.label_set_text(self._label,text)

    def draw(self,x,y ):
        hwgfx.label_render(self._label,x,y,1,1,1);

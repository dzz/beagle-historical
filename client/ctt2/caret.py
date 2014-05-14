_caret = None

def get_caret():
    global _caret
    return _caret

def drop():
    global _caret
    _caret = None

def acquire( ui_area ):
    global _caret
    _caret = ui_area

from    client.gfx.viewport         import nested_view
from    client.gfx.viewport         import viewport
from    client.ui.area_renderer     import area_renderer

class child_renderer(area_renderer):
    def __init__(self, ui_area):
        pass

    def render(self, ui_area):
        ui_area.layout()
        with nested_view(*(ui_area.r)):
            for child in ui_area.get_children():
                for renderer in child.renderers:
                    renderer.render(child)



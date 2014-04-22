import  client.ui.style             as style
from    client.ui.area_renderer     import area_renderer
from    client.gfx.label            import label 
from    client.gfx.rect             import rect_solid


class window_renderer(area_renderer):
    def __init__(self,ui_area):
        self.label = label( text        = ui_area.prop["titlebar_text"], 
                            color       = style.get("window_titlebar_text_color"),
                            fontsize    = style.get("window_titlebar_fontsize"))

        self.title_bgcol    = style.get("window_titlebar_bg_color")
        self.bgcol          = style.get("window_bg_color")
        self.clientcol      = style.get("window_client_color")

    def render(self, ui_area):
        rect_solid( ui_area.r, self.bgcol)
        rect_solid( [ui_area.r[0],
                         ui_area.r[1],
                         ui_area.r[2], 
                         ui_area.prop.get("titlebar_height")],
                         self.title_bgcol) 

        rect_solid  ( ui_area.compute_client_area(), self.clientcol)

        rect_solid  ( [ui_area.r[0]+ui_area.r[2] - 9,
                           ui_area.r[1]+ui_area.r[3] - 9,
                           9, 
                           9 ], 
                           self.bgcol );

        self.label.draw (ui_area.r[0], ui_area.r[1])

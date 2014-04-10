def get(key):
    style_data = {
            "window_titlebar_fontsize"      : 7.5,
            "window_titlebar_text_color"    : [ 1.0, 1.0, 1.0 ],
            "window_titlebar_bg_color"      : [ 0.3, 0.3, 0.3 ],
            "window_bg_color"               : [ 0.2, 0.2, 0.2 ],
            "window_client_color"           : [ 0.4,0.4,0.4]
            }
    return style_data.get(key)

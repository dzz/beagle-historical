def get(key):
    style_data = {
            "window_titlebar_fontsize"      : 7.5,
            "window_titlebar_text_color"    : [ 1.0, 1.0, 1.0 ],
            "window_titlebar_bg_color"      : [ 0.3, 0.3, 0.7 ],
            "window_bg_color"               : [ 0.51, 0.5, 0.55 ],
            "window_shadow_color"           : [ 0.2, 0.2, 0.2 ],
            "window_client_color"           : [ 1, 1, 1],
            "window_resize_handle_color"    : [ 0.9, 0.9, 0.9] 
            }
    return style_data.get(key)

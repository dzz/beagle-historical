
#TOP_BORDER  = chr(224)
#BOT_BORDER  = chr(221)
#CAP         = chr(266)


TOP_BORDER  = '-'
BOT_BORDER  = chr(221)
VRT_BORDER  = "|"
CAP         = "+"


def draw_interface(scr, game):

    scr.draw_str(1,1,"HOCKEY QUEST")
    scr.draw_rect(0,3,80,56,"-_|"+chr(9))
    scr.draw_rect(0,0,80,3,"-_|"+chr(9))

    scr.draw_str(1,4,"SEASON PROGRESS");
    scr.draw_rect(0,3,80,3,"-_|"+chr(9))

    scr.draw_rect(0,30,80,30,"-_|"+chr(9))
    scr.draw_rect(40,30,40,30,"-_|"+chr(9))

    scr.draw_rect(0,30,80,3,"-_|"+chr(9))
    scr.draw_rect(40,30,40,3,"-_|"+chr(9))

    scr.draw_str(2,31,"ACTIVITY")
    scr.draw_str(41,31,"PLAYER STATS")

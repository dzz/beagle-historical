from client.hockeyquest.player import player
#TOP_BORDER  = chr(224)
#BOT_BORDER  = chr(221)
#CAP         = chr(266)


TOP_BORDER  = '-'
BOT_BORDER  = chr(221)
VRT_BORDER  = "|"
CAP         = "+"

BORDERS = "--|"+chr(9)




def draw_interface(scr, game):

    #TITLEBAR

    scr.draw_str(1,1,"HOCKEY QUEST")
    scr.draw_rect(0,3,80,57,BORDERS)
    scr.draw_rect(0,0,80,3,BORDERS)
    scr.draw_str(55,1, game.date.strftime("%A %b %d, %Y") )
    scr.draw_str(37,1, "{0:02d}:{1:02d}".format( game.hour, game.minute ) )

    #####

    scr.draw_window(0,3,40,35,"Current Location",BORDERS, True)
    scr.draw_window(0,37,40,23,"Actions",BORDERS)
    scr.draw_str(41,4,"Story")

    scr.draw_str(15,20,player.Location)


    #scr.draw_str(1,4,"CURRENT LOCATION");
    #scr.draw_rect(0,3,50,3,BORDERS)

    #scr.draw_rect(0,30,80,30,BORDERS)
    #scr.draw_rect(40,30,40,30,BORDERS)

    #scr.draw_rect(0,30,80,3,BORDERS)
    #scr.draw_rect(40,30,40,3,BORDERS)

    #scr.draw_str(2,41,"ACTIVITY")
    #scr.draw_str(41,31,"PLAYER STATUS")

    #scr.draw_str(41,33,"Score:{0}".format(player.score))




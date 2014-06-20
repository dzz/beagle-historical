import client.gfx.text as text


class ascii_scr:
    def __init__(self,width,height,char_height):
        self.buffer= [0] * (width*height)
        self.width = width
        self.height = height
        self.char_height = char_height

    def draw_str(self,x,y,string):
        idx = (y*self.width)+x
        ct = 0
       
        for char in string:
            if( char=='\n'):
                y+=1
                idx = (y*self.width)+x
                ct = 0
                continue

            if( ct+x > self.width):
                ct+=1
                continue

            self.buffer[ idx+ct ] = ord(char)
            ct+=1


    def clear(self):
        self.buffer=[0]* (self.width*self.height)

    def render(self):
        for row in range(0,self.height):
            idx = row*self.width
            line = self.buffer[ idx : idx + self.width]
            compstr = ""
            for i in line:
                if i!=0:
                    compstr += chr(i)
                else: 
                    compstr += " "

            text.render_text(compstr,0,self.char_height*row)

    def v_line(self, x,y, height, line_char, cap_char = None ):

        if(cap_char == None):
            cap_char = line_char

        for row in range(0,height):
            char = line_char
            if(row==0 or row==(height-1)):
                char = cap_char
            self.draw_str(x,row+y,char)



    def h_line(self, x,y, length, line_char, cap_char = None ):
        if length < 2:
            return
        if cap_char is None:
            cap_char = line_char

        fill = cap_char + line_char*(length-2) + cap_char

        self.draw_str(x,y,fill)


    def draw_rect(self,x,y,width,height,borders):

        TOP_BORDER = borders[0]
        BOT_BORDER = borders[1]
        VRT_BORDER = borders[2]
        CAP = borders[3]

        self.h_line(x,y,width,TOP_BORDER,CAP)
        self.v_line(x,y+1,height-2,VRT_BORDER)
        self.v_line(x+width-1,y+1,height-2,VRT_BORDER)
        self.h_line(x,y+height-1,width,BOT_BORDER,CAP)

    def draw_window(self,x,y,width,height,title,borders,full_border = False):
        self.draw_rect(x,y,width,height,borders)
        self.draw_str(x+1,y+1,title)
        if(full_border):
            self.h_line(x,y+2,width,borders[0],borders[3])
        

        





def get_default_scr():
    return ascii_scr(80,60,8)

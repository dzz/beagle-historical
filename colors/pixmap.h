#ifndef __PIXMAP__
#define __PIXMAP__

typedef union { 
		unsigned int pix;
		struct {
				unsigned char a;
				unsigned char b;
				unsigned char g;
				unsigned char r;
		} p;
}  pixMap;

#endif

#ifndef console_color
#define console_color

void clrscr(void);

// position the cursor at column 'x', line 'y'
// the coodinates of upper left corner of the screen are (x,y)=(0,0)
void gotoxy(int x, int y);

// set text color
void setcolor(unsigned int color);

// set text color & background
void setcolor(unsigned int color, unsigned int background_color);

#endif

#include<stdio.h>
#include<graphics.h>
#include<iostream.h>
#include<dos.h>
#include<conio.h>
#include<fstream.h>
#include<stdlib.h>
#include<math.h>


union REGS i, o;
int xl, xr, yl, yr, clr, fg=1, th, tht, cx1, cx2, cy1, cy2;

void savefile(char*,int,int,int,int);
void loadfile(char*,int,int,int,int);

int outofbounds(int x, int y)
{
	if(x>xl && x<xr && y>yl && y<yr){
		return 0;
	}
	else{
		return 1;
	}
}

int initmouse()
{
	i.x.ax = 0;
	int86(0x33, &i, &o);
	return(o.x.ax);
}

void showmouse()
{
	i.x.ax = 1;
	int86(0x33, &i, &o);
}

void hidemouse()
{
	i.x.ax = 2;
	int86(0x33, &i, &o);
}

void getmouse(int *but, int *x, int *y)
{
	i.x.ax = 3;
	int86(0x33, &i, &o);
	*but = o.x.bx;
	*x = o.x.cx;
	*y = o.x.dx;
}


int restrictmouseptr(int x11,int y11, int x21,int y21)
{
	i.x.ax=7;
	i.x.cx=x11;
	i.x.dx=x21;
	int86(0x33,&i,&o);

	i.x.ax=8;
	i.x.cx=y11;
	i.x.dx=y21;
	int86(0x33,&i,&o);
}

int findcolor(int x)
{
	int c = (x-50)/31;
	setfillstyle(SOLID_FILL, c);
	bar(53, 453, 542, 472);
	if(c<0 || c>15){
	       c = clr;
	}
	return c;
}

void thicken(int x, int y, int f, int c)
{
	for(int i=1;i<=th;++i){
		if(f){
			if(!outofbounds(x+i,y) || fg)putpixel(x+i,y,c);
			if(!outofbounds(x-i,y) || fg)putpixel(x-i,y,c);
		}
		else{
			if(!outofbounds(x,y+i) || fg)putpixel(x,y+i,c);
			if(!outofbounds(x,y-i) || fg)putpixel(x,y-i,c);
		}
	}
}

linebres(int x1, int y1, int x2, int y2)
{
	int f,deltax = abs(x2 - x1), deltay = abs(y2 - y1), dx, dy, x, i, j, k, l, y, m, n, c = getcolor();

	if(deltax >= deltay){
		if(x2 < x1){
			x1^=x2^=x1^=x2;
			y1^=y2^=y1^=y2;
		}
		f = 0;
		dy = deltay, dx = deltax;
		i = x1, l = x2, j = 1, k = 0, m = 0, (y1 < y2) ? (n = 1) : (n = -1);
	}
	else{
		if(y2 < y1){
			x1^=x2^=x1^=x2;
			y1^=y2^=y1^=y2;
		}
		f = 1;
		dy = deltax, dx = deltay;
		i = y1, l = y2, j = 0, k = 1, (x1 < x2) ? (m = 1) : (m = -1) , n = 0;
	}
	int p = (dy<<1) - dx;
	dx = dx<<1;
	x = x1;
	y = y1;
	if(!outofbounds(x1, y1)||fg){putpixel(x1, y1, c);thicken(x1,y1,f,c);}

	for(; i < l; ++i){
		if(p < 0){
			x = x + j;
			y = y + k;
			if(!outofbounds(x,y)||fg){putpixel(x, y, c);thicken(x,y,f,c);}
			p = p + (dy<<1);
		}
		else{
			x = x + j + m;
			y = y + k + n;
			if(!outofbounds(x,y)||fg){putpixel(x, y, c);thicken(x,y,f,c);}
			p = p + (dy<<1) - dx;
		}
	}
}

circbres(float x, float y, float r)
{
	hidemouse();
	int clr,f;
	float p = 1.25 - r;
	clr=getcolor();
	//putpixel(x, y - r, 15);
	int i = 0, yt =r;
	float y1=y-r, y2 = y+r;
	float x1 = r/1.414,xt=x1;
	for(; i <= x1; ++i){
		//cout<<i<<"  ";
		if(p < 0){
			f=0;
			if(!outofbounds(x+i,y1)||fg){putpixel(x + i, y1, clr);thicken(x+i,y1,f,clr);}
			if(!outofbounds(x-i,y1)||fg){putpixel(x - i, y1, clr);thicken(x-i,y1,f,clr);}
			if(!outofbounds(x+i,y2)||fg){putpixel(x + i, y2, clr);thicken(x+i,y2,f,clr);}
			if(!outofbounds(x-i,y2)||fg){putpixel(x - i, y2, clr);thicken(x-i,y2,f,clr);}

			f=1;
			if(!outofbounds(x-(y1-y),y-i)||fg){putpixel(x-(y1-y),y-i,clr);thicken(x-(y1-y),y-i,f,clr);}
			if(!outofbounds(x+(y1-y),y-i)||fg){putpixel(x+(y1-y),y-i,clr);thicken(x+(y1-y),y-i,f,clr);}
			if(!outofbounds(x-(y1-y),y+i)||fg){putpixel(x-(y1-y),y+i,clr);thicken(x-(y1-y),y+i,f,clr);}
			if(!outofbounds(x+(y1-y),y+i)||fg){putpixel(x+(y1-y),y+i,clr);thicken(x+(y1-y),y+i,f,clr);}
			p = p + (i<<1) + 1;
		}
		else{
			++y1;
			--y2;
			f=0;
			if(!outofbounds(x+i,y1)||fg){putpixel(x + i, y1, clr);thicken(x+i,y1,f,clr);}
			if(!outofbounds(x-i,y1)||fg){putpixel(x - i, y1, clr);thicken(x-i,y1,f,clr);}
			if(!outofbounds(x+i,y2)||fg){putpixel(x + i, y2, clr);thicken(x+i,y2,f,clr);}
			if(!outofbounds(x-i,y2)||fg){putpixel(x - i, y2, clr);thicken(x-i,y2,f,clr);}

			f=1;
			if(!outofbounds(x-(y1-y),y-i)||fg){putpixel(x-(y1-y),y-i,clr);thicken(x-(y1-y),y-i,f,clr);}
			if(!outofbounds(x+(y1-y),y-i)||fg){putpixel(x+(y1-y),y-i,clr);thicken(x+(y1-y),y-i,f,clr);}
			if(!outofbounds(x-(y1-y),y+i)||fg){putpixel(x-(y1-y),y+i,clr);thicken(x-(y1-y),y+i,f,clr);}
			if(!outofbounds(x+(y1-y),y+i)||fg){putpixel(x+(y1-y),y+i,clr);thicken(x+(y1-y),y+i,f,clr);}

			--yt;
			p = p + (i<<1) + 1 - (yt<<1);
		}
		//delay(10);
	}
	if(!outofbounds(x+i-1,y1+1)||fg){putpixel(x + i-1, y1+1, clr);thicken(x+i-1,y1+1,1,clr);}
	if(!outofbounds(x-i,y1+1)||fg){putpixel(x - i, y1+1, clr);thicken(x-i,y1+1,1,clr);}
	if(!outofbounds(x-i,y2)||fg){putpixel(x - i, y2, clr);thicken(x-i,y2,0,clr);}
	showmouse();

}
int ellip(float rx,float ry,float xc,float yc)
{
	float p1[200],p2[200],x,y,k=0;
	x=0;
	y=ry;

	if(!outofbounds(xc,yc+ry)||fg){putpixel(xc,yc+ry,clr);thicken(xc,yc+ry,1,clr);}
	if(!outofbounds(xc,yc-ry)||fg){putpixel(xc,yc-ry,clr);thicken(xc,yc-ry,1,clr);}
	p1[0]=ry*ry-ry*rx*rx+(0.25)*rx*rx;

	while((x)<((rx/ry)*(rx/ry)*(y))){
		if(p1[k]<0){
			x++;
			p1[k+1]=p1[k]+2*ry*ry*x+3*ry*ry;
			if(!outofbounds(x+xc,y+yc)||fg){putpixel(x+xc,y+yc,clr);thicken(x+xc,y+yc,1,clr);}
			if(!outofbounds(x+xc,-y+yc)||fg){putpixel(x+xc,-y+yc,clr);thicken(x+xc,y+yc,1,clr);}
			if(!outofbounds(-x+xc,y+yc)||fg){putpixel(-x+xc,y+yc,clr);thicken(-x+xc,y+yc,1,clr);}
			if(!outofbounds(-x+xc,-y+yc)||fg){putpixel(-x+xc,-y+yc,clr);thicken(-x+xc,y+yc,1,clr);}
		}
		if(p1[k]>0){
			x++;
			y--;
			p1[k+1]=p1[k]+(2*x+3)*ry*ry-2*(y-1)*rx*rx;
			if(!outofbounds(x+xc,y+yc)||fg){putpixel(x+xc,y+yc,clr);thicken(x+xc,y+yc,1,clr);}
			if(!outofbounds(x+xc,-y+yc)||fg){putpixel(x+xc,-y+yc,clr);thicken(x+xc,-y+yc,1,clr);}
			if(!outofbounds(-x+xc,y+yc)||fg){putpixel(-x+xc,y+yc,clr);thicken(-x+xc,y+yc,1,clr);}
			if(!outofbounds(-x+xc,-y+yc)||fg){putpixel(-x+xc,-y+yc,clr);thicken(-x+xc,-y+yc,1,clr);}
		}
		k++;
	}

	k=0;
	p2[0]=ry*ry*(x+0.5)*(x+0.5)+(y-1)*(y-1)*rx*rx-rx*rx*ry*ry;

	while(y>0){
		if(p2[k]>0){
			y--;
			p2[k+1]=p2[k]-2*rx*rx*y+3*rx*rx;
			if(!outofbounds(x+xc,y+yc)||fg){putpixel(x+xc,y+yc,clr);thicken(x+xc,y+yc,1,clr);}
			if(!outofbounds(x+xc,-y+yc)||fg){putpixel(x+xc,-y+yc,clr);thicken(x+xc,-y+yc,1,clr);}
			if(!outofbounds(-x+xc,y+yc)||fg){putpixel(-x+xc,y+yc,clr);thicken(-x+xc,y+yc,1,clr);}
			if(!outofbounds(-x+xc,-y+yc)||fg){putpixel(-x+xc,-y+yc,clr);thicken(-x+xc,-y+yc,1,clr);}
		}
		if(p2[k]<=0){
			y--;
			x++;
			p2[k+1]=p2[k]+2*ry*ry*(x+1)-rx*rx*(2*y-3);
			if(!outofbounds(x+xc,y+yc)||fg){putpixel(x+xc,y+yc,clr);thicken(x+xc,y+yc,1,clr);}
			if(!outofbounds(x+xc,-y+yc)||fg){putpixel(x+xc,-y+yc,clr);thicken(x+xc,-y+yc,1,clr);}
			if(!outofbounds(-x+xc,y+yc)||fg){putpixel(-x+xc,y+yc,clr);thicken(-x+xc,y+yc,1,clr);}
			if(!outofbounds(-x+xc,-y+yc)||fg){putpixel(-x+xc,-y+yc,clr);thicken(-x+xc,-y+yc,1,clr);}
		}
		k++;
	}
}


void myrect(int x1, int y1, int x2, int y2)
{
	linebres(x1, y1, x1, y2);
	linebres(x1, y2, x2, y2);
	linebres(x2, y2, x2, y1);
	linebres(x2, y1, x1, y1);
}

void findth(int y)
{
	hidemouse();
	if(y > 110 && y<140){
		tht=0;
		th=0;
		setcolor(LIGHTRED);
		linebres(570, 120, 600, 120);
		setcolor(15);
		th = 1;
		linebres(570, 160, 600, 160);
		th = 2;
		linebres(570, 200, 600, 200);
		th = 3;
		linebres(570, 240, 600, 240);
		th = 4;
		linebres(570, 280, 600, 280);
	}
	else if(y>140 && y<180){
		tht = 1;
		th = 0;
		setcolor(15);
		linebres(570, 120, 600, 120);
		setcolor(LIGHTRED);
		th = 1;
		linebres(570, 160, 600, 160);
		th = 2;
		setcolor(15);
		linebres(570, 200, 600, 200);
		th = 3;
		linebres(570, 240, 600, 240);
		th = 4;
		linebres(570, 280, 600, 280);
	}
	else if(y>180 && y<210){
		tht = 2;
		th = 0;
		setcolor(15);
		linebres(570, 120, 600, 120);
		th = 1;
		linebres(570, 160, 600, 160);
		th = 2;
		setcolor(LIGHTRED);
		linebres(570, 200, 600, 200);
		setcolor(15);
		th = 3;
		linebres(570, 240, 600, 240);
		th = 4;
		linebres(570, 280, 600, 280);
	}
	else if(y>210 && y<260){
		tht = 3;
		th = 0;
		setcolor(15);
		linebres(570, 120, 600, 120);
		th = 1;
		linebres(570, 160, 600, 160);
		th = 2;
		linebres(570, 200, 600, 200);
		setcolor(LIGHTRED);
		th = 3;
		linebres(570, 240, 600, 240);
		th = 4;
		setcolor(15);
		linebres(570, 280, 600, 280);
	}
	else if(y>260 && y<300){
		tht = 4;
		th = 0;
		setcolor(15);
		linebres(570, 120, 600, 120);
		th = 1;
		linebres(570, 160, 600, 160);
		th = 2;

		linebres(570, 200, 600, 200);
		th = 3;
		linebres(570, 240, 600, 240);
		th = 4;
		setcolor(LIGHTRED);
		linebres(570, 280, 600, 280);
	}

	th = 0;
	showmouse();
}

int process(int x, int y, int b, int r)
{
	if(y>420 && y<440 && b==1){
		clr=findcolor(x);
		return r;
	}
	else if(x>570 && x<600 && y>110 && y<300){
		findth(y);
		return r;
	}
	else if(x>600 && x<620 && y>10 && y<30 && b==1){
		outtextxy(1,1,"Quitting...");
		return 0;
	}
	else if(x>10 && x<30 && y>110 && y<130 && b==1){

		return 2;
	}
	else if(x>10 && x<30 && y>70 && y<90 && b==1){
		return 1;
	}
	else if(x>570 && x<600 && y>70 && y<90 && b==1){
		setfillstyle(SOLID_FILL, 0);
		bar(51, 51, 549, 399);
		return r;
	}
	else if(x>10 && x<30 && y>150 && y<170 && b==1){
		return 3;
	}
	else if(x>10 && x<40 && y>190 && y<210 && b==1){
		return 4;
	}
	else if(x>10 && x<40 && y>230 && y<250 && b==1){
		return 5;
	}
	else if(x>10 && x<30 && y>270 && y<290 && b==1){
		return 6;
	}
	else if(x>55 && x<95 && y>25 && y<50 && b==1){
		return 7;
	}
	else if(x>95 && x<140 && y>25 && y<50 && b==1){
		return 8;
	}
	else if(x>140 && x<210 && y>25 && y<50 && b==1){
		return 9;
	}
	else if(x>210 && x<280 && y>25 && y<50 && b==1){
		return 10;
	}
	else if(x>280 && x<350 && y>25 && y<50 && b==1){
		return 11;
	}
	else if(x>10 && x<30 && y>310 && y<330){
		return 12;
	}
}

int pencil()
{
	int b, x, y, px, py;
	fg=0;
	th=tht;

	while(1){
		setcolor(clr);

		getmouse(&b, &x, &y);
		gotoxy(65,1);
		printf("%d , %d  ",x-50,y-50);

		if(outofbounds(x,y) && b){
			fg=1;
			th=0;
			return (process(x,y,b,1));
		}
		if(b&1){
			hidemouse();
			px = x;
			py = y;

		}
		while(b&1){
			linebres(px, py, x, y);
			px = x;
			py = y;
			getmouse(&b, &x, &y);
			gotoxy(65,1);
			printf("%d , %d  ",x-50,y-50);
		}
		showmouse();

	}
	fg = 1;
	th = 0;
}

int lines()
{
	fg = 0;
	th = tht;
	int b, x, y, px, py,px2,py2,g=1;

	while(1){
		setcolor(clr);

		getmouse(&b, &x, &y);
		gotoxy(65,1);
		printf("%d , %d  ",x-50,y-50);

		if(outofbounds(x,y) && b){
			fg=1;
			th=0;
			return (process(x,y,b,2));
		}
		if(b&1 && !outofbounds(x,y)){
			putpixel(x,y,clr);
			px = x;
			py = y;
			g=1;
			while(g){
				putpixel(px,py,clr);
				getmouse(&b, &x, &y);
				gotoxy(65,1);
				printf("%d , %d  ",x-50, y-50);
				if(b==2){
					hidemouse();
					g=0;
					linebres(px,py,x,y);
				}
			}
		}
		showmouse();

	}
	fg = 1;
	th = 0;
}

int ellips()
{
	int b, x, y, px, py,px2,py2,g=1;
	fg = 0;
	th = tht;
	while(1){
		setcolor(clr);

		getmouse(&b, &x, &y);
		gotoxy(65,1);
		printf("%d , %d  ",x-50,y-50);

		if(outofbounds(x,y) && b){
			fg=1;
			th=0;
			return (process(x,y,b,12));
		}
		if(b&1 && !outofbounds(x,y)){
			px = x;
			py = y;
			g=1;
			while(g){
				getmouse(&b, &x, &y);
				gotoxy(65,1);
				printf("%d , %d  ",x-50, y-50);
				if(b==2){
					hidemouse();
					g=0;
					ellip(abs(x-px)/2,abs(y-py)/2,(px+x)/2, (py+y)/2);
				}
			}
		}
		showmouse();

	}
	fg = 1;
	th = 0;
}


int polygons()
{
	int b, x, y, px, py,px2,py2,g=1,xi,yi;

	fg = 0;
	th = tht;
	while(g){
		setcolor(clr);

		getmouse(&b, &x, &y);
		gotoxy(65,1);
		printf("%d , %d  ",x-50,y-50);

		if(outofbounds(x,y) && b){
			fg=1;
			th=0;
			return (process(x,y,b,5));
		}
		if(b&1 && !outofbounds(x,y)){
			px = xi = x;
			py = yi = y;
			g = 0;
		}
	}
	while(1){
		setcolor(clr);

		getmouse(&b, &x, &y);
		gotoxy(65,1);
		printf("%d , %d  ",x-50,y-50);

		if(outofbounds(x,y) && b){
			fg=1;
			th=0;
			return (process(x,y,b,5));
		}
		if(b&1){
			hidemouse();
			linebres(px,py,x,y);
			px = x;
			py = y;
		}
		else if(b&2){
			hidemouse();
			linebres(xi,yi,px,py);
			showmouse();
			fg = 1;
			th = 0;
			return 5;
		}

		showmouse();
	}
	fg = 1;
	th = 0;

}



int circles()
{
	int b, x, y, px, py,px2,py2,g=1;
	fg = 0;
	th = tht;
	while(1){
		setcolor(clr);

		getmouse(&b, &x, &y);
		gotoxy(65,1);
		printf("%d , %d  ",x-50,y-50);

		if(outofbounds(x,y) && b){
			fg=1;
			th=0;
			return (process(x,y,b,3));
		}
		if(b&1 && !outofbounds(x,y)){
			putpixel(x,y,clr);
			px = x;
			py = y;
			g=1;
			while(g){
				putpixel(px,py,clr);
				getmouse(&b, &x, &y);
				gotoxy(65,1);
				printf("%d , %d  ",x-50, y-50);
				if(b==2){
					hidemouse();
					g=0;
					circbres((px+x)/2, (py+y)/2, (sqrt(pow(px-x,2) + pow(py-y,2)))/2);
				}
			}
		}
		showmouse();

	}
	fg = 1;
	th = 0;
}

int rect()
{
	fg = 0;
	th = tht;
	int b, x, y, px, py,px2,py2,g=1;

	while(1){
		setcolor(clr);

		getmouse(&b, &x, &y);
		gotoxy(65,1);
		printf("%d , %d  ",x-50,y-50);

		if(outofbounds(x,y) && b){
			fg=1;
			th=0;
			return (process(x,y,b,6));
		}
		if(b&1 && !outofbounds(x,y)){
			putpixel(x,y,clr);
			px = x;
			py = y;
			g=1;
			while(g){
				putpixel(px,py,clr);
				getmouse(&b, &x, &y);
				gotoxy(65,1);
				printf("%d , %d  ",x-50, y-50);
				if(b==2){
					hidemouse();
					g=0;
					myrect(px,py,x,y);
				}
			}
		}
		showmouse();

	}
	fg = 1;
	th = 0;
}

int paste()
{
	int b, x, y, g;
	getmouse(&b,&x,&y);
	while(b){
		getmouse(&b,&x,&y);
	}
	b=0;
	showmouse();
	while(!b){
		getmouse(&b,&x,&y);
		gotoxy(65,1);
		printf("%d , %d  ",x-50,y-50);
	}
	hidemouse();
	fg = 0;
	loadfile("temp.img",x,y,x+(cx2-cx1),y+(cy2-cy1));
	fg = 1;
	return 1;
}

int cut()
{
	fg = 0;
	th = 0;
	int b, x, y, px, py,px2,py2,g=1;

	while(g){
		setcolor(clr);

		getmouse(&b, &x, &y);
		gotoxy(65,1);
		printf("%d , %d  ",x-50,y-50);

		if(outofbounds(x,y) && b){
			fg=1;
			th=0;
			return (process(x,y,b,9));
		}
		if(b&1 && !outofbounds(x,y)){
			putpixel(x,y,clr);
			px = x;
			py = y;
			g=1;
			while(g){
				putpixel(px,py,clr);
				getmouse(&b, &x, &y);
				gotoxy(65,1);
				printf("%d , %d  ",x-50, y-50);
				if(b==2){
					hidemouse();
					g=0;
					myrect(px,py,x,y);
				}
			}
		}
		showmouse();


	}
	getch();
	savefile("temp.img",px+1,py+1,x-1,y-1);
	setfillstyle(SOLID_FILL,0);
	cx1=px+1,cx2=x-1,cy1=py+1,cy2=y-1;
	bar(px,py,x,y);
	fg = 1;
	th = 0;
	return 1;

}

void fill_right(int x,int y,int c)
{
	if((getpixel(x,y) == c)){
		putpixel(x,y,clr);
		fill_right(++x,y,c);
		x=x-1;
		fill_right(x,y-1,c);
		fill_right(x,y+1,c);
	}
}

void fill_left(int x,int y,int c)
{
	if((getpixel(x,y) == c)){
		putpixel(x,y,clr);
		fill_left(--x,y,c);
		x = x + 1 ;
		fill_left(x,y-1,c);
		fill_left(x,y+1,c);
	}
}

int fillstart()
{
	int b, x, y;

	while(1){
		setcolor(clr);

		getmouse(&b, &x, &y);
		gotoxy(65,1);
		printf("%d , %d  ",x-50,y-50);

		if(outofbounds(x,y) && b){
			return (process(x,y,b,4));
		}
		if(b&1 && !outofbounds(x,y)){
			hidemouse();
			fill_right(x, y,getpixel(x,y));
			fill_left(x-1, y,getpixel(x-1,y));
			showmouse();
			getmouse(&b,&x,&y);
			while(b){
				getmouse(&b,&x,&y);
			}
			return 4;
		}
	}
}

struct pixel
{
	int t;
}ob;

void savefile(char *s,int x1, int y1, int x2, int y2)
{
	hidemouse();
	ofstream ofile;
	int t;
	ofile.open(s,ios::binary);
	for(int i = x1; i< x2; ++i){
		for(int j = y1; j < y2; ++j){
			ob.t = getpixel(i,j);
			ofile.write((char*)&ob, sizeof(ob));

		}

	}
	ofile.close();
}

void loadfile(char *s,int x1,int y1,int x2,int y2)
{
	//cout<<s;
	ifstream ifile;
	int t;


	ifile.open(s,ios::binary);
	for(int i = x1; i<x2;++i){
		for(int j = y1; j<y2;++j){
			ifile.read((char*)&ob,sizeof(ob));

			if(!outofbounds(i,j)||fg){putpixel(i, j, ob.t);}
		}
	}
	ifile.close();

}

void loadstretch(char *s,int x1,int y1,int xf, int yf,int x3, int y3)
{
	//cout<<s;
	ifstream ifile;
	int t,k,l;

	//float xf=(x3-x1)/(x2-x1),yf=(y3-y1)/(y2-y1);
	//cout<<xf<<" "<<yf;
	ifile.open(s,ios::binary);
	for(int i = x1; i<x3;){
		for(int j = y1; j<y3 ;){
			ifile.read((char*)&ob,sizeof(ob));
			for(k=0;k<yf;++k,++j){
				if(!outofbounds(i,j)||fg){putpixel(i,j, ob.t);}
			}
		}
		++i;
		for(l=1;l<xf;++l,++i){
			for(int j = y1; j<y3;){
				for(k=0;k<yf;++k,++j){
					if(!outofbounds(i,j)||fg){putpixel(i, j, getpixel(i-1,j));}
				}
			}
		}
	}
	ifile.close();

}

int stretch(int sx, int sy)
{
	fg = 0;
	th = 0;
	int b, x, y, px, py,px2,py2,g=1;

	while(g){
		setcolor(clr);

		getmouse(&b, &x, &y);
		gotoxy(65,1);
		printf("%d , %d  ",x-50,y-50);

		if(outofbounds(x,y) && b){
			fg=1;
			th=0;
			return (process(x,y,b,9));
		}
		if(b&1 && !outofbounds(x,y)){
			putpixel(x,y,clr);
			px = x;
			py = y;
			g=1;
			while(g){
				putpixel(px,py,clr);
				getmouse(&b, &x, &y);
				gotoxy(65,1);
				printf("%d , %d  ",x-50, y-50);
				if(b==2){
					hidemouse();
					g=0;
					myrect(px,py,x,y);
				}
			}
		}
		showmouse();


	}
	getch();
	savefile("temp.img",px+1,py+1,x-1,y-1);
	loadstretch("temp.img",px,py,sx,sy,px+sx*(x-1-px-1),py+sy*(y-1-py-1));
	fg = 1;
	th = 0;
	return 1;
}


void main()
{
	th = 0;
	tht = 1;
	int gdriver = DETECT, gmode, errorcode;
	char *s;
	int xmax, ymax, f = 1, b, x, y, t, sx, sy;

	initgraph(&gdriver, &gmode, "\\bgi");


	/* read result of initialization */
	errorcode = graphresult();
	/* an error occurred */
	if (errorcode != grOk){
		printf("Graphics error: %s\n", grapherrormsg(errorcode));
		printf("Press any key to halt:");
		getch();
		exit(1);
	}
	setviewport(0,0,639,400,0);
	clearviewport();
	loadstretch("intro.img",70,55,1,1,70+(499)*2,55+(349)*1);
	getch();
	cleardevice();


	initmouse();
	showmouse();
	xl = 50;
	xr = 550;
	yl = 50;
	yr = 400;
	hidemouse();
	myrect(50, 50, 550, 400);
	clr = 15;
	setfillstyle(SOLID_FILL, clr);
	bar(53, 453, 542, 472);
	myrect(600, 10, 620, 30);
	outtextxy(607,16,"x");
	outtextxy(10, 76, "pen");
	outtextxy(9, 196, "fill");
	outtextxy(9,236,"poly");
	myrect(10, 270, 30, 290);
	ellipse(20, 320, 0,360,15, 10);
	linebres(10, 110, 30, 130);
	circbres(20, 160, 10);
	outtextxy(567,76,"clear");
	outtextxy(60, 30, "Load");
	outtextxy(120, 30, "Save");
	outtextxy(180, 30, "Cut");
	outtextxy(230, 30, "Paste");
	outtextxy(290, 30, "Stretch");
	th=0;
	setcolor(LIGHTRED);
	linebres(570,120,600,120);
	th=1;
	clr=15;
	setcolor(clr);
	linebres(570, 160, 600, 160);
	th=2;
	linebres(570, 200, 600, 200);
	th = 3;
	linebres(570, 240, 600, 240);
	th = 4;
	linebres(570, 280, 600, 280);
	th=0;


	for(int i = 0; i<16;++i){
		setfillstyle(SOLID_FILL, i);
		bar(50+i*31+2, 420, 50+i*31+25+2, 440);
		linebres(50+i*31+28+2, 417, 50+i*31+28+2, 443);
	}
	myrect(50, 417, 545, 443);
	myrect(50, 450, 545, 475);
	showmouse();
	th = 0;
	tht = 0;
	while(f){
		hidemouse();
		if(f==1){
			setcolor(LIGHTRED);
			outtextxy(10, 76, "pen");
			showmouse();
			setcolor(clr);
			f=pencil();
		}
		else if(f==2){
			setcolor(LIGHTRED);
			linebres(10, 110, 30, 130);
			setcolor(15);
			gotoxy(1,1);
			printf("Left Click the start point and right click the end point");
			showmouse();
			setcolor(clr);
			f=lines();
			showmouse();
			gotoxy(1,1);
			printf("\t\t\t\t\t\t\t\t");
		}
		else if(f==3){
			setcolor(LIGHTRED);
			circbres(20, 160, 10);
			setcolor(15);
			gotoxy(1,1);
			printf("Left click start and right click end point");
			showmouse();
			setcolor(clr);
			f=circles();
			gotoxy(1,1);
			printf("\t\t\t\t\t\t\t\t");
		}
		else if(f==4){
			setcolor(LIGHTRED);
			outtextxy(9,196,"fill");
			showmouse();
			setcolor(clr);
			f=fillstart();
			showmouse();
		}
		else if(f==5){
			setcolor(LIGHTRED);
			outtextxy(9, 236, "poly");
			showmouse();
			setcolor(clr);
			f=polygons();
			showmouse();
		}
		else if(f==6){
			setcolor(LIGHTRED);
			myrect(10, 270, 30, 290);
			setcolor(15);
			gotoxy(1,1);
			printf("Left click start and right click end point");
			showmouse();
			setcolor(clr);
			f=rect();
			showmouse();
			gotoxy(1,1);
			printf("\t\t\t\t\t\t\t\t");
		}
		else if(f==7){
			setcolor(LIGHTRED);
			outtextxy(60, 30, "Load");
			setcolor(15);
			gotoxy(1,1);
			printf("Enter File Name:");
			gets(s);
			gotoxy(1,1);
			printf("Loading...\t\t\t\t\t\t");
			loadfile(s,51,51,550,400);
			gotoxy(1,1);
			printf("\t\t\t\t\t\t\t\t");
			f=1;
		}
		else if(f==8){
			setcolor(LIGHTRED);
			outtextxy(120, 30, "Save");
			setcolor(15);
			gotoxy(1,1);
			printf("Enter File Name:");
			gets(s);
			gotoxy(1,1);
			printf("Saving...\t\t\t\t\t\t");
			savefile(s,51,51,550,400);
			gotoxy(1,1);
			printf("\t\t\t\t\t");
			f=1;
		}
		else if(f==9){
			setcolor(LIGHTRED);
			outtextxy(180, 30, "Cut");
			setcolor(15);
			gotoxy(1,1);
			printf("Draw clipping rectangle and then press any key to cut");
			f=cut();
			gotoxy(1,1);
			printf("\t\t\t\t\t\t\t\t");
		}
		else if(f==10){
			setcolor(LIGHTRED);
			outtextxy(230, 30, "Paste");
			setcolor(15);
			showmouse();
			f=paste();
		}
		else if(f==11){
			setcolor(LIGHTRED);
			outtextxy(290, 30, "Stretch");
			setcolor(15);
			gotoxy(1,1);
			printf("Enter x and y scale factors: ");
			scanf("%d %d",&sx,&sy);
			gotoxy(1,1);
			printf("\t\t\t\t\t\t\t");
			f=stretch(sx,sy);
		}
		else if(f==12){
			setcolor(LIGHTRED);
			ellipse(20, 320, 0,360,15, 10);
			setcolor(15);
			gotoxy(1,1);
			printf("Left click start and right click end point");
			showmouse();
			setcolor(clr);
			f=ellips();
			gotoxy(1,1);
			printf("\t\t\t\t\t\t\t\t");
		}
		setcolor(15);
		outtextxy(10, 76, "pen");
		linebres(10, 110, 30, 130);
		circbres(20, 160, 10);
		ellipse(20, 320, 0,360,15, 10);
		outtextxy(9, 196, "fill");
		outtextxy(9,236,"poly");
		myrect(10, 270, 30, 290);
		outtextxy(60, 30, "Load");
		outtextxy(120, 30, "Save");
		outtextxy(180, 30, "Cut");
		outtextxy(230, 30, "Paste");
		outtextxy(290, 30, "Stretch");

	}
}
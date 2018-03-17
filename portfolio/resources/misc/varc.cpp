/*
 *		This Code Was Created By Jeff Molofee 2000
 *		A HUGE Thanks To Fredric Echols For Cleaning Up
 *		And Optimizing This Code, Making It More Flexible!
 *		If You've Found This Code Useful, Please Let Me Know.
 *		Visit My Site At nehe.gamedev.net
 */

#include <math.h>
#include <stdio.h>			// Header File For Standard Input/Output
#include <windows.h>		// Header File For Windows
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#include <gl\glaux.h>		// Header File For The Glaux Library
#include <commctrl.h>
#include "resource.h"
#include "Object3DS.h"
#include "ObjectImporter.h"

#pragma comment(lib, "COMCTL32.lib")


HDC			hDC=NULL;		// Private GDI Device Context
HGLRC		hRC=NULL;		// Permanent Rendering Context
HWND		hWnd=NULL;		// Holds Our Window Handle
HINSTANCE	hInstance;		// Holds The Instance Of The Application


const char g_szClassName[] = "myWindowClass";

const char g_szClassName2[] = "myWindowClass2";

HWND hWnd_2DFrame;					// The 2D frame

HWND hWnd_Options;

HWND button_brick_wall, button_mossy_wall, button_rocky_wall, button_stony_wall;

HWND button_dark_brown_chair, button_light_brown_chair, button_rusty_chair;

HWND button_top_chair, button_right_chair, button_down_chair, button_left_chair;

HWND button_dark_brown_table, button_light_brown_table, button_rusty_table;

HWND button_top_table, button_right_table, button_down_table, button_left_table;

HWND button_1_door, button_2_door, button_3_door;

HWND button_top_door, button_right_door, button_down_door, button_left_door;

HWND edit_chair, edit_table, edit_wall, edit_door;

HWND label_chair, label_table, label_wall, label_door;



int lastx,lasty,x,y;		//GLOBAL VARIABLES used in drawing.

bool	keys[256];			// Array Used For The Keyboard Routine
bool	active=TRUE;		// Window Active Flag Set To TRUE By Default
bool	fullscreen=TRUE;	// Fullscreen Flag Set To Fullscreen Mode By Default

bool wallClicked = FALSE;
bool chairClicked = FALSE;
bool tableClicked = FALSE;
bool doorClicked = FALSE;

float piover180 = 0.0174532925f;
float heading;
float xpos;
float zpos;

float ypos;

float xrot;				// X Rotation ( NEW )
float yrot;				// Y Rotation ( NEW )
float zrot;				// Z Rotation ( NEW )
 
float walkbias = 0;
float walkbiasangle = 0;
float lookupdown = 0.0f;
float z=0.0f;				// Depth Into The Screen

int wallListx[100][4];
int wallListy[100][4];
int wallListTex[100];
int wallCost[100];
int noOfWalls = 0;

int chairList[100][2];
int chairTex[100];
int chairDir[100];
int chairCost[100];
int noOfChairs = 0;

int factor2d3d = 1;

int tableList[100][2];
int tableTex[100];
int tableDir[100];
int tableCost[100];
int noOfTables = 0;

int doorList[100][2];
int doorTex[100];
int doorDir[100];
int doorCost[100];
int noOfDoors = 0;

int extremePoints[2][2]={{10000,10000},{-10000,-10000}};

GLuint	texture[5];

Object3DS room_chair[100], room_table[100], room_door[100];

int totalCost = 0;

float light1Position[] = {10.0f, 10.0f,10.0f, 1.0f};		// Holds our light position
float light1Ambient[]  = {2.0f, 2.0f, 2.0f, 1.0f};		// Ambient light
float light1Diffuse[]  = {0.0f, 0.0f, 0.0f, 1.0f};	// Diffuse light
float light1Specular[] = {10.0f, 10.0f, 10.0f, 1.0f};	// Specular light

float light2Position[] = {100.0f, 10.0f,100.0f, 1.0f};		// Holds our light position
float light2Ambient[]  = {1.0f, 1.0f, 1.0f, 1.0f};		// Ambient light
float light2Diffuse[]  = {0.0f, 0.0f, 0.0f, 1.0f};	// Diffuse light
float light2Specular[] = {10.0f, 10.0f, 10.0f, 1.0f};	// Specular light


bool light0 = FALSE, light1 = FALSE;


LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// Declaration For WndProc


AUX_RGBImageRec *LoadBMP(char *Filename)				// Loads A Bitmap Image
{
	FILE *File=NULL;									// File Handle

	if (!Filename)										// Make Sure A Filename Was Given
	{
		return NULL;									// If Not Return NULL
	}

	File=fopen(Filename,"r");							// Check To See If The File Exists

	if (File)											// Does The File Exist?
	{
		fclose(File);									// Close The Handle
		return auxDIBImageLoad(Filename);				// Load The Bitmap And Return A Pointer
	}

	return NULL;										// If Load Failed Return NULL
}

int LoadGLTextures()									// Load Bitmaps And Convert To Textures
{
	int Status=FALSE;									// Status Indicator

	AUX_RGBImageRec *TextureImage[5];					// Create Storage Space For The Texture

	memset(TextureImage,0,sizeof(void *)*1);           	// Set The Pointer To NULL
	memset(TextureImage,1,sizeof(void *)*1);           	// Set The Pointer To NULL
	memset(TextureImage,2,sizeof(void *)*1);           	// Set The Pointer To NULL
	memset(TextureImage,3,sizeof(void *)*1);           	// Set The Pointer To NULL

	// Load The Bitmap, Check For Errors, If Bitmap's Not Found Quit
	if (TextureImage[0]=LoadBMP("Data/wall_brick.bmp"))
	{
		Status=TRUE;									// Set The Status To TRUE

		glGenTextures(1, &texture[0]);					// Create The Texture

		// Typical Texture Generation Using Data From The Bitmap
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	}

	if (TextureImage[0])									// If Texture Exists
	{
		if (TextureImage[0]->data)							// If Texture Image Exists
		{
			free(TextureImage[0]->data);					// Free The Texture Image Memory
		}

		free(TextureImage[0]);								// Free The Image Structure
	}

	if (TextureImage[1]=LoadBMP("Data/wall_mossy.bmp"))
	{
		Status=TRUE;									// Set The Status To TRUE

		glGenTextures(1, &texture[1]);					// Create The Texture

		// Typical Texture Generation Using Data From The Bitmap
		glBindTexture(GL_TEXTURE_2D, texture[1]);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[1]->sizeX, TextureImage[1]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[1]->data);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	}

	if (TextureImage[1])									// If Texture Exists
	{
		if (TextureImage[1]->data)							// If Texture Image Exists
		{
			free(TextureImage[1]->data);					// Free The Texture Image Memory
		}

		free(TextureImage[1]);								// Free The Image Structure
	}

	if (TextureImage[2]=LoadBMP("Data/wall_rocky.bmp"))
	{
		Status=TRUE;									// Set The Status To TRUE

		glGenTextures(1, &texture[2]);					// Create The Texture

		// Typical Texture Generation Using Data From The Bitmap
		glBindTexture(GL_TEXTURE_2D, texture[2]);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[2]->sizeX, TextureImage[2]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[2]->data);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	}

	if (TextureImage[2])									// If Texture Exists
	{
		if (TextureImage[2]->data)							// If Texture Image Exists
		{
			free(TextureImage[2]->data);					// Free The Texture Image Memory
		}

		free(TextureImage[2]);								// Free The Image Structure
	}

	if (TextureImage[3]=LoadBMP("Data/wall_stony.bmp"))
	{
		Status=TRUE;									// Set The Status To TRUE

		glGenTextures(1, &texture[3]);					// Create The Texture

		// Typical Texture Generation Using Data From The Bitmap
		glBindTexture(GL_TEXTURE_2D, texture[3]);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[3]->sizeX, TextureImage[3]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[3]->data);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	}

	if (TextureImage[3])									// If Texture Exists
	{
		if (TextureImage[3]->data)							// If Texture Image Exists
		{
			free(TextureImage[3]->data);					// Free The Texture Image Memory
		}

		free(TextureImage[3]);								// Free The Image Structure
	}


	if (TextureImage[4]=LoadBMP("Data/FloorDrt.bmp"))
	{
		Status=TRUE;									// Set The Status To TRUE

		glGenTextures(1, &texture[4]);					// Create The Texture

		// Typical Texture Generation Using Data From The Bitmap
		glBindTexture(GL_TEXTURE_2D, texture[4]);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[4]->sizeX, TextureImage[4]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[4]->data);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	}

	if (TextureImage[4])									// If Texture Exists
	{
		if (TextureImage[4]->data)							// If Texture Image Exists
		{
			free(TextureImage[4]->data);					// Free The Texture Image Memory
		}

		free(TextureImage[4]);								// Free The Image Structure
	}


	return Status;										// Return The Status
}





GLvoid ReSizeGLScene(GLsizei width, GLsizei height)		// Resize And Initialize The GL Window
{
	if (height==0)										// Prevent A Divide By Zero By
	{
		height=1;										// Making Height Equal One
	}

	glViewport(0,0,width,height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
}

int InitGL(GLvoid)										// All Setup For OpenGL Goes Here
{

	if (!LoadGLTextures())								// Jump To Texture Loading Routine ( NEW )
	{
		return FALSE;									// If Texture Didn't Load Return FALSE
	}
	// Enable light
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	light0=TRUE;
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light1Diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR,light1Specular);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light1Ambient);

	glEnable(GL_LIGHT1);
	light1=TRUE;
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light2Diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR,light2Specular);
	glLightfv(GL_LIGHT1, GL_AMBIENT, light2Ambient);


	glEnable(GL_TEXTURE_2D);							// Enable Texture Mapping ( NEW )
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do

	//CreateTexture(texture,"Data/wall_brick.bmp",1);
	
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
	return TRUE;										// Initialization Went OK
}

int DrawGLScene(GLvoid)									// Here's Where We Do All The Drawing
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
	glLoadIdentity();									// Reset The Current Modelview Matrix
	glClearColor(0.5f,0.5f,0.5f,1.0f);			// We'll Clear To The Color Of The Fog ( Modified )
	GLfloat fogColor[4]= {0.5f, 0.5f, 0.5f, 1.0f};
	glFogi(GL_FOG_MODE, GL_LINEAR);		// Fog Mode
	glFogfv(GL_FOG_COLOR, fogColor);			// Set Fog Color
	glFogf(GL_FOG_DENSITY, 0.01f);				// How Dense Will The Fog Be
	glHint(GL_FOG_HINT, GL_DONT_CARE);			// Fog Hint Value
	glFogf(GL_FOG_START, 1.0f);				// Fog Start Depth
	glFogf(GL_FOG_END, 500.0f);				// Fog End Depth
	glEnable(GL_FOG);	
	
	// set the light posision
	glLightfv(GL_LIGHT0, GL_POSITION, light1Position);

	glLightfv(GL_LIGHT1, GL_POSITION, light2Position);


	char str[20];
	sprintf(str, "%d %d %d %d",extremePoints[0][0],extremePoints[0][1],extremePoints[1][0],extremePoints[1][1]);
	//TextOut(hDC,0,0,str,strlen(str));
	float z=0;
         
         
	float xtrans = -xpos;
    float ztrans = -zpos;
    float ytrans = -walkbias-0.25f-ypos;
    float sceneroty = 360.0f - yrot;
      
    glRotatef(lookupdown,1.0f,0,0);
    glRotatef(sceneroty,0,1.0f,0);
    glTranslatef(xtrans, ytrans, ztrans);
	

	// Floor texture

	glBindTexture(GL_TEXTURE_2D, texture[4]);				

	if(extremePoints[1][0]!=10000 || extremePoints[1][1]!=10000 || extremePoints[0][0]!=-10000 || extremePoints[0][1]!=-10000){
		extremePoints[0][0]=(extremePoints[0][0]/10)*10;
		extremePoints[0][1]=(extremePoints[0][1]/10)*10;
		extremePoints[1][0]=(extremePoints[1][0]/10)*10;
		extremePoints[1][1]=(extremePoints[1][1]/10)*10;

		int xunits = (extremePoints[1][0]-extremePoints[0][0])/10;
		int yunits = (extremePoints[1][1]-extremePoints[0][1])/10;
		glBegin(GL_QUADS);

		for(int i=0;i<xunits;i++){
			for(int j=0;j<yunits;j++){
				glTexCoord2f(0.0f, 0.0f);glVertex3f(extremePoints[0][0]+i*10*xunits, 0, extremePoints[0][1]+j*10*xunits);
				glTexCoord2f(1.0f, 0.0f);glVertex3f(extremePoints[0][0]+(i+1)*10*xunits, 0, extremePoints[0][1]+j*10*xunits);
				glTexCoord2f(1.0f, 1.0f);glVertex3f(extremePoints[0][0]+(i+1)*10*xunits, 0, extremePoints[0][1]+(j+1)*10*xunits);
				glTexCoord2f(0.0f, 1.0f);glVertex3f(extremePoints[0][0]+i*10*xunits, 0, extremePoints[0][1]+(j+1)*10*xunits);
			}
		}
		glEnd();
	}
  

	for(int i=0;i<noOfWalls;i++)
	{
		for(int k = 0;k<4;k++)
		{
			int x0 = wallListx[i][k];
			int y0 = wallListy[i][k];
			int x1 = wallListx[i][(k+1)%4];
			int y1 = wallListy[i][(k+1)%4];
			glBindTexture(GL_TEXTURE_2D, texture[wallListTex[i]]);
			glBegin(GL_QUADS);

			int wallHeight = 10;
			float hypotenuse = (float)hypot((x1-x0),(y1-y0));
			float cosComp = wallHeight* (x1-x0)/hypotenuse; 
			float sinComp = wallHeight* (y1-y0)/hypotenuse; 
			float j;
			for(j=0.0f;j+1.0f<hypotenuse/wallHeight;j+=1.0f)
			{
				glTexCoord2f(0.0f, 0.0f);glVertex3f(x0+(float)j*cosComp, 0, y0+(float)j*sinComp);
				glTexCoord2f(1.0f, 0.0f);glVertex3f(x0+(float)(j+1.0f)*cosComp, 0, y0+(float)(j+1.0f)*sinComp);
				glTexCoord2f(1.0f, 1.0f);glVertex3f(x0+(float)(j+1.0f)*cosComp, wallHeight, y0+(float)(j+1.0f)*sinComp);
				glTexCoord2f(0.0f, 1.0f);glVertex3f(x0+(float)j*cosComp, wallHeight, y0+(float)j*sinComp);
				if(k==0)
				{
					glTexCoord2f(0.0f, 0.0f);glVertex3f(x0+(float)j*cosComp, wallHeight, y0+(float)j*sinComp);
					glTexCoord2f(1.0f, 0.0f);glVertex3f(x0+(float)(j+1.0f)*cosComp, wallHeight, y0+(float)(j+1.0f)*sinComp);
					glTexCoord2f(1.0f, 1.0f);glVertex3f(wallListx[i][3]+(float)(j+1.0f)*cosComp, wallHeight, wallListy[i][3]+(float)(j+1.0f)*sinComp);
					glTexCoord2f(0.0f, 1.0f);glVertex3f(wallListx[i][3]+(float)j*cosComp, wallHeight, wallListy[i][3]+(float)j*sinComp);
				}
			}

			glTexCoord2f(0.0f, 0.0f);glVertex3f((float)x0+(float)j*cosComp, 0, (float)y0+(float)j*sinComp);
			glTexCoord2f(hypotenuse/wallHeight-j, 0.0f);glVertex3f(x1, 0, y1);
			glTexCoord2f(hypotenuse/wallHeight-j, 1.0f);glVertex3f(x1, wallHeight, y1);
			glTexCoord2f(0.0f, 1.0f);glVertex3f((float)x0+(float)j*cosComp, wallHeight, (float)y0+(float)j*sinComp);
			if(k==0)
			{
				glTexCoord2f(0.0f, 0.0f);glVertex3f((float)x0+(float)j*cosComp, wallHeight, (float)y0+(float)j*sinComp);
				glTexCoord2f(hypotenuse/wallHeight-j, 0.0f);glVertex3f(x1, wallHeight, y1);
				glTexCoord2f(hypotenuse/wallHeight-j, 1.0f);glVertex3f(wallListx[i][2], wallHeight, wallListy[i][2]);
				glTexCoord2f(0.0f, 1.0f);glVertex3f((float)wallListx[i][3]+(float)j*cosComp, wallHeight, (float)wallListy[i][3]+(float)j*sinComp);
			}
			glEnd();
			char str[20];
			sprintf(str,"%d %d %d %d",x0,y0,x1,y1);
		}

  
	}

	for(i=0;i<noOfChairs;i++)
	{
		if(chairDir[i]==0)
			room_chair[i].Rotate(0.0f,90.0f,0.0f);
		else if(chairDir[i]==1)
			room_chair[i].Rotate(0.0f,0.0f,0.0f);
		else if(chairDir[i]==2)
			room_chair[i].Rotate(0.0f,270.0f,0.0f);
		else if(chairDir[i]==3)
			room_chair[i].Rotate(0.0f,180.0f,0.0f);
		room_chair[i].Render();
	}

	for(i=0;i<noOfTables;i++)
	{
		if(tableDir[i]==0)
			room_table[i].Rotate(0.0f,90.0f,0.0f);
		else if(tableDir[i]==1)
			room_table[i].Rotate(0.0f,0.0f,0.0f);
		else if(tableDir[i]==2)
			room_table[i].Rotate(0.0f,270.0f,0.0f);
		else if(tableDir[i]==3)
			room_table[i].Rotate(0.0f,180.0f,0.0f);
		room_table[i].Render();
	}

	for(i=0;i<noOfDoors;i++)
	{
		if(doorDir[i]==0)
			room_door[i].Rotate(0.0f,270.0f,0.0f);
		else if(doorDir[i]==1)
			room_door[i].Rotate(0.0f,180.0f,0.0f);
		else if(doorDir[i]==2)
			room_door[i].Rotate(0.0f,90.0f,0.0f);
		else if(doorDir[i]==3)
			room_door[i].Rotate(0.0f,0.0f,0.0f);
		room_door[i].Render();
	}



	return TRUE;										// Everything Went OK
}

GLvoid KillGLWindow(GLvoid)								// Properly Kill The Window
{
	if (fullscreen)										// Are We In Fullscreen Mode?
	{
		ChangeDisplaySettings(NULL,0);					// If So Switch Back To The Desktop
		ShowCursor(TRUE);								// Show Mouse Pointer
	}

	if (hRC)											// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL,NULL))					// Are We Able To Release The DC And RC Contexts?
		{
			MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))						// Are We Able To Delete The RC?
		{
			MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}
		hRC=NULL;										// Set RC To NULL
	}

	if (hDC && !ReleaseDC(hWnd,hDC))					// Are We Able To Release The DC
	{
		MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hDC=NULL;										// Set DC To NULL
	}

	if (hWnd && !DestroyWindow(hWnd))					// Are We Able To Destroy The Window?
	{
		MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;										// Set hWnd To NULL
	}

	if (!UnregisterClass("OpenGL",hInstance))			// Are We Able To Unregister Class
	{
		MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;									// Set hInstance To NULL
	}
}

/*	This Code Creates Our OpenGL Window.  Parameters Are:					*
 *	title			- Title To Appear At The Top Of The Window				*
 *	width			- Width Of The GL Window Or Fullscreen Mode				*
 *	height			- Height Of The GL Window Or Fullscreen Mode			*
 *	bits			- Number Of Bits To Use For Color (8/16/24/32)			*
 *	fullscreenflag	- Use Fullscreen Mode (TRUE) Or Windowed Mode (FALSE)	*/
 
BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag)
{
	GLuint		PixelFormat;			// Holds The Results After Searching For A Match
	WNDCLASS	wc;						// Windows Class Structure
	DWORD		dwExStyle;				// Window Extended Style
	DWORD		dwStyle;				// Window Style
	RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left=(long)0;			// Set Left Value To 0
	WindowRect.right=(long)width;		// Set Right Value To Requested Width
	WindowRect.top=(long)0;				// Set Top Value To 0
	WindowRect.bottom=(long)height;		// Set Bottom Value To Requested Height

	fullscreen=fullscreenflag;			// Set The Global Fullscreen Flag

	hInstance			= GetModuleHandle(NULL);				// Grab An Instance For Our Window
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc		= (WNDPROC) WndProc;					// WndProc Handles Messages
	wc.cbClsExtra		= 0;									// No Extra Window Data
	wc.cbWndExtra		= 0;									// No Extra Window Data
	wc.hInstance		= hInstance;							// Set The Instance
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground	= NULL;									// No Background Required For GL
	wc.lpszMenuName		= NULL;									// We Don't Want A Menu
	wc.lpszClassName	= "OpenGL";								// Set The Class Name

	if (!RegisterClass(&wc))									// Attempt To Register The Window Class
	{
		MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;											// Return FALSE
	}
	
	if (fullscreen)												// Attempt Fullscreen Mode?
	{
		DEVMODE dmScreenSettings;								// Device Mode
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	// Makes Sure Memory's Cleared
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);		// Size Of The Devmode Structure
		dmScreenSettings.dmPelsWidth	= width;				// Selected Screen Width
		dmScreenSettings.dmPelsHeight	= height;				// Selected Screen Height
		dmScreenSettings.dmBitsPerPel	= bits;					// Selected Bits Per Pixel
		dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

		// Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
		if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
		{
			// If The Mode Fails, Offer Two Options.  Quit Or Use Windowed Mode.
			if (MessageBox(NULL,"The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?","NeHe GL",MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
			{
				fullscreen=FALSE;		// Windowed Mode Selected.  Fullscreen = FALSE
			}
			else
			{
				// Pop Up A Message Box Letting User Know The Program Is Closing.
				MessageBox(NULL,"Program Will Now Close.","ERROR",MB_OK|MB_ICONSTOP);
				return FALSE;									// Return FALSE
			}
		}
	}

	if (fullscreen)												// Are We Still In Fullscreen Mode?
	{
		dwExStyle=WS_EX_APPWINDOW;								// Window Extended Style
		dwStyle=WS_POPUP;										// Windows Style
		ShowCursor(FALSE);										// Hide Mouse Pointer
	}
	else
	{
		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
		dwStyle=WS_OVERLAPPEDWINDOW;							// Windows Style
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

	// Create The Window
	if (!(hWnd=CreateWindowEx(	dwExStyle,							// Extended Style For The Window
								"OpenGL",							// Class Name
								title,								// Window Title
								dwStyle |							// Defined Window Style
								WS_CLIPSIBLINGS |					// Required Window Style
								WS_CLIPCHILDREN,					// Required Window Style
								375, 0,								// Window Position
								WindowRect.right-WindowRect.left,	// Calculate Window Width
								WindowRect.bottom-WindowRect.top,	// Calculate Window Height
								NULL,								// No Parent Window
								NULL,								// No Menu
								hInstance,							// Instance
								NULL)))								// Dont Pass Anything To WM_CREATE
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	static	PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		bits,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// 16Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};
	
	if (!(hDC=GetDC(hWnd)))							// Did We Get A Device Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))	// Did Windows Find A Matching Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if(!SetPixelFormat(hDC,PixelFormat,&pfd))		// Are We Able To Set The Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(hRC=wglCreateContext(hDC)))				// Are We Able To Get A Rendering Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if(!wglMakeCurrent(hDC,hRC))					// Try To Activate The Rendering Context
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	ShowWindow(hWnd,SW_SHOW);						// Show The Window
	SetForegroundWindow(hWnd);						// Slightly Higher Priority
	SetFocus(hWnd);									// Sets Keyboard Focus To The Window
	ReSizeGLScene(width, height);					// Set Up Our Perspective GL Screen

	if (!InitGL())									// Initialize Our Newly Created GL Window
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Initialization Failed.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	return TRUE;									// Success
}



void line(HDC _hdc,int x1,int y1,int x2,int y2)//This function draws line by the given four coordinates.


    {
    	MoveToEx(_hdc,x1,y1,NULL);
    	LineTo(_hdc,x2,y2);
}



// Function that creates a Win32 toolbar.

// Parameters:

//    HWND hWndParent - Handle to the parent window.

//    LONG lNumBitmaps - Number of bitmaps for the toolbar.

//    TBADDBITMAP tbBItmaps - Bitmap structure for Win32.

//    LONG lNumButtons - Number of buttons on the toolbar.

//    TBBUTON tbButtons - Pointer to button structure.

//    DWORD dwStyle - Special toolbar window styles. Can be a combination

//       of generic common control styles or specific toolbar styles.

//    int iToolBarID - ID for the toolbar window.

// Returns:

//    Handle to the toolbar window if successful; otherwise NULL.

//

HWND MyCreateToolBar ( HWND hWndParent, LONG lNumBitmaps, TBADDBITMAP tbBitmaps, LONG lNumButtons, TBBUTTON tbButtons, WORD dwStyle, UINT iToolBarID )

{

  HWND hWndToolBar;



  // Create the toolbar control.

  hWndToolBar = CreateWindowEx( 0L,   // No extended styles.

    TOOLBARCLASSNAME,                 // Class name for the toolbar.

    "",                               // No default text.

    WS_CHILD | WS_BORDER | WS_VISIBLE | dwStyle,    // Styles and defaults.

    0, 0, 100, 30,                    // Standard toolbar size and position.

    hWndParent,                       // Parent window of the toolbar.

    (HMENU)iToolBarID,                // Toolbar ID.

    tbBitmaps.hInst,                 // Current instance.

    NULL );                           // No class data.



  if (hWndToolBar)

  {

    // Load the bitmaps for the toolbar.

    if (SendMessage(hWndToolBar, TB_ADDBITMAP, lNumBitmaps, 

            (LONG) &tbBitmaps) == -1)  

        return (NULL);

    // Add the buttons to the toolbar.

    SendMessage(hWndToolBar, TB_ADDBUTTONS, lNumButtons, (LONG) &tbButtons);

    // The toolbar was created with the buttons successfully.

    // Return the handle to the toolbar.

    return (hWndToolBar);

  }

  // There was an error creating the toolbar; return NULL.

  return NULL;

}


               
void drawWall(int x1,int y1,int x2,int y2, float wt,HDC hdc)
{
	//SetDCPenColor(hdc, 
	double theta = atan((double)(y2-y1)/(double)(x2-x1));
	MoveToEx(hdc, x1+(int)((wt/2.0f)*sin(theta)),y1-(int)((wt/2.0f)*cos(theta)), NULL);
	LineTo(hdc, x2+(int)((wt/2.0f)*sin(theta)),y2-(int)((wt/2.0f)*cos(theta)));

	MoveToEx(hdc, x2-(int)((wt/2.0f)*sin(theta)),y2+(int)((wt/2.0f)*cos(theta)), NULL);
	LineTo(hdc, x2+(int)((wt/2.0f)*sin(theta)),y2-(int)((wt/2.0f)*cos(theta)));

	MoveToEx(hdc, x1-(int)((wt/2.0f)*sin(theta)),y1+(int)((wt/2.0f)*cos(theta)), NULL);
	LineTo(hdc, x2-(int)((wt/2.0f)*sin(theta)),y2+(int)((wt/2.0f)*cos(theta)));

	MoveToEx(hdc, x1-(int)((wt/2.0f)*sin(theta)),y1+(int)((wt/2.0f)*cos(theta)), NULL);
	LineTo(hdc, x1+(int)((wt/2.0f)*sin(theta)),y1-(int)((wt/2.0f)*cos(theta)));

}


void drawChair(int x1,int y1, float size,HDC hdc)
{
	MoveToEx(hdc, x1-size/2,y1-size/2, NULL);
	LineTo(hdc, x1-size/2,y1+size/2);

	MoveToEx(hdc, x1-size/2,y1+size/2, NULL);
	LineTo(hdc, x1+size/2,y1+size/2);

	MoveToEx(hdc, x1+size/2,y1+size/2, NULL);
	LineTo(hdc, x1+size/2,y1-size/2);

	MoveToEx(hdc, x1+size/2,y1-size/2, NULL);
	LineTo(hdc, x1-size/2,y1-size/2);


}


void drawTable(int x1,int y1, float size, int horiz, HDC hdc)
{
	if(horiz == 0){
		MoveToEx(hdc, x1-2*size/2,y1-size/2, NULL);
		LineTo(hdc, x1-2*size/2,y1+size/2);

		MoveToEx(hdc, x1-2*size/2,y1+size/2, NULL);
		LineTo(hdc, x1+2*size/2,y1+size/2);

		MoveToEx(hdc, x1+2*size/2,y1+size/2, NULL);
		LineTo(hdc, x1+2*size/2,y1-size/2);

		MoveToEx(hdc, x1+2*size/2,y1-size/2, NULL);
		LineTo(hdc, x1-2*size/2,y1-size/2);
	}
	else{
		MoveToEx(hdc, x1-size/2,y1-2*size/2, NULL);
		LineTo(hdc, x1-size/2,y1+2*size/2);

		MoveToEx(hdc, x1-size/2,y1+2*size/2, NULL);
		LineTo(hdc, x1+size/2,y1+2*size/2);

		MoveToEx(hdc, x1+size/2,y1+2*size/2, NULL);
		LineTo(hdc, x1+size/2,y1-2*size/2);

		MoveToEx(hdc, x1+size/2,y1-2*size/2, NULL);
		LineTo(hdc, x1-size/2,y1-2*size/2);
	}

}


void drawDoor(int x1,int y1, float size,HDC hdc)
{
	MoveToEx(hdc, x1-size/2,y1-size/2, NULL);
	LineTo(hdc, x1-size/2,y1+size/2);

	MoveToEx(hdc, x1-size/2,y1+size/2, NULL);
	LineTo(hdc, x1+size/2,y1+size/2);

	MoveToEx(hdc, x1+size/2,y1+size/2, NULL);
	LineTo(hdc, x1+size/2,y1-size/2);

	MoveToEx(hdc, x1+size/2,y1-size/2, NULL);
	LineTo(hdc, x1-size/2,y1-size/2);


}


int costVal(char* str)
{
	int ret = 0;
	for(int i=0;str[i]!='\0';i++)
	{
		if(str[i]<'0' || str[i]>'9')
			return 0;
		ret = ret*10 + (int)(str[i]-'0');
	}
	return ret;
}




LRESULT CALLBACK WndProc(	HWND	hWnd,			// Handle For This Window
							UINT	uMsg,			// Message For This Window
							WPARAM	wParam,			// Additional Message Information
							LPARAM	lParam)			// Additional Message Information
{

	static BOOL IsDrawing = FALSE;
	static int StartX, StartY;
	static int EndX, EndY;
	
	switch (uMsg)									// Check For Windows Messages
	{


			HDC hdc ;
			PAINTSTRUCT ps ;
			RECT rect ;

			{

				case WM_LBUTTONDOWN:
					if( wallClicked)
					{
						char coststr[50];
						GetWindowText(edit_wall,coststr,strlen(coststr));
						if(costVal(coststr)!=0)
						{
							hdc = GetDC(hWnd);
							StartX = LOWORD(lParam);
							StartY = HIWORD(lParam);
						
							EndX = LOWORD(lParam);
							EndY = HIWORD(lParam);
						
							SetROP2(hdc, R2_XORPEN);

							drawWall(StartX, StartY, EndX, EndY, 4, hdc);
							IsDrawing = TRUE;

							ReleaseDC(hWnd, hdc);
						}
						else
							MessageBox(NULL, "Cost/unit value is incorrect", "Error", 0);
					}

					if(chairClicked)
					{
						char coststr[50];
						GetWindowText(edit_chair,coststr,strlen(coststr));
						if(costVal(coststr)!=0)
						{
							hdc = GetDC(hWnd);
							StartX = LOWORD(lParam);
							StartY = HIWORD(lParam);
						
							SetROP2(hdc, R2_BLACK);

							drawChair(StartX, StartY, 6, hdc);

							char str[20];
							sprintf(str, "%d %d",StartX, StartY);
							//TextOut(hdc,0,0,str,strlen(str));

							if(StartX>extremePoints[1][0])
								extremePoints[1][0]=StartX;
							if(StartX<extremePoints[0][0])
								extremePoints[0][0]=StartX;
							if(StartY>extremePoints[1][1])
								extremePoints[1][1]=StartY;
							if(StartY<extremePoints[0][1])
								extremePoints[0][1]=StartY;


							if(chairTex[noOfChairs]==0)
								room_chair[noOfChairs].Initialize("Data/3ds/room_chair_1.3DS");//load the file into this instance
							else if(chairTex[noOfChairs]==1)
								room_chair[noOfChairs].Initialize("Data/3ds/room_chair_2.3DS");//load the file into this instance
							else if(chairTex[noOfChairs]==2)
								room_chair[noOfChairs].Initialize("Data/3ds/room_chair_3.3DS");//load the file into this instance
							
							room_chair[noOfChairs].SetScalar(0.0003f); //scale the model down to 5-hundredths its natural size
							room_chair[noOfChairs].SetPosition(StartX, 0.0f, StartY);//position the model at (10, 0, 0)

	/*						char str[10];
							sprintf(str,"%d,%d",StartX,StartY);
							MessageBox(NULL,str,"",0);
	*/						
							chairCost[noOfChairs] = costVal(coststr);
							noOfChairs++;

							ReleaseDC(hWnd, hdc);
						}
						else
							MessageBox(NULL, "Cost value is incorrect", "Error", 0);

					}

					if(tableClicked)
					{
						char coststr[50];
						GetWindowText(edit_table,coststr,strlen(coststr));
						if(costVal(coststr)!=0)
						{
							hdc = GetDC(hWnd);
							StartX = LOWORD(lParam);
							StartY = HIWORD(lParam);
						
							SetROP2(hdc, R2_BLACK);

							drawTable(StartX, StartY, 4, tableDir[noOfTables]%2, hdc);

							if(StartX>extremePoints[1][0])
								extremePoints[1][0]=StartX;
							if(StartX<extremePoints[0][0])
								extremePoints[0][0]=StartX;
							if(StartY>extremePoints[1][1])
								extremePoints[1][1]=StartY;
							if(StartY<extremePoints[0][1])
								extremePoints[0][1]=StartY;


							if(tableTex[noOfTables]==0)
								room_table[noOfTables].Initialize("Data/3ds/room_table_1.3DS");//load the file into this instance
							else if(tableTex[noOfTables]==1)
								room_table[noOfTables].Initialize("Data/3ds/room_table_2.3DS");//load the file into this instance
							else if(tableTex[noOfTables]==2)
								room_table[noOfTables].Initialize("Data/3ds/room_table_3.3DS");//load the file into this instance
							
							room_table[noOfTables].SetScalar(0.00001f); 
							room_table[noOfTables].SetPosition(StartX, 0.0f, StartY);

							tableCost[noOfTables] = costVal(coststr);
							noOfTables++;

							ReleaseDC(hWnd, hdc);
						}
						else
							MessageBox(NULL, "Cost value is incorrect", "Error", 0);

					}

					if(doorClicked)
					{
						char coststr[50];
						GetWindowText(edit_door,coststr,strlen(coststr));
						if(costVal(coststr)!=0)
						{
							hdc = GetDC(hWnd);
							StartX = LOWORD(lParam);
							StartY = HIWORD(lParam);
						
							SetROP2(hdc, R2_BLACK);

							drawDoor(StartX, StartY, 6, hdc);

							char str[20];
							sprintf(str, "%d %d",StartX, StartY);
							//TextOut(hdc,0,0,str,strlen(str));

							/*if(StartX>extremePoints[1][0])
								extremePoints[1][0]=StartX;
							if(StartX<extremePoints[0][0])
								extremePoints[0][0]=StartX;
							if(StartY>extremePoints[1][1])
								extremePoints[1][1]=StartY;
							if(StartY<extremePoints[0][1])
								extremePoints[0][1]=StartY;*/


							if(doorTex[noOfDoors]==0)
								room_door[noOfDoors].Initialize("Data/3ds/room_door_1.3DS");//load the file into this instance
							else if(doorTex[noOfDoors]==1)
								room_door[noOfDoors].Initialize("Data/3ds/room_door_2.3DS");//load the file into this instance
							else if(doorTex[noOfDoors]==2)
								room_door[noOfDoors].Initialize("Data/3ds/room_door_3.3DS");//load the file into this instance
							
							room_door[noOfDoors].SetScalar(0.00000015f); //scale the model down to 5-hundredths its natural size
							room_door[noOfDoors].SetPosition(StartX, 7.0f, StartY);//position the model at (10, 0, 0)

							doorCost[noOfDoors] = costVal(coststr);

							//char str2[10];
							//sprintf(str2,"%d",chairCost[coOf);
							//MessageBox(NULL,str2,"",0);
	
							noOfDoors++;

							ReleaseDC(hWnd, hdc);
						}
						else
							MessageBox(NULL, "Cost value is incorrect", "Error", 0);

					}

					return 0;

				case WM_MOUSEMOVE:
					{
					hdc = GetDC(hWnd);
					if( IsDrawing == TRUE && wallClicked)
					{
						SetROP2(hdc, R2_NOTXORPEN);
					
						drawWall(StartX, StartY, EndX, EndY, 4, hdc);
						EndX = LOWORD(lParam);
						EndY = HIWORD(lParam);
					
						drawWall(StartX, StartY, EndX, EndY, 4, hdc);
					}

					if(chairClicked)
					{
						SetROP2(hdc, R2_XORPEN);

						//drawChair(StartX, StartY, 10, hdc);
						StartX = LOWORD(lParam);
						StartY = HIWORD(lParam);
					
						drawChair(StartX, StartY, 6, hdc);

						ReleaseDC(hWnd, hdc);
					}

					if(tableClicked)
					{
						SetROP2(hdc, R2_XORPEN);

						//drawChair(StartX, StartY, 10, hdc);
						StartX = LOWORD(lParam);
						StartY = HIWORD(lParam);
					
						drawTable(StartX, StartY, 4, tableDir[noOfTables]%2, hdc);

						ReleaseDC(hWnd, hdc);
					}

					if(doorClicked)
					{
						SetROP2(hdc, R2_XORPEN);

						//drawChair(StartX, StartY, 10, hdc);
						StartX = LOWORD(lParam);
						StartY = HIWORD(lParam);
					
						drawChair(StartX, StartY, 6, hdc);

						ReleaseDC(hWnd, hdc);
					}


					ReleaseDC(hWnd, hdc);
					break;
					}
				case WM_LBUTTONUP:
					if(wallClicked)
					{
					hdc = GetDC(hWnd);

					EndX = LOWORD(lParam);
					EndY = HIWORD(lParam);

					SetROP2(hdc, R2_XORPEN);
					
					drawWall(StartX, StartY, EndX, EndY, 4, hdc);
					IsDrawing = FALSE;


						if(StartX>extremePoints[1][0])
							extremePoints[1][0]=StartX;
						if(StartX<extremePoints[0][0])
							extremePoints[0][0]=StartX;
						if(StartY>extremePoints[1][1])
							extremePoints[1][1]=StartY;
						if(StartY<extremePoints[0][1])
							extremePoints[0][1]=StartY;

						if(EndX>extremePoints[1][0])
							extremePoints[1][0]=EndX;
						if(EndX<extremePoints[0][0])
							extremePoints[0][0]=EndX;
						if(EndY>extremePoints[1][1])
							extremePoints[1][1]=EndY;
						if(EndY<extremePoints[0][1])
							extremePoints[0][1]=EndY;

						int wt = 4;
						int x1 = StartX, y1 = StartY, x2 = EndX, y2 = EndY;
						double theta = atan((double)(y2-y1)/(double)(x2-x1));

						wallListx[noOfWalls][0] =  x1+(int)((wt/2.0f)*sin(theta));
						wallListy[noOfWalls][0] = y1-(int)((wt/2.0f)*cos(theta));
	
						wallListx[noOfWalls][1] =  x2+(int)((wt/2.0f)*sin(theta));
						wallListy[noOfWalls][1] = y2-(int)((wt/2.0f)*cos(theta));
	
						wallListx[noOfWalls][2] =  x2-(int)((wt/2.0f)*sin(theta));
						wallListy[noOfWalls][2] = y2+(int)((wt/2.0f)*cos(theta));
	
						wallListx[noOfWalls][3] =  x1-(int)((wt/2.0f)*sin(theta));
						wallListy[noOfWalls][3] = y1+(int)((wt/2.0f)*cos(theta));

						char str[50];
						GetWindowText(edit_wall,str,strlen(str));
						wallCost[noOfWalls] = (int)((double)costVal(str)*(hypot(StartX-EndX,StartY-EndY)));

						noOfWalls++;	

					ReleaseDC(hWnd, hdc);

					break;
					}
				case WM_COMMAND:

					if(((HWND)lParam) && (HIWORD(wParam) == BN_CLICKED)){
						int iMID;
						iMID = LOWORD(wParam);
						switch(iMID)
						{
							case IDB_WALL_BRICK:
								wallListTex[noOfWalls] = 0;
								break;
						  
							case IDB_WALL_MOSSY:
								wallListTex[noOfWalls] = 1;
								break;
						  
							case IDB_WALL_ROCKY:
								wallListTex[noOfWalls] = 2;
								break;
						  
							case IDB_WALL_STONY:
								wallListTex[noOfWalls] = 3;
								break;


///////////////////////////////////CHAIR//////////////////////////////////////////

							case IDB_D_B_CHAIR:
								chairTex[noOfChairs] = 0;
								break;
						  
							case IDB_L_B_CHAIR:
								chairTex[noOfChairs] = 1;
								break;
						  
							case IDB_RUSTY_CHAIR:
								chairTex[noOfChairs] = 2;
								break;
						  

							case IDB_TOP_CHAIR:
								chairDir[noOfChairs] = 0;
								break;
						  
							case IDB_RIGHT_CHAIR:
								chairDir[noOfChairs] = 1;
								break;
						  
							case IDB_DOWN_CHAIR:
								chairDir[noOfChairs] = 2;
								break;
						  
							case IDB_LEFT_CHAIR:
								chairDir[noOfChairs] = 3;
								break;
						  
///////////////////////////////////TABLE//////////////////////////////////////////

							case IDB_D_B_TABLE:
								tableTex[noOfTables] = 0;
								break;
						  
							case IDB_L_B_TABLE:
								tableTex[noOfTables] = 1;
								break;
						  
							case IDB_RUSTY_TABLE:
								tableTex[noOfTables] = 2;
								break;
						  

							case IDB_TOP_TABLE:
								tableDir[noOfTables] = 0;
								break;
						  
							case IDB_RIGHT_TABLE:
								tableDir[noOfTables] = 1;
								break;
						  
							case IDB_DOWN_TABLE:
								tableDir[noOfTables] = 2;
								break;
						  
							case IDB_LEFT_TABLE:
								tableDir[noOfTables] = 3;
								break;

///////////////////////////////////DOOR//////////////////////////////////////////

							case IDB_1_DOOR:
								doorTex[noOfDoors] = 0;
								break;
						  
							case IDB_2_DOOR:
								doorTex[noOfDoors] = 1;
								break;
						  
							case IDB_3_DOOR:
								doorTex[noOfDoors] = 2;
								break;
						  
						
							case IDB_TOP_DOOR:
								doorDir[noOfDoors] = 0;
								break;
						  
							case IDB_RIGHT_DOOR:
								doorDir[noOfDoors] = 1;
								break;
						  
							case IDB_DOWN_DOOR:
								doorDir[noOfDoors] = 2;
								break;
						  
							case IDB_LEFT_DOOR:
								doorDir[noOfDoors] = 3;
								break;

						
						}
					}

					switch(LOWORD(wParam))
					{
						case IDM_DRAW_WALL:
							wallClicked = TRUE;
							chairClicked = FALSE;
							tableClicked = FALSE;
							doorClicked = FALSE;
							//DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_WALL_TEXTURES), hWnd, WallTexturesDlgProc);
							//DialogBox(hInstance, MAKEINTRESOURCE(IDD_WALL_TEXTURES),
								// hWnd, reinterpret_cast<DLGPROC>(WallTexturesDlgProc));

							DestroyWindow(button_dark_brown_chair);
							DestroyWindow(button_light_brown_chair);
							DestroyWindow(button_rusty_chair);

							DestroyWindow(button_top_chair);
							DestroyWindow(button_right_chair);
							DestroyWindow(button_down_chair);
							DestroyWindow(button_left_chair);
							
							DestroyWindow(button_dark_brown_table);
							DestroyWindow(button_light_brown_table);
							DestroyWindow(button_rusty_table);

							DestroyWindow(button_top_table);
							DestroyWindow(button_right_table);
							DestroyWindow(button_down_table);
							DestroyWindow(button_left_table);
							
							DestroyWindow(button_1_door);
							DestroyWindow(button_2_door);
							DestroyWindow(button_3_door);

							DestroyWindow(button_top_door);
							DestroyWindow(button_right_door);
							DestroyWindow(button_down_door);
							DestroyWindow(button_left_door);
							
							DestroyWindow(edit_chair);
							DestroyWindow(edit_table);
							DestroyWindow(edit_door);
							
							DestroyWindow(label_chair);
							DestroyWindow(label_table);
							DestroyWindow(label_door);

							button_brick_wall = CreateWindow(
           "BUTTON", /* this makes a "button" */
           "Brick Wall", /* this is the text which will appear in the button */
           WS_VISIBLE | WS_CHILD,
           5, /* these four lines are the position and dimensions of the button */
           60,
           105,
           20,
           hWnd_Options, /* this is the buttons parent window */
           (HMENU)IDB_WALL_BRICK, /* these next two lines pretty much tell windows what to do when the button is pressed */
           hInstance,
           NULL);

							button_mossy_wall = CreateWindow(
           "BUTTON", /* this makes a "button" */
           "Mossy Wall", /* this is the text which will appear in the button */
           WS_VISIBLE | WS_CHILD,
           120, /* these four lines are the position and dimensions of the button */
           60,
           105,
           20,
           hWnd_Options, /* this is the buttons parent window */
           (HMENU)IDB_WALL_MOSSY, /* these next two lines pretty much tell windows what to do when the button is pressed */
           hInstance,
           NULL);

							button_rocky_wall = CreateWindow(
           "BUTTON", /* this makes a "button" */
           "Rocky Wall", /* this is the text which will appear in the button */
           WS_VISIBLE | WS_CHILD,
           335, /* these four lines are the position and dimensions of the button */
           60,
           105,
           20,
           hWnd_Options, /* this is the buttons parent window */
           (HMENU)IDB_WALL_ROCKY, /* these next two lines pretty much tell windows what to do when the button is pressed */
           hInstance,
           NULL);

							button_stony_wall = CreateWindow(
           "BUTTON", /* this makes a "button" */
           "Stone Wall", /* this is the text which will appear in the button */
           WS_VISIBLE | WS_CHILD,
           450, /* these four lines are the position and dimensions of the button */
           60,
           105,
           20,
           hWnd_Options, /* this is the buttons parent window */
           (HMENU)IDB_WALL_STONY, /* these next two lines pretty much tell windows what to do when the button is pressed */
           hInstance,
           NULL);

							label_wall = CreateWindow(
           "STATIC", /* this makes a "button" */
           "Cost/unit:", /* this is the text which will appear in the button */
           WS_VISIBLE | WS_CHILD,
           600, /* these four lines are the position and dimensions of the button */
           60,
           70,
           20,
           hWnd_Options, /* this is the buttons parent window */
           (HMENU)IDL_WALL, /* these next two lines pretty much tell windows what to do when the button is pressed */
           hInstance,
           NULL);

							edit_wall = CreateWindow(
           "EDIT", /* this makes a "button" */
           "50", /* this is the text which will appear in the button */
           WS_VISIBLE | WS_CHILD,
           670, /* these four lines are the position and dimensions of the button */
           60,
           105,
           20,
           hWnd_Options, /* this is the buttons parent window */
           (HMENU)IDE_WALL, /* these next two lines pretty much tell windows what to do when the button is pressed */
           hInstance,
           NULL);

						break;
						case IDM_DRAW_CHAIR:
							chairClicked = TRUE;
							wallClicked = FALSE;
							tableClicked = FALSE;
							doorClicked = FALSE;


							DestroyWindow(button_brick_wall);
							DestroyWindow(button_mossy_wall);
							DestroyWindow(button_rocky_wall);
							DestroyWindow(button_stony_wall);
							
							DestroyWindow(button_dark_brown_table);
							DestroyWindow(button_light_brown_table);
							DestroyWindow(button_rusty_table);

							DestroyWindow(button_top_table);
							DestroyWindow(button_right_table);
							DestroyWindow(button_down_table);
							DestroyWindow(button_left_table);

							DestroyWindow(button_1_door);
							DestroyWindow(button_2_door);
							DestroyWindow(button_3_door);

							DestroyWindow(button_top_door);
							DestroyWindow(button_right_door);
							DestroyWindow(button_down_door);
							DestroyWindow(button_left_door);

							DestroyWindow(edit_wall);
							DestroyWindow(edit_table);
							DestroyWindow(edit_door);
							
							DestroyWindow(label_wall);
							DestroyWindow(label_table);
							DestroyWindow(label_door);


														button_dark_brown_chair = CreateWindow(
           "BUTTON", /* this makes a "button" */
           "Dark Brown Chair", /* this is the text which will appear in the button */
           WS_VISIBLE | WS_CHILD,
           5, /* these four lines are the position and dimensions of the button */
           60,
           150,
           20,
           hWnd_Options, /* this is the buttons parent window */
           (HMENU)IDB_D_B_CHAIR, /* these next two lines pretty much tell windows what to do when the button is pressed */
           hInstance,
           NULL);

							button_light_brown_chair = CreateWindow(
           "BUTTON", /* this makes a "button" */
           "Light Brown Chair", /* this is the text which will appear in the button */
           WS_VISIBLE | WS_CHILD,
           160, /* these four lines are the position and dimensions of the button */
           60,
           150,
           20,
           hWnd_Options, /* this is the buttons parent window */
           (HMENU)IDB_L_B_CHAIR, /* these next two lines pretty much tell windows what to do when the button is pressed */
           hInstance,
           NULL);

							button_rusty_chair = CreateWindow(
           "BUTTON", /* this makes a "button" */
           "Rusty Chair", /* this is the text which will appear in the button */
           WS_VISIBLE | WS_CHILD,
           315, /* these four lines are the position and dimensions of the button */
           60,
           150,
           20,
           hWnd_Options, /* this is the buttons parent window */
           (HMENU)IDB_RUSTY_CHAIR, /* these next two lines pretty much tell windows what to do when the button is pressed */
           hInstance,
           NULL);



							button_top_chair = CreateWindow(
           "BUTTON", /* this makes a "button" */
           "Top", /* this is the text which will appear in the button */
           WS_VISIBLE | WS_CHILD,
           5, /* these four lines are the position and dimensions of the button */
           90,
           105,
           20,
           hWnd_Options, /* this is the buttons parent window */
           (HMENU)IDB_TOP_CHAIR, /* these next two lines pretty much tell windows what to do when the button is pressed */
           hInstance,
           NULL);

							button_right_chair = CreateWindow(
           "BUTTON", /* this makes a "button" */
           "Right", /* this is the text which will appear in the button */
           WS_VISIBLE | WS_CHILD,
           120, /* these four lines are the position and dimensions of the button */
           90,
           105,
           20,
           hWnd_Options, /* this is the buttons parent window */
           (HMENU)IDB_RIGHT_CHAIR, /* these next two lines pretty much tell windows what to do when the button is pressed */
           hInstance,
           NULL);

							button_down_chair = CreateWindow(
           "BUTTON", /* this makes a "button" */
           "Down", /* this is the text which will appear in the button */
           WS_VISIBLE | WS_CHILD,
           335, /* these four lines are the position and dimensions of the button */
           90,
           105,
           20,
           hWnd_Options, /* this is the buttons parent window */
           (HMENU)IDB_DOWN_CHAIR, /* these next two lines pretty much tell windows what to do when the button is pressed */
           hInstance,
           NULL);

							button_left_chair = CreateWindow(
           "BUTTON", /* this makes a "button" */
           "Left", /* this is the text which will appear in the button */
           WS_VISIBLE | WS_CHILD,
           450, /* these four lines are the position and dimensions of the button */
           90,
           105,
           20,
           hWnd_Options, /* this is the buttons parent window */
           (HMENU)IDB_LEFT_CHAIR, /* these next two lines pretty much tell windows what to do when the button is pressed */
           hInstance,
           NULL);

							label_chair = CreateWindow(
           "STATIC", /* this makes a "button" */
           "Cost:", /* this is the text which will appear in the button */
           WS_VISIBLE | WS_CHILD,
           600, /* these four lines are the position and dimensions of the button */
           60,
           70,
           20,
           hWnd_Options, /* this is the buttons parent window */
           (HMENU)IDL_CHAIR, /* these next two lines pretty much tell windows what to do when the button is pressed */
           hInstance,
           NULL);

							edit_chair = CreateWindow(
           "EDIT", /* this makes a "button" */
           "600", /* this is the text which will appear in the button */
           WS_VISIBLE | WS_CHILD,
           670, /* these four lines are the position and dimensions of the button */
           60,
           105,
           20,
           hWnd_Options, /* this is the buttons parent window */
           (HMENU)IDE_CHAIR, /* these next two lines pretty much tell windows what to do when the button is pressed */
           hInstance,
           NULL);





						break;


						case IDM_DRAW_TABLE:
							tableClicked = TRUE;
							wallClicked = FALSE;
							chairClicked = FALSE;
							doorClicked = FALSE;


							DestroyWindow(button_brick_wall);
							DestroyWindow(button_mossy_wall);
							DestroyWindow(button_rocky_wall);
							DestroyWindow(button_stony_wall);
							
							DestroyWindow(button_dark_brown_chair);
							DestroyWindow(button_light_brown_chair);
							DestroyWindow(button_rusty_chair);

							DestroyWindow(button_top_chair);
							DestroyWindow(button_right_chair);
							DestroyWindow(button_down_chair);
							DestroyWindow(button_left_chair);

							DestroyWindow(button_1_door);
							DestroyWindow(button_2_door);
							DestroyWindow(button_3_door);

							DestroyWindow(button_top_door);
							DestroyWindow(button_right_door);
							DestroyWindow(button_down_door);
							DestroyWindow(button_left_door);
							
							DestroyWindow(edit_wall);
							DestroyWindow(edit_chair);
							DestroyWindow(edit_door);
							
							DestroyWindow(label_wall);
							DestroyWindow(label_chair);
							DestroyWindow(label_door);



														button_dark_brown_table = CreateWindow(
           "BUTTON", /* this makes a "button" */
           "Dark Brown Table", /* this is the text which will appear in the button */
           WS_VISIBLE | WS_CHILD,
           5, /* these four lines are the position and dimensions of the button */
           60,
           150,
           20,
           hWnd_Options, /* this is the buttons parent window */
           (HMENU)IDB_D_B_TABLE, /* these next two lines pretty much tell windows what to do when the button is pressed */
           hInstance,
           NULL);

							button_light_brown_table = CreateWindow(
           "BUTTON", /* this makes a "button" */
           "Light Brown Table", /* this is the text which will appear in the button */
           WS_VISIBLE | WS_CHILD,
           160, /* these four lines are the position and dimensions of the button */
           60,
           150,
           20,
           hWnd_Options, /* this is the buttons parent window */
           (HMENU)IDB_L_B_TABLE, /* these next two lines pretty much tell windows what to do when the button is pressed */
           hInstance,
           NULL);

							button_rusty_table = CreateWindow(
           "BUTTON", /* this makes a "button" */
           "Rusty Table", /* this is the text which will appear in the button */
           WS_VISIBLE | WS_CHILD,
           315, /* these four lines are the position and dimensions of the button */
           60,
           150,
           20,
           hWnd_Options, /* this is the buttons parent window */
           (HMENU)IDB_RUSTY_TABLE, /* these next two lines pretty much tell windows what to do when the button is pressed */
           hInstance,
           NULL);



							button_top_table = CreateWindow(
           "BUTTON", /* this makes a "button" */
           "Top", /* this is the text which will appear in the button */
           WS_VISIBLE | WS_CHILD,
           5, /* these four lines are the position and dimensions of the button */
           90,
           105,
           20,
           hWnd_Options, /* this is the buttons parent window */
           (HMENU)IDB_TOP_TABLE, /* these next two lines pretty much tell windows what to do when the button is pressed */
           hInstance,
           NULL);

							button_right_table = CreateWindow(
           "BUTTON", /* this makes a "button" */
           "Right", /* this is the text which will appear in the button */
           WS_VISIBLE | WS_CHILD,
           120, /* these four lines are the position and dimensions of the button */
           90,
           105,
           20,
           hWnd_Options, /* this is the buttons parent window */
           (HMENU)IDB_RIGHT_TABLE, /* these next two lines pretty much tell windows what to do when the button is pressed */
           hInstance,
           NULL);

							button_down_table = CreateWindow(
           "BUTTON", /* this makes a "button" */
           "Down", /* this is the text which will appear in the button */
           WS_VISIBLE | WS_CHILD,
           335, /* these four lines are the position and dimensions of the button */
           90,
           105,
           20,
           hWnd_Options, /* this is the buttons parent window */
           (HMENU)IDB_DOWN_TABLE, /* these next two lines pretty much tell windows what to do when the button is pressed */
           hInstance,
           NULL);

							button_left_table = CreateWindow(
           "BUTTON", /* this makes a "button" */
           "Left", /* this is the text which will appear in the button */
           WS_VISIBLE | WS_CHILD,
           450, /* these four lines are the position and dimensions of the button */
           90,
           105,
           20,
           hWnd_Options, /* this is the buttons parent window */
           (HMENU)IDB_LEFT_TABLE, /* these next two lines pretty much tell windows what to do when the button is pressed */
           hInstance,
           NULL);


							label_table = CreateWindow(
           "STATIC", /* this makes a "button" */
           "Cost:", /* this is the text which will appear in the button */
           WS_VISIBLE | WS_CHILD,
           600, /* these four lines are the position and dimensions of the button */
           60,
           70,
           20,
           hWnd_Options, /* this is the buttons parent window */
           (HMENU)IDL_TABLE, /* these next two lines pretty much tell windows what to do when the button is pressed */
           hInstance,
           NULL);

							edit_table = CreateWindow(
           "EDIT", /* this makes a "button" */
           "700", /* this is the text which will appear in the button */
           WS_VISIBLE | WS_CHILD,
           670, /* these four lines are the position and dimensions of the button */
           60,
           105,
           20,
           hWnd_Options, /* this is the buttons parent window */
           (HMENU)IDE_TABLE, /* these next two lines pretty much tell windows what to do when the button is pressed */
           hInstance,
           NULL);




						break;

						case IDM_ADV_DOOR:
							doorClicked = TRUE;
							wallClicked = FALSE;
							chairClicked = FALSE;
							tableClicked = FALSE;


							DestroyWindow(button_brick_wall);
							DestroyWindow(button_mossy_wall);
							DestroyWindow(button_rocky_wall);
							DestroyWindow(button_stony_wall);
							
							DestroyWindow(button_dark_brown_chair);
							DestroyWindow(button_light_brown_chair);
							DestroyWindow(button_rusty_chair);

							DestroyWindow(button_top_chair);
							DestroyWindow(button_right_chair);
							DestroyWindow(button_down_chair);
							DestroyWindow(button_left_chair);

							DestroyWindow(button_dark_brown_table);
							DestroyWindow(button_light_brown_table);
							DestroyWindow(button_rusty_table);

							DestroyWindow(button_top_table);
							DestroyWindow(button_right_table);
							DestroyWindow(button_down_table);
							DestroyWindow(button_left_table);

							DestroyWindow(edit_wall);
							DestroyWindow(edit_chair);
							DestroyWindow(edit_table);
							
							DestroyWindow(label_wall);
							DestroyWindow(label_chair);
							DestroyWindow(label_table);


														button_1_door = CreateWindow(
           "BUTTON", /* this makes a "button" */
           "Door 1", /* this is the text which will appear in the button */
           WS_VISIBLE | WS_CHILD,
           5, /* these four lines are the position and dimensions of the button */
           60,
           150,
           20,
           hWnd_Options, /* this is the buttons parent window */
           (HMENU)IDB_1_DOOR, /* these next two lines pretty much tell windows what to do when the button is pressed */
           hInstance,
           NULL);

							button_2_door = CreateWindow(
           "BUTTON", /* this makes a "button" */
           "Door 2", /* this is the text which will appear in the button */
           WS_VISIBLE | WS_CHILD,
           160, /* these four lines are the position and dimensions of the button */
           60,
           150,
           20,
           hWnd_Options, /* this is the buttons parent window */
           (HMENU)IDB_2_DOOR, /* these next two lines pretty much tell windows what to do when the button is pressed */
           hInstance,
           NULL);

							button_3_door = CreateWindow(
           "BUTTON", /* this makes a "button" */
           "Door 3", /* this is the text which will appear in the button */
           WS_VISIBLE | WS_CHILD,
           315, /* these four lines are the position and dimensions of the button */
           60,
           150,
           20,
           hWnd_Options, /* this is the buttons parent window */
           (HMENU)IDB_3_DOOR, /* these next two lines pretty much tell windows what to do when the button is pressed */
           hInstance,
           NULL);


							button_top_door = CreateWindow(
           "BUTTON", /* this makes a "button" */
           "Top", /* this is the text which will appear in the button */
           WS_VISIBLE | WS_CHILD,
           5, /* these four lines are the position and dimensions of the button */
           90,
           105,
           20,
           hWnd_Options, /* this is the buttons parent window */
           (HMENU)IDB_TOP_DOOR, /* these next two lines pretty much tell windows what to do when the button is pressed */
           hInstance,
           NULL);

							button_right_door = CreateWindow(
           "BUTTON", /* this makes a "button" */
           "Right", /* this is the text which will appear in the button */
           WS_VISIBLE | WS_CHILD,
           120, /* these four lines are the position and dimensions of the button */
           90,
           105,
           20,
           hWnd_Options, /* this is the buttons parent window */
           (HMENU)IDB_RIGHT_DOOR, /* these next two lines pretty much tell windows what to do when the button is pressed */
           hInstance,
           NULL);

							button_down_door = CreateWindow(
           "BUTTON", /* this makes a "button" */
           "Down", /* this is the text which will appear in the button */
           WS_VISIBLE | WS_CHILD,
           335, /* these four lines are the position and dimensions of the button */
           90,
           105,
           20,
           hWnd_Options, /* this is the buttons parent window */
           (HMENU)IDB_DOWN_DOOR, /* these next two lines pretty much tell windows what to do when the button is pressed */
           hInstance,
           NULL);

							button_left_door = CreateWindow(
           "BUTTON", /* this makes a "button" */
           "Left", /* this is the text which will appear in the button */
           WS_VISIBLE | WS_CHILD,
           450, /* these four lines are the position and dimensions of the button */
           90,
           105,
           20,
           hWnd_Options, /* this is the buttons parent window */
           (HMENU)IDB_LEFT_DOOR, /* these next two lines pretty much tell windows what to do when the button is pressed */
           hInstance,
           NULL);

							label_door = CreateWindow(
           "STATIC", /* this makes a "button" */
           "Cost:", /* this is the text which will appear in the button */
           WS_VISIBLE | WS_CHILD,
           600, /* these four lines are the position and dimensions of the button */
           60,
           70,
           20,
           hWnd_Options, /* this is the buttons parent window */
           (HMENU)IDL_DOOR, /* these next two lines pretty much tell windows what to do when the button is pressed */
           hInstance,
           NULL);

							edit_door = CreateWindow(
           "EDIT", /* this makes a "button" */
           "1000", /* this is the text which will appear in the button */
           WS_VISIBLE | WS_CHILD,
           670, /* these four lines are the position and dimensions of the button */
           60,
           105,
           20,
           hWnd_Options, /* this is the buttons parent window */
           (HMENU)IDE_DOOR, /* these next two lines pretty much tell windows what to do when the button is pressed */
           hInstance,
           NULL);



						break;


						case IDM_FILE_COST:
							totalCost = 0;
							for(int i1=0;i1<noOfWalls;i1++)
								totalCost += wallCost[i1];
							for(int i2=0;i2<noOfChairs;i2++)
								totalCost += chairCost[i2];
							for(int i3=0;i3<noOfTables;i3++)
								totalCost += tableCost[i3];
							for(int i4=0;i4<noOfDoors;i4++)
								totalCost += doorCost[i4];
							char str[50];
							sprintf(str, "The total cost of the house is: %d", totalCost);
							MessageBox(NULL, str , "Total cost",0);
					}
					break;

/*				case WM_CREATE:
				{
					HMENU hMenu, hSubMenu;
					HICON hIcon, hIconSm;

					hMenu = CreateMenu();

					hSubMenu = CreatePopupMenu();
					AppendMenu(hSubMenu, MF_STRING, ID_FILE_New, "E&xit");
					AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenu, "&File");

					hSubMenu = CreatePopupMenu();
					AppendMenu(hSubMenu, MF_STRING, ID_STUFF_GO, "&Go");
					AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenu, "&Stuff");

					SetMenu(hwnd, hMenu);


					hIcon = LoadImage(NULL, "menu_two.ico", IMAGE_ICON, 32, 32, LR_LOADFROMFILE);
					if(hIcon)
						SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
					else
						MessageBox(hwnd, "Could not load large icon!", "Error", MB_OK | MB_ICONERROR);

					hIconSm = LoadImage(NULL, "menu_two.ico", IMAGE_ICON, 16, 16, LR_LOADFROMFILE);
					if(hIconSm)
						SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIconSm);
					else
						MessageBox(hwnd, "Could not load small icon!", "Error", MB_OK | MB_ICONERROR);
				}
				break;*/
	
/*				case WM_LBUTTONDOWN:					//If Left mouse button is pressed
        			lastx=LOWORD(lParam);			//Store the x-coordiante in lastx
        			lasty=HIWORD(lParam);			//Store the y-coordinate in lasty
        			return 0;
				case WM_MOUSEMOVE:						//When mouse is moved on the client area (or form for VB users)
        			hdc = GetDC(hWnd);					//hdc is handle to device context
        			x=LOWORD(lParam);					//Store the current x 
        			y=HIWORD(lParam);					//Store the current y
        			if (wParam & MK_LBUTTON)			//If Left mouse button is down then draw
        			{	
        				line(hdc,lastx,lasty,x,y);		//Draw the line frome the last pair of coordiates to current
        				lastx=x;						//The current x becomes the lastx for next line to be drawn
        				lasty=y;						//The current y becomes the lasty for next line to be drawn
        			}
        			ReleaseDC(hWnd,hdc);
        			return 0;*/
				case WM_PAINT:
					hdc = BeginPaint (hWnd, &ps) ;
					GetClientRect (hWnd, &rect) ;
        			 //TextOut(hdc,0,0 ,"What's up with this world?",53);		 		 		 
        			 EndPaint (hWnd, &ps) ;
					return 0 ;
        
				case WM_DESTROY:
					PostQuitMessage (WM_QUIT) ;
					break ;
			}

        
		case WM_ACTIVATE:							// Watch For Window Activate Message
		{
			if (!HIWORD(wParam))					// Check Minimization State
			{
				active=TRUE;						// Program Is Active
			}
			else
			{
				active=FALSE;						// Program Is No Longer Active
			}

			return 0;								// Return To The Message Loop
		}

		case WM_SYSCOMMAND:							// Intercept System Commands
		{
			switch (wParam)							// Check System Calls
			{
				case SC_SCREENSAVE:					// Screensaver Trying To Start?
				case SC_MONITORPOWER:				// Monitor Trying To Enter Powersave?
				return 0;							// Prevent From Happening
			}
			break;									// Exit
		}

		case WM_CLOSE:								// Did We Receive A Close Message?
		{
			PostQuitMessage(0);						// Send A Quit Message
			return 0;								// Jump Back
		}

		case WM_KEYDOWN:							// Is A Key Being Held Down?
		{
			keys[wParam] = TRUE;					// If So, Mark It As TRUE
			return 0;								// Jump Back
		}

		case WM_KEYUP:								// Has A Key Been Released?
		{
			keys[wParam] = FALSE;					// If So, Mark It As FALSE
			return 0;								// Jump Back
		}

		case WM_SIZE:								// Resize The OpenGL Window
		{
			ReSizeGLScene(LOWORD(lParam),HIWORD(lParam));  // LoWord=Width, HiWord=Height
			return 0;								// Jump Back
		}
	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

int WINAPI WinMain(	HINSTANCE	hInstance,			// Instance
					HINSTANCE	hPrevInstance,		// Previous Instance
					LPSTR		lpCmdLine,			// Command Line Parameters
					int			nCmdShow)			// Window Show State
{
	MSG		msg;									// Windows Message Structure
	BOOL	done=FALSE;								// Bool Variable To Exit Loop

	// Ask The User Which Screen Mode They Prefer
//	if (MessageBox(NULL,"Would You Like To Run In Fullscreen Mode?", "Start FullScreen?",MB_YESNO|MB_ICONQUESTION)==IDNO)
//	{
		fullscreen=FALSE;							// Windowed Mode
//	}


	// Initialize the Common Controls
	INITCOMMONCONTROLSEX InitCtrlEx;

	InitCtrlEx.dwSize = sizeof(INITCOMMONCONTROLSEX);
	InitCtrlEx.dwICC  = ICC_BAR_CLASSES;
	InitCommonControlsEx(&InitCtrlEx);

	
    WNDCLASSEX wc,wc2;

    //Step 1: Registering the Window Class
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = 0;
    wc.lpfnWndProc   = WndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInstance;
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.lpszMenuName  = MAKEINTRESOURCE(IDR_MAIN_MENU);
    wc.lpszClassName = g_szClassName;
    wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

    if(!RegisterClassEx(&wc))
    {
        MessageBox(NULL, "Window Registration Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // Step 2: Creating the Window
    hWnd_2DFrame = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        g_szClassName,
        "2D Drawing Area",
        WS_OVERLAPPEDWINDOW,
        0, 0, 375, 515,
        NULL, NULL, hInstance, NULL);


    if(hWnd_2DFrame == NULL)
    {
        MessageBox(NULL, "Window Creation Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }



    //Step 1: Registering the Window Class
    wc2.cbSize        = sizeof(WNDCLASSEX);
    wc2.style         = 0;
    wc2.lpfnWndProc   = WndProc;
    wc2.cbClsExtra    = 0;
    wc2.cbWndExtra    = 0;
    wc2.hInstance     = hInstance;
    wc2.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wc2.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc2.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc2.lpszMenuName  = NULL;
    wc2.lpszClassName = g_szClassName2;
    wc2.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

    if(!RegisterClassEx(&wc2))
    {
        MessageBox(NULL, "Window Registration Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }


    
	hWnd_Options = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        g_szClassName2,
        "Options",
        WS_OVERLAPPEDWINDOW,
        0, 515, 1024, 215,
        NULL, NULL, hInstance, NULL);

    if(hWnd_Options == NULL)
    {
        MessageBox(NULL, "Window Creation Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }



	TBADDBITMAP tbab;
	tbab.hInst = hInstance;
    tbab.nID = IDB_STANDARD;
    //SendMessage(hWndToolbar, TB_ADDBITMAP, 0, (LPARAM)&tbab);

	const int NUMBUTTONS = 3;

	TBBUTTON tbrButtons[NUMBUTTONS];

	ZeroMemory(tbrButtons, sizeof(tbrButtons));

	tbrButtons[0].iBitmap   = 0;
	tbrButtons[0].idCommand = IDM_FILE_NEW;
	tbrButtons[0].fsState   = TBSTATE_ENABLED;
	tbrButtons[0].fsStyle   = TBSTYLE_BUTTON;
	tbrButtons[0].dwData    = 0L;
	tbrButtons[0].iBitmap   = 0;
	tbrButtons[0].iString   = 0;
	
	tbrButtons[1].iBitmap   = 1;
	tbrButtons[1].idCommand = IDM_FILE_OPEN;
	tbrButtons[1].fsState   = TBSTATE_ENABLED;
	tbrButtons[1].fsStyle   = TBSTYLE_BUTTON;
	tbrButtons[1].dwData    = 0L;
	tbrButtons[1].iString   = 0;


	HWND hWndToolbar;
//	hWndToolbar = CreateWindowEx(0, g_szClassName, NULL, WS_CHILD | WS_VISIBLE, 0, 0, 100, 30,
//        hWnd_2DFrame, (HMENU)IDR_MAIN_MENU, GetModuleHandle(NULL), NULL);

	hWndToolbar = MyCreateToolBar(NULL,1,tbab,2,tbrButtons[NUMBUTTONS],0,IDR_TOOLBAR);

	SendMessage(hWndToolbar, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);
 

//SendMessage(hWndToolbar, TB_ADDBUTTONS, sizeof(tbrButtons)/sizeof(TBBUTTON), (LPARAM)&tbrButtons);


   ShowWindow(hWnd_2DFrame, nCmdShow);
    UpdateWindow(hWnd_2DFrame);
	
   ShowWindow(hWnd_Options, nCmdShow);
    UpdateWindow(hWnd_Options);
	
	
	
	// Create Our OpenGL Window
	if (!CreateGLWindow("3D Navigation Area",640,480,16,fullscreen))
	{
		return 0;									// Quit If Window Was Not Created
	}

	while(!done)									// Loop That Runs While done=FALSE
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))	// Is There A Message Waiting?
		{
			if (msg.message==WM_QUIT)				// Have We Received A Quit Message?
			{
				done=TRUE;							// If So done=TRUE
			}
			else									// If Not, Deal With Window Messages
			{
				TranslateMessage(&msg);				// Translate The Message
				DispatchMessage(&msg);				// Dispatch The Message
			}
		}
		else										// If There Are No Messages
		{
			// Draw The Scene.  Watch For ESC Key And Quit Messages From DrawGLScene()
			if (active)								// Program Active?
			{
				if (keys[VK_ESCAPE])				// Was ESC Pressed?
				{
					done=TRUE;						// ESC Signalled A Quit
				}
				else								// Not Time To Quit, Update Screen
				{
					DrawGLScene();					// Draw The Scene
					SwapBuffers(hDC);				// Swap Buffers (Double Buffering)
				}
			}

/*			if (keys[VK_F1])						// Is F1 Being Pressed?
			{
				keys[VK_F1]=FALSE;					// If So Make Key FALSE
				KillGLWindow();						// Kill Our Current Window
				fullscreen=!fullscreen;				// Toggle Fullscreen / Windowed Mode
				// Recreate Our OpenGL Window
				if (!CreateGLWindow("NeHe's OpenGL Framework",640,480,16,fullscreen))
				{
					return 0;						// Quit If Window Was Not Created
				}
			}
*/
			if (keys[VK_RIGHT])							// Is The Right Arrow Being Pressed?
			{
				 heading -= 0.5f;
				 yrot = heading;
			}

			if (keys[VK_LEFT])							// Is The Left Arrow Being Pressed?
			{
				 heading += 0.5f;
				 yrot = heading;
			}

			if (keys[VK_UP])							// Is The Up Arrow Being Pressed?
			{
				xpos -= (float)sin(heading*piover180) * 0.05f;			// Move On The X-Plane Based On Player Direction
				zpos -= (float)cos(heading*piover180) * 0.05f;			// Move On The Z-Plane Based On Player Direction
				if (walkbiasangle >= 359.0f)					// Is walkbiasangle>=359?
				{
					walkbiasangle = 0.0f;					// Make walkbiasangle Equal 0
				}
				else								// Otherwise
				{
					 walkbiasangle+= 10;					// If walkbiasangle < 359 Increase It By 10
				}
				walkbias = (float)sin(walkbiasangle * piover180)/20.0f;		// Causes The Player To Bounce
			}

			if (keys[VK_DOWN])							// Is The Down Arrow Being Pressed?
			{
				xpos += (float)sin(heading*piover180) * 0.05f;			// Move On The X-Plane Based On Player Direction
				zpos += (float)cos(heading*piover180) * 0.05f;			// Move On The Z-Plane Based On Player Direction
				if (walkbiasangle <= 1.0f)					// Is walkbiasangle<=1?
				{
					walkbiasangle = 359.0f;					// Make walkbiasangle Equal 359
				}
				else								// Otherwise
				{
					walkbiasangle-= 10;					// If walkbiasangle > 1 Decrease It By 10
				}
				walkbias = (float)sin(walkbiasangle * piover180)/20.0f;		// Causes The Player To Bounce
			}

			if (keys[VK_NUMPAD2])
			{
				lookupdown-=0.1f;
			}

			if (keys[VK_NUMPAD8])
			{
				lookupdown+=0.1f;
			}

			if (keys[VK_NUMPAD9])
			{
				ypos+=0.05f;
			}

			if (keys[VK_NUMPAD3])
			{
				ypos-=0.05f;
			}

			if (keys[VK_F1])
			{
				if(!light0){
					glEnable(GL_LIGHT0);
					light0=TRUE;
				}
				else{
					light0=FALSE;
					glDisable(GL_LIGHT0);
				}
			}

			if (keys[VK_F2])
			{
				if(!light1){
					glEnable(GL_LIGHT1);
					light1=TRUE;
				}
				else{
					glDisable(GL_LIGHT1);
					light1=FALSE;
				}
			}



		}
	}


	
	
	// Shutdown
	KillGLWindow();									// Kill The Window
	return (msg.wParam);							// Exit The Program
}

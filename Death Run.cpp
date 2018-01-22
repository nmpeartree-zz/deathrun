/***************************************
   Death Run Long Island              *
BASH KONTROLLER ENTERTAINMENT         *
Authors: Eric Larsen & Nelson Pereira *
 (c) 2004 ALL RIGHTS RESERVED         *
***************************************/




//* INCLUDES *///////////////////////////////////////////////
#define WIN32_LEAN_AND_MEAN  
#define INITGUID //for COM object ID

#include <windows.h>   // include important windows stuff
#include <windowsx.h>
#include <mmsystem.h>
#include <objbase.h>
#include <iostream.h> // include important C/C++ stuff
#include <conio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <math.h>
#include <io.h>
#include <fcntl.h>


#include <ddraw.h>  // DirectX modules
#include <dinput.h>
#include <dsound.h>
#include <dmksctrl.h>
#include <dmusici.h>
#include <dmusicc.h>
#include <dmusicf.h>

#include "T3DLIB1.H" //include library files
#include "T3DLIB2.H"
#include "T3DLIB3.H"
//#include "resource.h" // include resource files (Icon only for now)
//////////////////////////////////////////////////////////



//* DEFINES */////////////////////////////////////////////
// defines for windows
#define WINDOW_CLASS_NAME "WINCLASS1"

// default screen size
#define SCREEN_WIDTH    800  // size of screen
#define SCREEN_HEIGHT   600
#define SCREEN_BPP      16    // bits per pixel

#define BITMAP_ID            0x4D42 // universal id for a bitmap
#define MAX_COLORS_PALETTE   256

#define NUM_OBJECTS_UNIVERSE    200 //total number of objects

#define NUM_PATTERNS    4     // number of patterns in system

//#define MAX_UNIVERSE_X 600
//#define MAX_UNIVERSE_Y 16000
/////////////////////////////////////////////////////////////


//* PROTOTYPES */////////////////////////////////////////////
int Game_Init(void *parms=NULL, int num_parms = 0);
int Game_Shutdown(void *parms=NULL, int num_parms = 0);
int Game_Main(void *parms=NULL,  int num_parms = 0);
int Game_Splash(void);
int Load_Level(int level);
void PowerUp();
void Collision_Detection(int index);
void AI(int index);
void Gas(float minus);
void Draw_Objects();
int Game_Gui();
void Terrorist_AI(int index);
void Game_Mode(int mode);
void Boss_Level();
//////////////////////////////////////////////////////////////



//* GLOBALS *////////////////////////////////////////////////

// windows vars
HWND      main_window_handle = NULL; // globally track main window
int       window_closed      = 0;    // tracks if window is closed
HINSTANCE main_instance      = NULL; // globally track hinstance
char buffer[80];                // used to print text


//WORLD
#define TILE_LENGTH 800
#define TILE_WIDTH 600
#define TILE_SHIFT   1 // used for shifting
#define NUM_TEXTURES   1 //only one per bitmap file
//#define BOARD_WIDTH    100
//#define BOARD_HEIGHT   16000 // 20 tiles high
BOB textures;     // texture memory
int X_TILES_PER_SCREEN = 1;
int Y_TILES_PER_SCREEN = 1;
int world_x = 0,   // current position of viewing window
    world_y = 16000;
//the background world
char *world[20];
int loadFlag = 0; //flag for loading a level
float tempo = 1.0; //for DirectMusic
int color_scan; //color scan for grass

BOB bossHead;

//OBJECTS
BOB_PTR object_ptr; // the object that is going to be rendered
typedef struct OBJECT
{
       int type;  // type of BOB object
	   int currF;
       int x,y;   // position of object
       int vx,vy;  // x,y velocity for movement
       int width;// these will be used for bounding boxes
       int height;
} GAME_OBJ;

typedef struct OBJECT2
{
       int type;  // type of object
       int x,y;   // position of object
       int vx,vy;  // x,y velocity for movement
       int width;// these will be used for bounding boxes
       int height;
} GAME_OBJ_GUI;


GAME_OBJ worldObjects[NUM_OBJECTS_UNIVERSE]; // our world consists of 200 objects randomly placed
GAME_OBJ_GUI gui[10];
int selected = 1;
BOB menu, about, help, menuSelect; //menu stuff
BOB gamemode; //for game over or level complete;


#define TREE		0 
#define POTHOLE		1
#define CAR			2
#define BUILDING1	3 //2 buildings per level for uniqueness
#define BUILDING2	4
#define FLY         5
#define ADULT1		6
#define ADULT2		7
#define OLD			8
#define LEVELPERSON 9  //refers to the unique person of the level
#define POWERUP		10 //one powerup per level

char *world1b[4]= 
   {
	"555555555555555555555555555555555555",
	"012301230123012301230123012301230123",// middle of board
	"555555555555555555555555555555555555",
	"555555555555555555555555555555555555",/// dont put anything here
	
   };
//level1
char *world1[36]= 
   {
	"505",
	"505",
	"505",
	"505",
	"505",
	"505",
	"535",
	"565",
	"515",
	"505",
	"505",
	"535",
	"515",
	"525",
	"575",
	"575",
	"575",
	"545",
	"505",
	"535",
	"565",
	"565",
	"565",
	"565",
	"515",
	"505",
	"505",//15600
	"535",
	"565",
	"565",
	"515",
	"505",
	"505",
	"505",
	"505",// bottom of world
	"555",
	
   };
//level2
char *world2[4]= 
  {
	"555555555555555555555555555555555555",
	"000166200373437773012120016620377743",// middle of board
	"555555555555555555555555555555555555",
	"555555555555555555555555555555555555",/// dont put anything here
	
   };
//levelboss2
//level3
char *world3[36]= 
{
	"005",
	"005",
	"005",
	"005",
	"005",
	"005",
	"005",
	"005",
	"005",
	"005",
	"005",
	"005",
	"005",
	"005",
	"005",
	"005",
	"005",
	"005",
	"005",
	"005",
	"005",
	"005",
	"005",
	"005",
	"005",
	"005",
	"005",//15600
	"005",
	"005",
	"005",
	"005",
	"005",
	"005",
	"005",
	"005",// bottom of world
	"005",
	
   };
//levelboss3
char *world3b[37]= 
  {
	"000000000000000000000000000000000000",
	"000000000000000000000000000000000000",// middle of board
	"555555555555555555555555555555555555",
	"555555555555555555555555555555555555",/// dont put anything here
   };
//level4
char *world4[37]= {
   	"000000000000000000000000000000000000",
	"000000000000000000000000000000000000",// middle of board
	"555555555555555555555555555555555555",
	"555555555555555555555555555555555555",/// dont put anything here
   };
//levelboss4
char *world4b[36]= 
   {
	"005",
	"005",
	"005",
	"005",
	"005",
	"005",
	"005",
	"005",
	"005",
	"005",
	"005",
	"005",
	"005",
	"005",
	"005",
	"005",
	"005",
	"005",
	"005",
	"005",
	"005",
	"005",
	"005",
	"005",
	"005",
	"005",
	"005",//15600
	"005",
	"005",
	"005",
	"005",
	"005",
	"005",
	"005",
	"005",// bottom of world
	"005",
	
   };

BOB jeep;
BOB tree, pothole, car, fly, building1, building2;
BOB adult1,adult2, old, levelPerson;
BOB powerup, projectile;
int jeep_x=0, jeep_y=0;

//Projectile Animation
int projectile_anim[1][2] = { {0,1}, };

POLYGON2D gas; // the gas meter
VERTEX2DF gas_vertices[4] = {-10,0,
							  650,0,
							  650,-10,
							  -10,-10};
float gas_min_x,
      gas_min_y,
      gas_max_x,
      gas_max_y;


int itemsFlag[NUM_OBJECTS_UNIVERSE];//flag for determining uttermost death 199 TOTAL


//GAME VARIABLES
int MAX_UNIVERSE_X=600;
int MAX_UNIVERSE_Y=16000;
int BOARD_WIDTH=100; //         1600 //test for straight line
int BOARD_HEIGHT=16000;  //       1600 // 20 tiles high

int show_PowerUp = 0;// flag to show powerup
int fire_PowerUp = 0;
int scroll_speedx = 15;
int scroll_speedy = 15;
int lives = 7;
int score = 0; // score
int hit = 0; //number of people killed
int fuel; //no use
float gasDown = 0.9995; // amount gas drops
float loseGas; //if hit objects or grass, lose more gas
float scalesize = 1.0; //for gas calculations
int timer; //for boss levels
int pause = 1;
int currLevel = 0; //current level, start at game menu
int levelComplete = 0; //for completing a level
int gameOver = 0; //for game over screen
int badgerDead = 0, pikachuDead = 0;

//BOSS LEVELS
int target = 0; //challenged people to kill
int innocent = 0; //other people

//AI
#define M_E    0  // move west
#define M_NE   1  // move northeast
#define M_N    2  // move north
#define M_NW   3  // move northwest
#define M_W    4  // move west
#define M_SW   5  // move southwest
#define M_S    6  // move south
#define M_SE   7  // move southeast

// special instructions
#define M_STOP 8  // stop for a moment
#define M_RAND 9  // select a random direction
#define M_JEEP_DIST   10 // test distance
#define M_END  -1 // end pattern
#define RUN_AWAY  300 // minimum distance before people run

// patterns in Mode operand format
int pattern_1[] = {M_W,2, M_NW,2, M_JEEP_DIST,50,  // a distance test
                   M_W,2, M_SW,2,M_W,2, M_NW,2, M_JEEP_DIST,50,  // a distance test
                   M_W,2, M_SW,2,M_W,2, M_NW,2, M_JEEP_DIST,50,  // a distance test
                   M_W,2, M_SW,2,M_W,2, M_NW,2, M_JEEP_DIST,50,  // a distance test
                   M_W,2, M_SW,2,M_W,2, M_NW,2, M_JEEP_DIST,50,  // a distance test
                   M_W,2, M_SW,2,M_W,2, M_NW,2, M_JEEP_DIST,50,  // a distance test
                   M_W,2, M_SW,2, M_END,0};
                   

int pattern_2[] = {M_E,2, M_NE,2,  M_JEEP_DIST,50,  // a distance test
                   M_E,2, M_SE,2, M_E,2, M_NE,2,  M_JEEP_DIST,50,  // a distance test
                   M_E,2, M_SE,2, M_E,2, M_NE,2,  M_JEEP_DIST,50,  // a distance test
                   M_E,2, M_SE,2, M_E,2, M_NE,2,  M_JEEP_DIST,50,  // a distance test
                   M_E,2, M_SE,2, M_E,2, M_NE,2,  M_JEEP_DIST,50,  // a distance test
                   M_E,2, M_SE,2, M_E,2, M_NE,2,  M_JEEP_DIST,50,  // a distance test
                   M_E,2, M_SE,2, M_E,2, M_NE,2,  M_JEEP_DIST,50,  // a distance test
                   M_E,2, M_SE,2,  M_END,0};



int pattern_3[] = {M_N,2, M_NW, 2, M_JEEP_DIST, 50, // a distance test
                   M_N,2, M_NE, 2,M_N,2, M_NW, 2, M_JEEP_DIST, 50, // a distance test
                   M_N,2, M_NE, 2,M_N,2, M_NW, 2, M_JEEP_DIST, 50, // a distance test
                   M_N,2, M_NE, 2,M_N,2, M_NW, 2, M_JEEP_DIST, 50, // a distance test
                   M_N,2, M_NE, 2,M_N,2, M_NW, 2, M_JEEP_DIST, 50, // a distance test
                   M_N,2, M_NE, 2,M_N,2, M_NW, 2, M_JEEP_DIST, 50, // a distance test
                   M_N,2, M_NE, 2, M_END,0};


int pattern_4[] = {M_S,2, M_SW, 2, M_JEEP_DIST, 50, // a distance test
                   M_S,2, M_SE, 2,M_S,2, M_SW, 2, M_JEEP_DIST, 50, // a distance test
                   M_S,2, M_SE, 2,M_S,2, M_SW, 2, M_JEEP_DIST, 50, // a distance test
                   M_S,2, M_SE, 2,M_S,2, M_SW, 2, M_JEEP_DIST, 50, // a distance test
                   M_S,2, M_SE, 2,M_S,2, M_SW, 2, M_JEEP_DIST, 50, // a distance test
                   M_S,2, M_SE, 2,M_S,2, M_SW, 2, M_JEEP_DIST, 50, // a distance test
                   M_S,2, M_SE, 2,M_S,2, M_SW, 2, M_JEEP_DIST, 50, // a distance test
                   M_S,2, M_SE, 2, M_END,0};





int *patterns[NUM_PATTERNS] = {pattern_1, pattern_2, pattern_3, pattern_4};

int *curr_pattern = NULL;// current pattern being processed

int person_ip=0,       // pattern instruction pointer for person
    person_counter=0,       // counter of pattern control
    person_pattern_index;   // the current pattern being executed

// used as a index to string lookup to help print out
char *Mode_names[] = {"M_E",
                      "M_NE", 
                      "M_N",    
                      "M_NW",   
                      "M_W",    
                      "M_SW",   
                      "M_S",    
                      "M_SE",   
                      "M_STOP", 
                      "M_RAND", 
                      "M_JEEP_DIST"};  



//////////////////////////////////////////////////////////



//* WinProc *//////////////////////////////////////////////

LRESULT CALLBACK WindowProc(HWND hwnd,
                           UINT msg,
                           WPARAM wparam,
                           LPARAM lparam)
{
// this is the main message handler of the system
PAINTSTRUCT     ps;     // used in WM_PAINT
HDC             hdc;    // handle to a device context
char buffer[80];        // used to print strings

// what is the message
switch(msg)
   {
   case WM_CREATE:
       {
       return(0);
       } break;

      case WM_PAINT:
       {
       // simply validatd the window
       hdc = BeginPaint(hwnd,&ps);  
       
       // end painting
       EndPaint(hwnd,&ps);

       // return success
       return(0);
       } break;

   case WM_DESTROY:
       {
       PostQuitMessage(0);

       // return success
       return(0);
       } break;

   default:break;

   } // end switch

// process any messages that we didn't take care of
return (DefWindowProc(hwnd, msg, wparam, lparam));

} // end WinProc
/////////////////////////////////////////////////////////////



//* WINMAIN *////////////////////////////////////////////////

int WINAPI WinMain( HINSTANCE hinstance,
                   HINSTANCE hprevinstance,
                   LPSTR lpcmdline,
                   int ncmdshow)
{

WNDCLASSEX winclass; // this will hold the class we create
HWND       hwnd;     // generic window handle
MSG        msg;      // generic message
HDC        hdc;      // graphics device context

// first fill in the window class stucture
winclass.cbSize         = sizeof(WNDCLASSEX);
winclass.style          = CS_DBLCLKS | CS_OWNDC |
                         CS_HREDRAW | CS_VREDRAW;
winclass.lpfnWndProc    = WindowProc;
winclass.cbClsExtra     = 0;
winclass.cbWndExtra     = 0;
winclass.hInstance      = hinstance;
winclass.hIcon          = LoadIcon(NULL, IDI_APPLICATION);
winclass.hCursor        = LoadCursor(NULL, IDC_ARROW);
winclass.hbrBackground  = (HBRUSH)GetStockObject(BLACK_BRUSH);
winclass.lpszMenuName   = NULL;
winclass.lpszClassName  = WINDOW_CLASS_NAME;
winclass.hIconSm        = LoadIcon(NULL, IDI_APPLICATION);//need new icon

// save hinstance in global
main_instance = hinstance;

// register the window class
if (!RegisterClassEx(&winclass))
   return(0);

// create the window
if (!(hwnd = CreateWindowEx(NULL,                  // extended style
                           WINDOW_CLASS_NAME,     // class
                           "Death Run: Long Island", // title
                           WS_OVERLAPPED | WS_VISIBLE ,
                           0,0,      // initial x,y
                           SCREEN_WIDTH,SCREEN_HEIGHT,  // initial width, height
                           NULL,     // handle to parent
                           NULL,     // handle to menu
                           hinstance,// instance of this application
                           NULL)))   // extra creation parms
return(0);

// save main window handle
main_window_handle = hwnd;

// initialize game here
Game_Init();

// enter main event loop
while(TRUE)
   {
   // test if there is a message in queue, if so get it
   if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
      {
      // test if this is a quit
      if (msg.message == WM_QUIT)
          break;
   
      // translate any accelerator keys
      TranslateMessage(&msg);

      // send the message to the window proc
      DispatchMessage(&msg);
      } // end if
   
      // main game processing goes here
      Game_Main();
     
   } // end while

// closedown game here
Game_Shutdown();

// return to Windows like this
return(msg.wParam);

} // end WinMain
///////////////////////////////////////////////////


/*******************GAME PROGRAMMING MODULES************************/


//* GAME_INIT *////////////////////////////////////

int Game_Init(void *parms,  int num_parms)
{
   
   // hide the mouse
   ShowCursor(FALSE);

   //int cap = 0;
   int index;         // looping var
   char filename[80]; // used to build up files names


   // initilize DirectSound
   DSound_Init();
   // initialize DirectDraw
   DDraw_Init(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP);
   // initialize DirectMusic
   DMusic_Init();
   // initialize DirectInput
   DInput_Init();
   // acquire the keyboard only
      DInput_Init_Joystick(-24,24,-24,24,10);

   DInput_Init_Keyboard();
//   DInput_Init_Joystick(-24,24,-24,24,10);

	// initialize gas meter
	gas.state = 1;   // turn it on
	gas.num_verts = 4; 
	gas.x0 = 55; // position it
	gas.y0 = 45;
	gas.xv = 0;
	gas.yv = 0;
	gas.color = 15; // green
	gas.vlist = new VERTEX2DF [gas.num_verts];

	for (index = 0; index < gas.num_verts; index++)
		gas.vlist[index] = gas_vertices[index];	


   // initialize directdraw to fullscreen
   lppd_FullMode();
	
	DSound_Load_WAV("SOUNDS\\crash.WAV");
	DSound_Set_Freq(0,44100);
    
	DSound_Load_WAV("SOUNDS\\HIT.WAV");
    
	DSound_Load_WAV("SOUNDS\\THUMP.WAV");
    
	DSound_Load_WAV("SOUNDS\\powerup.WAV");
	DSound_Set_Freq(3,44100);
	
	DSound_Load_WAV("SOUNDS\\Boss1.WAV");
	DSound_Set_Freq(4,44100);
	
	DSound_Load_WAV("SOUNDS\\Boss2.WAV");
	DSound_Set_Freq(5,44100);
	
	DSound_Load_WAV("SOUNDS\\Boss3.WAV");
	DSound_Set_Freq(6,44100);
	
	DSound_Load_WAV("SOUNDS\\Boss4.WAV");
	DSound_Set_Freq(7,44100);

	DSound_Load_WAV("BONUS\\Badger.WAV");
	DSound_Load_WAV("BONUS\\POKE-PIKACHU1.WAV");
	DSound_Load_WAV("BONUS\\Aol.WAV");

   
   // run intro & splash screen
  Game_Splash();

   return(1); //success
   
} // end Game_Init

///////////////////////////////////////////////////////////



//* GAME_SHUTDOWN */////////////////////////////////////
int Game_Shutdown(void *parms,  int num_parms)
{

   // first the palette
   if (lpddpal)
   {
       lpddpal->Release();
       lpddpal = NULL;
   } // end if

   // now the back buffer surface
   if (lpddsback)
   {
       lpddsback->Release();
       lpddsback = NULL;
   } // end if

   // now the primary surface
   if (lpddsprimary)
   {
       lpddsprimary->Release();
       lpddsprimary = NULL;
   } // end if

   // shut down directinput
   DInput_Shutdown();
   // shutdown directdraw
   DDraw_Shutdown();
   // now directsound
   DSound_Stop_All_Sounds();
   // shutdown directsound
   DSound_Shutdown();
   // delete all the music
   DMusic_Delete_All_MIDI();
   // shutdown directmusic
   DMusic_Shutdown();


   return(1);//return success

} // end Game_Shutdown

///////////////////////////////////////////////////////////


//* GAME_MAIN * ///////////////////////////////////////////
int Game_Main(void *parms, int num_parms)
{

   
   int width, height, type;      // used to extract bob info
   int x0,y0,x1,y1; // used to compute the upper left and lower right corners of each object
   int index, index_x, index_y;  // looping vars
   int start_map_x,start_map_y,  // map positions
   end_map_x,end_map_y; 

   
	loseGas = 0.0; //reset the lose gas variable


	//Exit the Game at anytime
	DInput_Read_Keyboard();
	DInput_Read_Joystick();
		//F-KEYS FOR DEMO PRESENTATION
	if(keyboard_state[DIK_F1])
	{
		loadFlag = 0;
		currLevel = 1;
	}
	if(keyboard_state[DIK_F2])
	{
		loadFlag = 0;
		currLevel = 2;
	}
	if(keyboard_state[DIK_F3])
	{
		loadFlag = 0;
		currLevel = 3;
	}
	if(keyboard_state[DIK_F4])
	{
		loadFlag = 0;
		currLevel = 4;
	}
	if(keyboard_state[DIK_F5])
	{
		loadFlag = 0;
		currLevel = 5;
	}
	if(keyboard_state[DIK_F6])
	{
		loadFlag = 0;
		currLevel = 6;
	}
	if(keyboard_state[DIK_F7])
	{
		loadFlag = 0;
		currLevel = 7;
	}
	if(keyboard_state[DIK_F8])
	{
		loadFlag = 0;
		currLevel = 8;
	}




	// exit game
	if (keyboard_state[DIK_X])
	{	
       Game_Shutdown();
       PostQuitMessage(0); //kill the app & exit
	}

	// pause game
	if(keyboard_state[DIK_ESCAPE]||(joy_state.rgbButtons[4]&&joy_state.rgbButtons[5]&&joy_state.rgbButtons[6]&&joy_state.rgbButtons[7]))
		pause = 1;

       

   // start the timing clock
   Start_Clock();


   DDraw_Fill_Surface(lpddsback, 0);

   
   //if level is complete
   if(levelComplete == 1)
   {
	   Game_Mode(1);
   }

   //if game is over
   if(gameOver == 1 || lives == 0)
   {
	   Game_Mode(0);
   }

   //load the level
   if(loadFlag == 0)
   {
	Load_Level(currLevel); //get this level
	loadFlag = 1; //level does not need to be loaded
   }


   if(pause == 1)//menu screen
	   Game_Gui();

   if(pause == 2)//about screen
   {
	   Draw_BOB(&about, lpddsback);
	   if (keyboard_state[DIK_SPACE] &0x80)
		   pause = 1;
   }

   if(pause == 3) //help screen
   {
	   Draw_BOB(&help, lpddsback);
	   if (keyboard_state[DIK_SPACE] &0x80)	
			pause = 1; //back to menu
   }


   //logic for levels
   if(currLevel > 0 && pause == 0 && levelComplete == 0 && gameOver == 0) //main levels and boss levels
  {
	   int scrollBoarder = 250;
   int jeepSize = 150;
	

	   // lock surface, so we can for off road
	DDraw_Lock_Back_Surface();
 /*
	//scan for grass
	if(Color_Scan(jeep_x+16, jeep_y, jeep_x+35, jeep_y+jeepSize,                                    
               color_scan, color_scan, back_buffer,back_lpitch))
	{
		loseGas += 0.0005;
		scroll_speedx = 10;
	scroll_speedy =10;
    } // end while
	else {scroll_speedx = 15;
	scroll_speedy =15;}
*/
	// done, so unlock
	DDraw_Unlock_Back_Surface();



	//tile scrolling
   // compute starting map indices by dividing position by size of cell
   start_map_x = world_x/800;
   start_map_y = world_y/600;

   // compute end of map rectangle for best cast
    end_map_x = start_map_x + X_TILES_PER_SCREEN - 1;
	end_map_y = start_map_y+1;


   // how much is scrolled off the edge?
	int offset_x = -(world_x % TILE_LENGTH);
	int offset_y = -(world_y % 600);
 

	// set starting position in first upper lh texture

	//int texture_x = 0;
	//int texture_y = 0;
	// adjust end_map_x,y for offsets

	if (offset_x)
		end_map_x++;

	if (offset_y)
		end_map_y++;

	int texture_x = offset_x;
	int texture_y = offset_y;


   // draw the current window
   for (index_y = start_map_y; index_y <= end_map_y; index_y++)
   {
       for (index_x = start_map_x; index_x <= end_map_x; index_x++)
       {
           // set position to blit
           textures.x = texture_x;
           textures.y = texture_y;
       
	   	
		   switch(currLevel){
		   case 1 :{
           textures.curr_frame = world1[index_y][index_x] - '0';
		   }break;
			 case 2 :{
           textures.curr_frame = world1b[index_y][index_x] - '0';
		   }break;
			 case 3 :{
           textures.curr_frame = world2[index_y][index_x] - '0';
		   }break;
			
			case 5 :{
           textures.curr_frame = world3[index_y][index_x] - '0';
		   }break;
			case 6 :{
           textures.curr_frame = world3b[index_y][index_x] - '0';
		   }break;
			case 7 :{
           textures.curr_frame = world4[index_y][index_x] - '0';
		   }break;
			case 8 :{
           textures.curr_frame = world4b[index_y][index_x] - '0';
		   }break;
			default:
				//textures.curr_frame = world4b[index_y][index_x] - '0';
				break;
		   }// end switch
    
           // draw the texture
           Draw_BOB16(&textures,lpddsback);

           // update texture position
           texture_x+=TILE_LENGTH;

       } // end for map_x

       // reset x postion, update y
       texture_x =  offset_x;
       texture_y += 600;

   } // end for map_y

 

	// input handling for game-- to be put in the Pause loop
	DInput_Read_Keyboard();
	// these keys will control the jeep, gameplay
	
	
	
			if (keyboard_state[DIK_RIGHT]||joy_state.lX >0){
			if(currLevel==1||currLevel==4||currLevel == 5||currLevel==8)
				projectile.x-=scroll_speedx;
			else
				projectile.x-=5;
		if((world_x == 0)&&(jeep_x+scroll_speedx) <= scrollBoarder)
			jeep_x+=scroll_speedx;
		else{
			if ((world_x+=scroll_speedx) >= BOARD_WIDTH)
				world_x = BOARD_WIDTH-1;
			
			
			if((jeep_x+=scroll_speedx) >= BOARD_WIDTH+800-jeepSize)// screen width - 150(size of jeep)
				jeep_x=BOARD_WIDTH+800-jeepSize;
			
			if(world_x <= BOARD_WIDTH+650 && jeep_x> world_x+scrollBoarder)//view port tracking
				world_x+=5;//scroll_speed;
		}	
	}
	
	if (keyboard_state[DIK_LEFT]||joy_state.lX <0){	
		if(currLevel==1||currLevel==4||currLevel == 5||currLevel==8)
			projectile.x+=scroll_speedx;
		else
			projectile.x+=5;
		if((world_x == BOARD_WIDTH-1)&&(jeep_x-scroll_speedx) >= world_x+650-scrollBoarder)
			jeep_x-=scroll_speedx;
		else{
			if ((world_x-=scroll_speedx) <= 0)
					world_x = 0;
		if((jeep_x-=scroll_speedx) <= 0)
				jeep_x=0;
		if(world_x >= 650 && jeep_x < world_x+650-scrollBoarder)//viewport tracking
				world_x-=5;//scroll_speed;
			}	
	
	}
	
scrollBoarder = 60;
	if (keyboard_state[DIK_UP]||joy_state.lY <0){	
		if(currLevel==2||currLevel==3||currLevel == 6||currLevel==7)
			projectile.y+=scroll_speedy+5;
		else
			projectile.y+=5;
		if((world_y == BOARD_HEIGHT-1)&&(jeep_y - scroll_speedy) >= world_y+450-scrollBoarder)
			jeep_y -= scroll_speedy;
		
	else{
		if ((world_y-= scroll_speedy) <= 0)
				world_y = 0;
		if((jeep_y-= scroll_speedy) <= 0)
				jeep_y= 0;
		if(world_y >= 450 && jeep_y < world_y+450-scrollBoarder)//view port tracking
				world_y-=5;//scroll_speed;
		}
	
	}


	if (keyboard_state[DIK_DOWN]||joy_state.lY >0){	
		if(currLevel==2||currLevel==3||currLevel == 6||currLevel==7)
			projectile.y-=scroll_speedy;
		else
			projectile.y-=5;
		if((world_y == 0)&&(jeep_y+scroll_speedy) <= scrollBoarder)
			jeep_y+=scroll_speedy;

	else{
		if ((world_y+=scroll_speedy) >= BOARD_HEIGHT)
				world_y = BOARD_HEIGHT-1;
		if((jeep_y+=scroll_speedy) >= BOARD_HEIGHT+450)// screen height - 150(jeep height)
				jeep_y=BOARD_HEIGHT+450-1;
		if(world_y <= BOARD_HEIGHT+450 && jeep_y > world_y+scrollBoarder)//view port tracking
				world_y+= 5;//scroll_speed;
		}	

	}

	if(show_PowerUp == 1)//if got powerup
	{
		if(keyboard_state[DIK_SPACE]||joy_state.rgbButtons[0]){
			//projectile.x = jeep_x+30;
			//projectile.y = jeep_y;
			fire_PowerUp = 1;//shoot
		}
	}

	if(fire_PowerUp == 1)//if powerup has been shot
	{
		PowerUp();//do powerup stuff
	}

	//Draw & Animate objects onto the screen.
	//AI is also done in this function
	//Collision Detection is also done in this function
	Draw_Objects();


	//INFOBAR
	if(currLevel == 1 || currLevel == 5)
	{
		sprintf(buffer,"Lives: %d   Score: %d    Distance: %d    Hit: %d", lives, score, world_y, hit);
		Draw_Text_GDI(buffer,10,10,RGB(255,255,255),lpddsback);
		Draw_Text_GDI("Gas:",10,30,RGB(255,255,255),lpddsback);
		if(world_y <= 0) //check for end of level
		{
			loadFlag = 0;
			levelComplete = 1;
		}

	}
	else if(currLevel == 3 || currLevel == 7)
	{
		int remaining = MAX_UNIVERSE_X - world_x;
		sprintf(buffer,"Lives: %d   Score: %d    Distance: %d    Hit: %d", lives, score, remaining, hit);
		Draw_Text_GDI(buffer,10,10,RGB(255,255,255),lpddsback);
		Draw_Text_GDI("Gas:",10,30,RGB(255,255,255),lpddsback);
		if(remaining <= 0)//check for end of level
		{
			loadFlag = 0;
			levelComplete = 1;
		}
	}
	else if(currLevel == 9)
	{
		sprintf(buffer,"BONUS--KILL ANYTHING IN YOUR PATH!!!");
		Draw_Text_GDI(buffer,10,10,RGB(255,255,255),lpddsback);
		Draw_Text_GDI("Time:",10,30,RGB(255,255,255),lpddsback);
	}
		
	else
	{
		//boss levels
		sprintf(buffer,"BOSS---Lives: %d   Targets Hit: %d   Innocent Hit: %d", lives, target, innocent);
		Draw_Text_GDI(buffer,10,10,RGB(255,255,255),lpddsback);
		Draw_Text_GDI("Time:",10,30,RGB(255,255,255),lpddsback);
	}
		

	
	if(show_PowerUp == 1)
	{
		sprintf(buffer,"You Got Powerup!");
		Draw_Text_GDI(buffer,10,50,RGB(255,255,255),lpddsback);
	}

	
	
 


	//calculate extra gas depeleted
	
	float minus = gasDown - loseGas;
	Gas(minus);

	if(currLevel % 2 == 0)
		Boss_Level();

}


// display joystick and buttons 0-7
DInput_Read_Joystick();


// flip the surfaces
DDraw_Flip();
//}// pause
// sync to 30 fps
Wait_Clock(30);

if(DMusic_Status_MIDI(0)==MIDI_STOPPED)
DMusic_Play(0);
// return success
return(1);

} // end Game_Main
////////////////////////////////////////////////////////////////////



//* GAME_SPLASH *///////////////////////////////////////////////////
int Game_Splash(void){

   
   BOB splash[6];
   BOB background;
   Set_Palette(bitmap8bit.palette);
   Unload_Bitmap_File(&bitmap8bit);

   // index 0 is where the backgournd gui pic is loaded
   // 1-6 are positions wehre hte "selected" bitmap will be painted
   gui[0].x = 100;
   gui[0].y = 50;
   gui[0].type = 1;
   gui[1].x = 162;
   gui[1].y = 105;
   gui[1].type = 1;
   
   for(int k =2;k<10;k++){
       gui[k].x = 162;
       gui[k].y = 40+k*66;
       gui[k].type = 0;
   }


   //load and play music
   DMusic_Load_MIDI("sounds\\INTRO.mid");
   dm_perf->SetGlobalParam(GUID_PerfMasterTempo, &tempo, sizeof(float));
   DMusic_Play(0);

   // load the background
   Load_Bitmap_File(&bitmap8bit, "gui\\Menu256.bmp");
   if (!Create_BOB(&menu,0,0,500,500,1,
              BOB_ATTR_VISIBLE | 
BOB_ATTR_MULTI_FRAME,DDSCAPS_SYSTEMMEMORY))
       return(0);
   Load_Frame_BOB(&menu,&bitmap8bit,0,0,0,1);
   Unload_Bitmap_File(&bitmap8bit);

   
   // select bar
   Load_Bitmap_File(&bitmap8bit, "gui\\menuSelect256.bmp");
   if (!Create_BOB(&menuSelect,0,0,383,68,1,
              BOB_ATTR_VISIBLE | 
BOB_ATTR_MULTI_FRAME,DDSCAPS_SYSTEMMEMORY))
       return(0);
   Load_Frame_BOB(&menuSelect,&bitmap8bit,0,0,0,1);


   Load_Bitmap_File(&bitmap8bit, "gui\\SplashScreen256.bmp");
   if (!Create_BOB(&background,0,0,800,600,1,
              BOB_ATTR_VISIBLE | 
BOB_ATTR_MULTI_FRAME,DDSCAPS_SYSTEMMEMORY))
       return(0);
   Load_Frame_BOB(&background,&bitmap8bit,0,0,0,1);
   Unload_Bitmap_File(&bitmap8bit);

   Load_Bitmap_File(&bitmap8bit, "gui\\logo256a.bmp");
   for(k =0;k<3;k++){
       if (!Create_BOB(&splash[k],0,0,322,300,1,
              BOB_ATTR_VISIBLE | 
BOB_ATTR_MULTI_FRAME,DDSCAPS_SYSTEMMEMORY))
           return(0);
       Load_Frame_BOB(&splash[k],&bitmap8bit,0,k*322,0,1);
   }

   for(k =1;k<4;k++){
       if (!Create_BOB(&splash[k+2],0,0,322,300,1,
              BOB_ATTR_VISIBLE | 
BOB_ATTR_MULTI_FRAME,DDSCAPS_SYSTEMMEMORY))
           return(0);
       Load_Frame_BOB(&splash[k+2],&bitmap8bit,0,(k-1)*322,300,1);
   }

   Set_Palette(bitmap8bit.palette);
   Unload_Bitmap_File(&bitmap8bit);
   PALETTEENTRY black = {0,0,0,PC_NOCOLLAPSE};
   Set_Palette_Entry(135,&black);
   splash[0].x=200;
   splash[0].y=-400;
   splash[1].x=200;
   splash[1].y=550;

   for(k = 2;k<6;k++){
       splash[k].x=200;
       splash[k].y=100;
   }


   int width, height;      // used to extract bob info
   char title[17]="P R E S E N T S";
   
   HDC             hdc;    
   HWND hwnd = 0; //globally track hwnd



   int x0=0,y0=0; // used to compute the upper left and lower right corners of each object
   int scene = 0;
   int loop = 0; // number loop
   int timer =0;


   BOB_PTR object_ptr; // the object that is going to be rendered

   while(loop==0){
       if(KEY_DOWN(VK_RETURN)){
           loop=1;
       }

       switch(scene){
           
       case 0:{
           DDraw_Fill_Surface(lpddsback, 255);
           if(splash[0].y<=100)
               splash[0].y++;
           if(splash[1].y>=100)
               splash[1].y--;
           
           width = splash[0].width;
           height = splash[0].height;

           for(k = 5;k>=0;k--){
               if(k <=1)
               {
                   object_ptr = &splash[k];
                   object_ptr->x = splash[k].x;
                   object_ptr->y = splash[k].y;
                   Draw_BOB(object_ptr, lpddsback);
               }
               else if(k>1&&splash[0].y==101)
               {
                   object_ptr = &splash[k];
                   object_ptr->x = splash[k].x;
                   object_ptr->y = splash[k].y;
                   Draw_BOB(object_ptr, lpddsback);
                   timer++;
               }// end if
           }// end for

           if(timer > 240){
               scene++;
               timer =0;
           }
       }break;// end of case 0

       case 1:{
           // build up the messages
           DDraw_Fill_Surface(lpddsback, 255);
           SetMapMode(hdc, MM_TEXT);
           
           // create a 75 pixel tall Times New Roman font!
           HFONT fontArialBig = CreateFont(
               85, // height (negative means use character heights
               0, // width (0 means use default)
               0, // escapement (0 means none)
               0, // orientation (0 means none)
               FW_BOLD, // "boldness" of font
               TRUE, // italic?  true or false
               FALSE, // underline? true or false
               FALSE, // strikeout?  true or false
               ANSI_CHARSET, // desired character set
               OUT_TT_PRECIS, // output precision - use TrueType only
               CLIP_DEFAULT_PRECIS, // clip precision - use default
               PROOF_QUALITY, // proof quality
               DEFAULT_PITCH | FF_DONTCARE, // pitch and family
               "Arial" // name of font face desired
               );
           hdc = GetDC(hwnd);
           SetBkMode(hdc,TRANSPARENT);

           // select our font
           // store the old font to be restored
   
           SetTextColor(hdc,RGB(250,5,0));
           SelectObject(hdc,fontArialBig);
           TextOut(hdc,100,200,title,strlen(title));
           DeleteObject(fontArialBig);
           ReleaseDC(hwnd,hdc);

           if(timer > 180){
               scene++;
               timer =0;
           }
           
           timer++;
       }break;

       case 2:{
           DDraw_Fill_Surface(lpddsback, 0);

           if(x0<400){
               x0+=3;
               y0+=2;
           }

           object_ptr = &background;
           object_ptr->x = x0;
           object_ptr->y = y0;
           Zoom_Bit(object_ptr, lpddsback);

           if(timer > 360&& x0>400){
               loop=1;
               timer =0;
           }

           timer++;
       }break;
   
       default: break;
       }// end of switch
   
       DDraw_Flip();

       // sync to 30 fps
       Wait_Clock(30);
   }// end while

   for(k =0;k<6;k++)
       Destroy_BOB(&splash[k]);
       
   Destroy_BOB(&background);
   Load_Bitmap_File(&bitmap8bit, "gui\\menuSelect256.bmp");
   Set_Palette(bitmap8bit.palette);
   Unload_Bitmap_File(&bitmap8bit);


}//end Splash_Screen
////////////////////////////////////////////////////////////////



//* LOAD_LEVEL *////////////////////////////////////////////////
int Load_Level(int level)
{
   //this loads the level or the boss level
   //a certain level will correspond to the switch statement

   /*cases:
       0 is Game Menu
       1 is Level 1 etc....
   */


	//UNIVERSAL OBJECTS
	   show_PowerUp = 0; //reset the powerup
	   fire_PowerUp = 0;
	// initialize gas meter
	gas.state = 1;   // turn it on
	gas.num_verts = 4; 
	gas.x0 = 55; // position it
	gas.y0 = 45;
	gas.xv = 0;
	gas.yv = 0;
	gas.color = 15; // green
	gas.vlist = new VERTEX2DF [gas.num_verts];

	for (int index = 0; index < gas.num_verts; index++)
		gas.vlist[index] = gas_vertices[index];

	Load_Bitmap_File(&bitmap8bit, "BITMAPS\\TREE.BMP");
   if (!Create_BOB(&tree,0,0,80,100,1, BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_FRAME,DDSCAPS_SYSTEMMEMORY,0,16))
               return(0);
   Load_Frame_BOB(&tree,&bitmap8bit,0,0,0,1);
   Unload_Bitmap_File(&bitmap8bit);
       
   Load_Bitmap_File(&bitmap8bit, "BITMAPS\\POTHOLE.BMP");
   if (!Create_BOB(&pothole,0,0,124,28,1,
              BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_FRAME,DDSCAPS_SYSTEMMEMORY,0,16))
           return(0);
   Load_Frame_BOB(&pothole,&bitmap8bit,0,0,0,1);
   Unload_Bitmap_File(&bitmap8bit);

   Load_Bitmap_File(&bitmap8bit, "BITMAPS\\PKCAR.BMP");
   if (!Create_BOB(&car,0,0,65,100,1,
              BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_FRAME,DDSCAPS_SYSTEMMEMORY,0,16))
       return(0);
   Load_Frame_BOB(&car,&bitmap8bit,0,0,0,1);
   Unload_Bitmap_File(&bitmap8bit);
       
   Load_Bitmap_File(&bitmap8bit, "BITMAPS\\MUTANTFLY.BMP");
   if (!Create_BOB(&fly,0,0,64,64,2,
              BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_FRAME,DDSCAPS_SYSTEMMEMORY,0,16))
       return(0);
   Load_Frame_BOB(&fly,&bitmap8bit,0,0,0,1);
   Load_Frame_BOB(&fly,&bitmap8bit,1,64,0,1);
   Unload_Bitmap_File(&bitmap8bit);
		
	

	//GENERAL PEOPLE
	
	Load_Bitmap_File(&bitmap8bit, "BITMAPS\\ADULT.BMP");
   if (!Create_BOB(&adult1,0,0,64,64,2,
              BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_FRAME,DDSCAPS_SYSTEMMEMORY,0,16))
       return(0);
   Load_Frame_BOB(&adult1,&bitmap8bit,0,0,0,BITMAP_EXTRACT_MODE_CELL);
   Load_Frame_BOB(&adult1,&bitmap8bit,1,64,0,1);
   Unload_Bitmap_File(&bitmap8bit);
   
   
   Load_Bitmap_File(&bitmap8bit, "BITMAPS\\GUY.BMP");
   if (!Create_BOB(&adult2,0,0,64,70,2,
              BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_FRAME,DDSCAPS_SYSTEMMEMORY,0,16))
       return(0);
   Load_Frame_BOB(&adult2,&bitmap8bit,0,0,0,1);
   Load_Frame_BOB(&adult2,&bitmap8bit,1,64,0,1);
   Unload_Bitmap_File(&bitmap8bit);

  
   Load_Bitmap_File(&bitmap8bit, "BITMAPS\\OLD.BMP");
   if (!Create_BOB(&old,0,0,64,64,2,
              BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_FRAME,DDSCAPS_SYSTEMMEMORY,0,16))
       return(0);
   Load_Frame_BOB(&old,&bitmap8bit,0,0,0,1);
   Load_Frame_BOB(&old,&bitmap8bit,1,64,0,1);
   Unload_Bitmap_File(&bitmap8bit);

   
		
	

	//reset killed flags
	for( index = 0; index < NUM_OBJECTS_UNIVERSE; index++)
	{
		itemsFlag[index] = 0;
	}



	//CHOOSE LEVEL STUFF
   switch(level){
   case 0:{
           
		//do nothing

          }break;
   case 1:{    //Load Level 1

		//color_scan = 2; //color for grass
	
		//LOAD JEEP
	Load_Bitmap_File(&bitmap8bit, "BITMAPS\\JEEP.BMP");
	if (!Create_BOB(&jeep,0,0,280,376,1,
               BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_FRAME,DDSCAPS_SYSTEMMEMORY,0,16))
		return(0);
	Load_Frame_BOB(&jeep,&bitmap8bit,0,0,0,1);
	 Unload_Bitmap_File(&bitmap8bit);

	   //unique buildings
			Load_Bitmap_File(&bitmap8bit, "BITMAPS\\BUILDING1.BMP");
           if (!Create_BOB(&building1,0,0,400,100,1,
                      BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_FRAME,DDSCAPS_SYSTEMMEMORY,0,16))
               return(0);
           Load_Frame_BOB(&building1,&bitmap8bit,0,0,0,1);
           Unload_Bitmap_File(&bitmap8bit);
       
           Load_Bitmap_File(&bitmap8bit, "BITMAPS\\BUILDING2.BMP");
           if (!Create_BOB(&building2,0,0,300,200,1,
                      BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_FRAME,DDSCAPS_SYSTEMMEMORY,0,16))
               return(0);
           Load_Frame_BOB(&building2,&bitmap8bit,0,0,0,1);
           Unload_Bitmap_File(&bitmap8bit);
		

		//rich people and machine gun powerup
	   
			Load_Bitmap_File(&bitmap8bit, "BITMAPS\\RICH PERSON.BMP");
           if (!Create_BOB(&levelPerson,0,0,64,64,2,
                      BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_FRAME,DDSCAPS_SYSTEMMEMORY,0,16))
               return(0);
           Load_Frame_BOB(&levelPerson,&bitmap8bit,0,0,0,1);
		   Load_Frame_BOB(&levelPerson,&bitmap8bit,1,64,0,1);
           Unload_Bitmap_File(&bitmap8bit);

       
           Load_Bitmap_File(&bitmap8bit, "BITMAPS\\POWERGUN.BMP");
           if (!Create_BOB(&powerup,0,0,64,64,1,
                      BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_FRAME,DDSCAPS_SYSTEMMEMORY,0,16))
               return(0);
           Load_Frame_BOB(&powerup,&bitmap8bit,0,0,0,1);
		

		   		// load in texture maps
			if (!Create_BOB(&textures,0,0,800, 600 ,8, 
               BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_FRAME,DDSCAPS_SYSTEMMEMORY,0,16))
			return(0);

			for(int index = 0; index < 8;index++){
				sprintf(buffer,"LEVEL1\\ROAD%d.BMP",index);
		   	 Load_Bitmap_File(&bitmap24bit, buffer);
			

			// load each texture bitmap into the texture BOB object
			//for (int index = 0; index < NUM_TEXTURES; index++)
				Load_Frame_BOB(&textures,&bitmap24bit,index,0,0,1); 
			// unload the texture map bitmap
			Unload_Bitmap_File(&bitmap24bit);
			}

	
// setup the world from a file!
FILE * pFile;

   pFile = fopen ("Level1\\Level1.txt","r");
    // world-x, y
	fscanf (pFile, "%d", &world_x);
	fscanf (pFile, "%d", &world_y);
	
	// jeep_x,y
	fscanf (pFile, "%d", &jeep_x);
	fscanf (pFile, "%d", &jeep_y);
	
	// gas and gasdrop
	fscanf (pFile, "%d", &fuel);
	fscanf (pFile, "%f", &gasDown);
	// color scan
	fscanf (pFile, "%d", &color_scan);
	   
	//MAX_UNIVERSE_X  MAX_UNIVERSE_Y
	fscanf (pFile, "%d", &MAX_UNIVERSE_X);
	fscanf (pFile, "%d", &MAX_UNIVERSE_Y);
	//BOARD_WIDTH BOARD_HEIGHT
	 fscanf (pFile, "%d", &BOARD_WIDTH);
	fscanf (pFile, "%d", &BOARD_HEIGHT);

for(int k =0; k <NUM_OBJECTS_UNIVERSE;k++){
	fscanf (pFile, "%d", &worldObjects[k].type);
	fscanf (pFile, "%d", &worldObjects[k].x);
	fscanf (pFile, "%d", &worldObjects[k].y);
	worldObjects[k].currF = 0;
	itemsFlag[k] = 0;
   }

   fclose (pFile);

	//load, delete and play music
			DMusic_Delete_MIDI(0);
			DMusic_Load_MIDI("LEVEL1\\LEVEL1.mid");
			dm_perf->SetGlobalParam(GUID_PerfMasterTempo, &tempo, sizeof(float));
			DMusic_Play(0);
          }break;
   case 2:{
	   		//LOAD JEEP
	Load_Bitmap_File(&bitmap8bit, "BITMAPS\\JEEPR.BMP");
	if (!Create_BOB(&jeep,0,0,376,280,1,
               BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_FRAME,DDSCAPS_SYSTEMMEMORY,0,16))
		return(0);
	Load_Frame_BOB(&jeep,&bitmap8bit,0,0,0,1);
	 Unload_Bitmap_File(&bitmap8bit);

	   //unique buildings
			Load_Bitmap_File(&bitmap8bit, "BITMAPS\\BOSSBLD1.BMP");
           if (!Create_BOB(&building1,0,0,800,100,1,
                      BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_FRAME,DDSCAPS_SYSTEMMEMORY,0,16))
               return(0);
           Load_Frame_BOB(&building1,&bitmap8bit,0,0,0,1);
           Unload_Bitmap_File(&bitmap8bit);
       
           Load_Bitmap_File(&bitmap8bit, "BITMAPS\\BOSSBLD2.BMP");
           if (!Create_BOB(&building2,0,0,800,100,1,
                      BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_FRAME,DDSCAPS_SYSTEMMEMORY,0,16))
               return(0);
           Load_Frame_BOB(&building2,&bitmap8bit,0,0,0,1);
           Unload_Bitmap_File(&bitmap8bit);
		

		//rich people and machine gun powerup
	   
			Load_Bitmap_File(&bitmap8bit, "BITMAPS\\COP.BMP");
           if (!Create_BOB(&levelPerson,0,0,64,64,2,
                      BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_FRAME,DDSCAPS_SYSTEMMEMORY,0,16))
               return(0);
           Load_Frame_BOB(&levelPerson,&bitmap8bit,0,0,0,1);
		   Load_Frame_BOB(&levelPerson,&bitmap8bit,1,64,0,1);
           Unload_Bitmap_File(&bitmap8bit);
		Load_Bitmap_File(&bitmap8bit, "BITMAPS\\INMATE.BMP");
           if (!Create_BOB(&adult1,0,0,64,64,2,
                      BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_FRAME,DDSCAPS_SYSTEMMEMORY,0,16))
               return(0);
           Load_Frame_BOB(&adult1,&bitmap8bit,0,0,0,1);
		   Load_Frame_BOB(&adult1,&bitmap8bit,1,64,0,1);
           Unload_Bitmap_File(&bitmap8bit);
       
           Load_Bitmap_File(&bitmap8bit, "BITMAPS\\POWERGUN.BMP");
           if (!Create_BOB(&powerup,0,0,64,64,1,
                      BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_FRAME,DDSCAPS_SYSTEMMEMORY,0,16))
               return(0);
           Load_Frame_BOB(&powerup,&bitmap8bit,0,0,0,1);
		

		   		// load in texture maps
			if (!Create_BOB(&textures,0,0,800, 600 ,6, 
               BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_FRAME,DDSCAPS_SYSTEMMEMORY,0,16))
			return(0);

			for(int index = 0; index < 6;index++){
				sprintf(buffer,"BOSS1\\BOSS1%d.BMP",index);
		   	 Load_Bitmap_File(&bitmap24bit, buffer);
			

			// load each texture bitmap into the texture BOB object
			//for (int index = 0; index < NUM_TEXTURES; index++)
				Load_Frame_BOB(&textures,&bitmap24bit,index,0,0,1); 
			// unload the texture map bitmap
			Unload_Bitmap_File(&bitmap24bit);
			}

	
// setup the world from a file!
FILE * pFile;

   pFile = fopen ("BOSS1\\Level2.txt","r");
    // world-x, y
	fscanf (pFile, "%d", &world_x);
	fscanf (pFile, "%d", &world_y);
	
	// jeep_x,y
	fscanf (pFile, "%d", &jeep_x);
	fscanf (pFile, "%d", &jeep_y);
	
	// gas and gasdrop
	fscanf (pFile, "%d", &fuel);
	fscanf (pFile, "%f", &gasDown);
	// color scan
	fscanf (pFile, "%d", &color_scan);
	   
	//MAX_UNIVERSE_X  MAX_UNIVERSE_Y
	fscanf (pFile, "%d", &MAX_UNIVERSE_X);
	fscanf (pFile, "%d", &MAX_UNIVERSE_Y);
	//BOARD_WIDTH BOARD_HEIGHT
	 fscanf (pFile, "%d", &BOARD_WIDTH);
	fscanf (pFile, "%d", &BOARD_HEIGHT);

for(int k =0; k <NUM_OBJECTS_UNIVERSE;k++){
	fscanf (pFile, "%d", &worldObjects[k].type);
	fscanf (pFile, "%d", &worldObjects[k].x);
	fscanf (pFile, "%d", &worldObjects[k].y);
	worldObjects[k].currF = 0;
	itemsFlag[k] = 0;
   }

   fclose (pFile);

	//load, delete and play music
			DMusic_Delete_MIDI(0);
			DMusic_Load_MIDI("BOSS1\\BOSS1.mid");
			dm_perf->SetGlobalParam(GUID_PerfMasterTempo, &tempo, sizeof(float));
			DMusic_Play(0);

			if(DSound_Status_Sound(4) != DSBSTATUS_PLAYING)
               DSound_Play(4,0);
          }break;
   case 3:{
	  // kill texture memory
		Destroy_BOB(&textures);

		//LOAD JEEP
	Load_Bitmap_File(&bitmap8bit, "BITMAPS\\JEEPR.BMP");
	if (!Create_BOB(&jeep,0,0,376,280,1,
               BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_FRAME,DDSCAPS_SYSTEMMEMORY,0,16))
		return(0);
	Load_Frame_BOB(&jeep,&bitmap8bit,0,0,0,1);
	 Unload_Bitmap_File(&bitmap8bit);
	 //jeep_x = world_x +320;
	 //jeep_y = world_y +440;
Destroy_BOB(&building1);
Destroy_BOB(&building2);
	   //unique buildings
			Load_Bitmap_File(&bitmap8bit, "BITMAPS\\BUILDING3.BMP");
           if (!Create_BOB(&building1,0,0,200,300,1,
                      BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_FRAME,DDSCAPS_SYSTEMMEMORY,0,16))
               return(0);
           Load_Frame_BOB(&building1,&bitmap8bit,0,0,0,1);
           Unload_Bitmap_File(&bitmap8bit);
       
           Load_Bitmap_File(&bitmap8bit, "BITMAPS\\BUILDING4.BMP");
           if (!Create_BOB(&building2,0,0,420,440,1,
                      BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_FRAME,DDSCAPS_SYSTEMMEMORY,0,16))
               return(0);
           Load_Frame_BOB(&building2,&bitmap8bit,0,0,0,1);
           Unload_Bitmap_File(&bitmap8bit);
		

		//rich people and machine gun powerup
	   
			Load_Bitmap_File(&bitmap8bit, "BITMAPS\\FARMER.BMP");
           if (!Create_BOB(&levelPerson,0,0,64,70,2,
                      BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_FRAME,DDSCAPS_SYSTEMMEMORY,0,16))
               return(0);
           Load_Frame_BOB(&levelPerson,&bitmap8bit,0,0,0,1);
		   Load_Frame_BOB(&levelPerson,&bitmap8bit,1,64,0,1);
           Unload_Bitmap_File(&bitmap8bit);

           Load_Bitmap_File(&bitmap8bit, "BITMAPS\\POWERROCKET.BMP");
           if (!Create_BOB(&powerup,0,0,64,64,1,
                      BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_FRAME,DDSCAPS_SYSTEMMEMORY,0,16))
               return(0);
           Load_Frame_BOB(&powerup,&bitmap8bit,0,0,0,1);
		

		   		// load in texture maps
			if (!Create_BOB(&textures,0,0,800, 600 ,8, 
               BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_FRAME,DDSCAPS_SYSTEMMEMORY,0,16))
			return(0);

			for(int index = 0; index < 8;index++){
				sprintf(buffer,"Level2\\ROAD%d.BMP",index);
		   	 Load_Bitmap_File(&bitmap8bit, buffer);
			

			// load each texture bitmap into the texture BOB object
			//for (int index = 0; index < NUM_TEXTURES; index++)
				Load_Frame_BOB(&textures,&bitmap8bit,index,0,0,1); 
			// unload the texture map bitmap
			Unload_Bitmap_File(&bitmap8bit);
			}

		//load, delete and play music
			DMusic_Delete_MIDI(0);
			DMusic_Load_MIDI("LEVEL2\\LEVEL2.mid");
			dm_perf->SetGlobalParam(GUID_PerfMasterTempo, &tempo, sizeof(float));
			DMusic_Play(0);

	   FILE * pFile;

   pFile = fopen ("LEVEL2\\Level3.txt","r");
    // world-x, y
	fscanf (pFile, "%d", &world_x);
	fscanf (pFile, "%d", &world_y);
	
	// jeep_x,y
	fscanf (pFile, "%d", &jeep_x);
	fscanf (pFile, "%d", &jeep_y);
	
	// gas and gasdrop
	fscanf (pFile, "%d", &fuel);
	fscanf (pFile, "%f", &gasDown);
	// color scan
	fscanf (pFile, "%d", &color_scan);
	   
	//MAX_UNIVERSE_X  MAX_UNIVERSE_Y
	fscanf (pFile, "%d", &MAX_UNIVERSE_X);
	fscanf (pFile, "%d", &MAX_UNIVERSE_Y);
	//BOARD_WIDTH BOARD_HEIGHT
	 fscanf (pFile, "%d", &BOARD_WIDTH);
	fscanf (pFile, "%d", &BOARD_HEIGHT);

for(int k =0; k <NUM_OBJECTS_UNIVERSE;k++){
	fscanf (pFile, "%d", &worldObjects[k].type);
	fscanf (pFile, "%d", &worldObjects[k].x);
	fscanf (pFile, "%d", &worldObjects[k].y);
	worldObjects[k].currF = 0;
	itemsFlag[k] = 0;
   }

   fclose (pFile);

          }break;
   case 4:{
			
	   	Load_Bitmap_File(&bitmap8bit, "BITMAPS\\JEEP.BMP");
	if (!Create_BOB(&jeep,0,0,280,376,1,
               BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_FRAME,DDSCAPS_SYSTEMMEMORY,0,16))
		return(0);
	Load_Frame_BOB(&jeep,&bitmap8bit,0,0,0,1);
	 Unload_Bitmap_File(&bitmap8bit);
	   //unique buildings
			Load_Bitmap_File(&bitmap8bit, "BITMAPS\\BOSS2BLD1.BMP");
           if (!Create_BOB(&building1,0,0,600,200,1,
                      BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_FRAME,DDSCAPS_SYSTEMMEMORY,0,16))
               return(0);
           Load_Frame_BOB(&building1,&bitmap8bit,0,0,0,1);
           Unload_Bitmap_File(&bitmap8bit);
       
           Load_Bitmap_File(&bitmap8bit, "BITMAPS\\BOSS2BLD2.BMP");
           if (!Create_BOB(&building2,0,0,600,400,1,
                      BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_FRAME,DDSCAPS_SYSTEMMEMORY,0,16))
               return(0);
           Load_Frame_BOB(&building2,&bitmap8bit,0,0,0,1);
           Unload_Bitmap_File(&bitmap8bit);


		//rich people and machine gun powerup
	   
			Load_Bitmap_File(&bitmap8bit, "BITMAPS\\NERD.BMP");
           if (!Create_BOB(&levelPerson,0,0,64,64,2,
                      BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_FRAME,DDSCAPS_SYSTEMMEMORY,0,16))
               return(0);
           Load_Frame_BOB(&levelPerson,&bitmap8bit,0,0,0,1);
		   Load_Frame_BOB(&levelPerson,&bitmap8bit,1,64,0,1);
           Unload_Bitmap_File(&bitmap8bit);
	    
           Load_Bitmap_File(&bitmap8bit, "BITMAPS\\POWERROCKET.BMP");
           if (!Create_BOB(&powerup,0,0,64,64,1,
                      BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_FRAME,DDSCAPS_SYSTEMMEMORY,0,16))
               return(0);
           Load_Frame_BOB(&powerup,&bitmap8bit,0,0,0,1);

	
// setup the world from a file!
FILE * pFile;

   pFile = fopen ("BOSS2\\Level4.txt","r");
    // world-x, y
	fscanf (pFile, "%d", &world_x);
	fscanf (pFile, "%d", &world_y);
	
	// jeep_x,y
	fscanf (pFile, "%d", &jeep_x);
	fscanf (pFile, "%d", &jeep_y);
	
	// gas and gasdrop
	fscanf (pFile, "%d", &fuel);
	fscanf (pFile, "%f", &gasDown);
	// color scan
	fscanf (pFile, "%d", &color_scan);
	   
	//MAX_UNIVERSE_X  MAX_UNIVERSE_Y
	fscanf (pFile, "%d", &MAX_UNIVERSE_X);
	fscanf (pFile, "%d", &MAX_UNIVERSE_Y);
	//BOARD_WIDTH BOARD_HEIGHT
	 fscanf (pFile, "%d", &BOARD_WIDTH);
	fscanf (pFile, "%d", &BOARD_HEIGHT);

for(int k =0; k <NUM_OBJECTS_UNIVERSE;k++){
	fscanf (pFile, "%d", &worldObjects[k].type);
	fscanf (pFile, "%d", &worldObjects[k].x);
	fscanf (pFile, "%d", &worldObjects[k].y);
	worldObjects[k].currF = 0;
	itemsFlag[k] = 0;
   }

   fclose (pFile);

	//load, delete and play music
			DMusic_Delete_MIDI(0);
			DMusic_Load_MIDI("BOSS2\\BOSS2.mid");
			dm_perf->SetGlobalParam(GUID_PerfMasterTempo, &tempo, sizeof(float));
			DMusic_Play(0);
			//DSound_Delete_All_Sounds();
			DSound_Stop_All_Sounds();
			//DSound_Delete_All_Sounds();
			if(DSound_Status_Sound(5) != DSBSTATUS_PLAYING)
               DSound_Play(5,0);

			
	   Load_Bitmap_File(&bitmap8bit, "BOSS2\\BOSSHEAD2.BMP");
   if (!Create_BOB(&bossHead,0,0,318,244,2,
              BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_FRAME,DDSCAPS_SYSTEMMEMORY,0,16))
       return(0);
   Load_Frame_BOB(&bossHead,&bitmap8bit,0,0,0,BITMAP_EXTRACT_MODE_CELL);
   Load_Frame_BOB(&bossHead,&bitmap8bit,1,318,0,1);
   Unload_Bitmap_File(&bitmap8bit);
   Load_Animation_BOB(&bossHead,0,2,projectile_anim[0]);
			Set_Animation_BOB(&bossHead, 0);
			Set_Anim_Speed_BOB(&bossHead, 3);
		
	   // kill texture memory
		Destroy_BOB(&textures);

		//LOAD JEEP
		/*
	Load_Bitmap_File(&bitmap8bit, "BITMAPS\\JEEP.BMP");
	if (!Create_BOB(&jeep,0,0,400,400,1,
               BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_FRAME,DDSCAPS_SYSTEMMEMORY,0,16))
		return(0);
	Load_Frame_BOB(&jeep,&bitmap8bit,0,0,0,1);
	 Unload_Bitmap_File(&bitmap8bit);
	 */
          }break;
   case 5:{

		
	   // kill texture memory
		Destroy_BOB(&textures);

		//LOAD JEEP
	Load_Bitmap_File(&bitmap8bit, "BITMAPS\\JEEP.BMP");
	if (!Create_BOB(&jeep,0,0,280,376,1,
               BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_FRAME,DDSCAPS_SYSTEMMEMORY,0,16))
		return(0);
	Load_Frame_BOB(&jeep,&bitmap8bit,0,0,0,1);
	 Unload_Bitmap_File(&bitmap8bit);
	 jeep_x = world_x +320;
	 jeep_y = world_y +440;

	   //unique buildings
			Load_Bitmap_File(&bitmap8bit, "BITMAPS\\BUILDING5.BMP");
           if (!Create_BOB(&building1,0,0,400,200,1,
                      BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_FRAME,DDSCAPS_SYSTEMMEMORY,0,16))
               return(0);
           Load_Frame_BOB(&building1,&bitmap8bit,0,0,0,1);
           Unload_Bitmap_File(&bitmap8bit);
       
           Load_Bitmap_File(&bitmap8bit, "BITMAPS\\BUILDING3.BMP");
           if (!Create_BOB(&building2,0,0,200,300,1,
                      BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_FRAME,DDSCAPS_SYSTEMMEMORY,0,16))
               return(0);
           Load_Frame_BOB(&building2,&bitmap8bit,0,0,0,1);
           Unload_Bitmap_File(&bitmap8bit);
		

		//rich people and machine gun powerup
	   
			Load_Bitmap_File(&bitmap8bit, "BITMAPS\\HOOKER.BMP");
           if (!Create_BOB(&levelPerson,0,0,64,64,2,
                      BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_FRAME,DDSCAPS_SYSTEMMEMORY,0,16))
               return(0);
           Load_Frame_BOB(&levelPerson,&bitmap8bit,0,0,0,1);
		   Load_Frame_BOB(&levelPerson,&bitmap8bit,1,64,0,1);
           Unload_Bitmap_File(&bitmap8bit);

       
           Load_Bitmap_File(&bitmap8bit, "BITMAPS\\POWERFIRE.BMP");
           if (!Create_BOB(&powerup,0,0,64,64,1,
                      BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_FRAME,DDSCAPS_SYSTEMMEMORY,0,16))
               return(0);
           Load_Frame_BOB(&powerup,&bitmap8bit,0,0,0,1);
		

		   	   		// load in texture maps
			if (!Create_BOB(&textures,0,0,800, 600 ,8, 
               BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_FRAME,DDSCAPS_SYSTEMMEMORY,0,16))
			return(0);
			
			// load in texture maps
		   	 Load_Bitmap_File(&bitmap8bit, "LEVEL3\\ROADL3.BMP");
			// create the texture bob
			if (!Create_BOB(&textures,0,0,800, 600 ,10, 
               BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_FRAME,DDSCAPS_SYSTEMMEMORY,0,16))
			return(0);

			// load each texture bitmap into the texture BOB object
			for (int index = 0; index < NUM_TEXTURES; index++)
				Load_Frame_BOB(&textures,&bitmap8bit,index,0,0,1); 
			// unload the texture map bitmap
			Unload_Bitmap_File(&bitmap8bit);
           

		//load, delete and play music
			DMusic_Delete_MIDI(0);
			DMusic_Load_MIDI("LEVEL3\\LEVEL3.mid");
			dm_perf->SetGlobalParam(GUID_PerfMasterTempo, &tempo, sizeof(float));
			DMusic_Play(0);

FILE * pFile;

   pFile = fopen ("LEVEL3\\Level5.txt","r");
    // world-x, y
	fscanf (pFile, "%d", &world_x);
	fscanf (pFile, "%d", &world_y);
	
	// jeep_x,y
	fscanf (pFile, "%d", &jeep_x);
	fscanf (pFile, "%d", &jeep_y);
	
	// gas and gasdrop
	fscanf (pFile, "%d", &fuel);
	fscanf (pFile, "%f", &gasDown);
	// color scan
	fscanf (pFile, "%d", &color_scan);
	   
	//MAX_UNIVERSE_X  MAX_UNIVERSE_Y
	fscanf (pFile, "%d", &MAX_UNIVERSE_X);
	fscanf (pFile, "%d", &MAX_UNIVERSE_Y);
	//BOARD_WIDTH BOARD_HEIGHT
	 fscanf (pFile, "%d", &BOARD_WIDTH);
	fscanf (pFile, "%d", &BOARD_HEIGHT);

for(int k =0; k <NUM_OBJECTS_UNIVERSE;k++){
	fscanf (pFile, "%d", &worldObjects[k].type);
	fscanf (pFile, "%d", &worldObjects[k].x);
	fscanf (pFile, "%d", &worldObjects[k].y);
	worldObjects[k].currF = 0;
	itemsFlag[k] = 0;
   }

   fclose (pFile);

          }break;
   case 6:{
Load_Bitmap_File(&bitmap8bit, "BOSS3\\BOSSHEAD3.BMP");
   if (!Create_BOB(&bossHead,0,0,64,64,2,
              BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_FRAME,DDSCAPS_SYSTEMMEMORY,0,16))
       return(0);
   Load_Frame_BOB(&bossHead,&bitmap8bit,0,0,0,BITMAP_EXTRACT_MODE_CELL);
   Load_Frame_BOB(&bossHead,&bitmap8bit,1,64,0,1);
   Unload_Bitmap_File(&bitmap8bit);
   Load_Animation_BOB(&bossHead,0,2,projectile_anim[0]);
			Set_Animation_BOB(&bossHead, 0);
			Set_Anim_Speed_BOB(&bossHead, 3);

	   
	   // kill texture memory
		Destroy_BOB(&textures);

		//LOAD JEEP
	Load_Bitmap_File(&bitmap8bit, "BITMAPS\\JEEPR.BMP");
	if (!Create_BOB(&jeep,0,0,376,280,1,
               BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_FRAME,DDSCAPS_SYSTEMMEMORY,0,16))
		return(0);
	Load_Frame_BOB(&jeep,&bitmap8bit,0,0,0,1);
	 Unload_Bitmap_File(&bitmap8bit);
	 jeep_x = world_x +320;
	 jeep_y = world_y +440;

		Load_Bitmap_File(&bitmap8bit, "BITMAPS\\HORSE.BMP");
   if (!Create_BOB(&levelPerson,0,0,64,64,2,
              BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_FRAME,DDSCAPS_SYSTEMMEMORY,0,16))
       return(0);
   Load_Frame_BOB(&levelPerson,&bitmap8bit,0,0,0,BITMAP_EXTRACT_MODE_CELL);
   Load_Frame_BOB(&levelPerson,&bitmap8bit,1,64,0,1);
   Unload_Bitmap_File(&bitmap8bit);
   
   
   Load_Bitmap_File(&bitmap8bit, "BITMAPS\\RICH PERSON.BMP");
   if (!Create_BOB(&adult1,0,0,64,70,2,
              BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_FRAME,DDSCAPS_SYSTEMMEMORY,0,16))
       return(0);
   Load_Frame_BOB(&adult1,&bitmap8bit,0,0,0,1);
   Load_Frame_BOB(&adult1,&bitmap8bit,1,64,0,1);
   Unload_Bitmap_File(&bitmap8bit);


			// load in texture maps
		   	 Load_Bitmap_File(&bitmap8bit, "BOSS3\\ROADB3.BMP");
			// create the texture bob
			if (!Create_BOB(&textures,0,0,800, 600 ,10, 
               BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_FRAME,DDSCAPS_SYSTEMMEMORY,0,16))
			return(0);

			// load each texture bitmap into the texture BOB object
			for (int index = 0; index < NUM_TEXTURES; index++)
				Load_Frame_BOB(&textures,&bitmap8bit,index,0,0,1); 
			// unload the texture map bitmap

		  Unload_Bitmap_File(&bitmap8bit);
		  

			


FILE * pFile;

   pFile = fopen ("BOSS3\\Level6.txt","r");
    // world-x, y
	fscanf (pFile, "%d", &world_x);
	fscanf (pFile, "%d", &world_y);
	
	// jeep_x,y
	fscanf (pFile, "%d", &jeep_x);
	fscanf (pFile, "%d", &jeep_y);
	
	// gas and gasdrop
	fscanf (pFile, "%d", &fuel);
	fscanf (pFile, "%f", &gasDown);
	// color scan
	fscanf (pFile, "%d", &color_scan);
	   
	//MAX_UNIVERSE_X  MAX_UNIVERSE_Y
	fscanf (pFile, "%d", &MAX_UNIVERSE_X);
	fscanf (pFile, "%d", &MAX_UNIVERSE_Y);
	//BOARD_WIDTH BOARD_HEIGHT
	 fscanf (pFile, "%d", &BOARD_WIDTH);
	fscanf (pFile, "%d", &BOARD_HEIGHT);

for(int k =0; k <NUM_OBJECTS_UNIVERSE;k++){
	fscanf (pFile, "%d", &worldObjects[k].type);
	fscanf (pFile, "%d", &worldObjects[k].x);
	fscanf (pFile, "%d", &worldObjects[k].y);
	worldObjects[k].currF = 0;
	itemsFlag[k] = 0;
   }

   fclose (pFile);

   DSound_Stop_All_Sounds();
			DSound_Set_Volume(6,100);
			//DSound_Delete_All_Sounds();
			if(DSound_Status_Sound(6) != DSBSTATUS_PLAYING)
               DSound_Play(6,0);

		//load, delete and play music
			DMusic_Delete_MIDI(0);
			DMusic_Load_MIDI("BOSS3\\BOSS3.mid");
			dm_perf->SetGlobalParam(GUID_PerfMasterTempo, &tempo, sizeof(float));
			DMusic_Play(0);

          }break;
   case 7:{
		
	   show_PowerUp = 0; //reset the powerup

	   
	   // kill texture memory
		Destroy_BOB(&textures);

		//LOAD JEEP
	Load_Bitmap_File(&bitmap8bit, "BITMAPS\\JEEPR.BMP");
	if (!Create_BOB(&jeep,0,0,376,280,1,
               BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_FRAME,DDSCAPS_SYSTEMMEMORY,0,16))
		return(0);
	Load_Frame_BOB(&jeep,&bitmap8bit,0,0,0,1);
	 Unload_Bitmap_File(&bitmap8bit);
	 jeep_x = world_x +320;
	 jeep_y = world_y +440;


		//unique buildings
			Load_Bitmap_File(&bitmap8bit, "BITMAPS\\BUILDING6.BMP");
           if (!Create_BOB(&building1,0,0,800,100,1,
                      BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_FRAME,DDSCAPS_SYSTEMMEMORY,0,16))
               return(0);
           Load_Frame_BOB(&building1,&bitmap8bit,0,0,0,1);
           Unload_Bitmap_File(&bitmap8bit);
       
           Load_Bitmap_File(&bitmap8bit, "BITMAPS\\BUILDING7.BMP");
           if (!Create_BOB(&building2,0,0,350,200,1,
                      BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_FRAME,DDSCAPS_SYSTEMMEMORY,0,16))
               return(0);
           Load_Frame_BOB(&building2,&bitmap8bit,0,0,0,1);
           Unload_Bitmap_File(&bitmap8bit);
		

		//rich people and machine gun powerup
	   
			Load_Bitmap_File(&bitmap8bit, "BITMAPS\\SUIT.BMP");
           if (!Create_BOB(&levelPerson,0,0,64,64,2,
                      BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_FRAME,DDSCAPS_SYSTEMMEMORY,0,16))
               return(0);
           Load_Frame_BOB(&levelPerson,&bitmap8bit,0,0,0,1);
		   Load_Frame_BOB(&levelPerson,&bitmap8bit,1,64,0,1);
           Unload_Bitmap_File(&bitmap8bit);

       
           Load_Bitmap_File(&bitmap8bit, "BITMAPS\\POWERLASER.BMP");
           if (!Create_BOB(&powerup,0,0,64,64,1,
                      BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_FRAME,DDSCAPS_SYSTEMMEMORY,0,16))
               return(0);
           Load_Frame_BOB(&powerup,&bitmap8bit,0,0,0,1);
		


			// load in texture maps
		   	 Load_Bitmap_File(&bitmap8bit, "LEVEL4\\ROADL4.BMP");
			// create the texture bob
			if (!Create_BOB(&textures,0,0,800, 600 ,10, 
               BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_FRAME,DDSCAPS_SYSTEMMEMORY,0,16))
			return(0);

			// load each texture bitmap into the texture BOB object
			for (int index = 0; index < NUM_TEXTURES; index++)
				Load_Frame_BOB(&textures,&bitmap8bit,index,0,0,1); 
			// unload the texture map bitmap
			Unload_Bitmap_File(&bitmap8bit);
    
		//load, delete and play music
			DMusic_Delete_MIDI(0);
			DMusic_Load_MIDI("LEVEL4\\LEVEL4.mid");
			dm_perf->SetGlobalParam(GUID_PerfMasterTempo, &tempo, sizeof(float));
			DMusic_Play(0);
			
FILE * pFile;

   pFile = fopen ("LEVEL4\\Level7.txt","r");
    // world-x, y
	fscanf (pFile, "%d", &world_x);
	fscanf (pFile, "%d", &world_y);
	
	// jeep_x,y
	fscanf (pFile, "%d", &jeep_x);
	fscanf (pFile, "%d", &jeep_y);
	
	// gas and gasdrop
	fscanf (pFile, "%d", &fuel);
	fscanf (pFile, "%f", &gasDown);
	// color scan
	fscanf (pFile, "%d", &color_scan);
	   
	//MAX_UNIVERSE_X  MAX_UNIVERSE_Y
	fscanf (pFile, "%d", &MAX_UNIVERSE_X);
	fscanf (pFile, "%d", &MAX_UNIVERSE_Y);
	//BOARD_WIDTH BOARD_HEIGHT
	 fscanf (pFile, "%d", &BOARD_WIDTH);
	fscanf (pFile, "%d", &BOARD_HEIGHT);

for(int k =0; k <NUM_OBJECTS_UNIVERSE;k++){
	fscanf (pFile, "%d", &worldObjects[k].type);
	fscanf (pFile, "%d", &worldObjects[k].x);
	fscanf (pFile, "%d", &worldObjects[k].y);
	worldObjects[k].currF = 0;
	itemsFlag[k] = 0;
   }

   fclose (pFile);

          }break;
   case 8:{
		
	   Load_Bitmap_File(&bitmap8bit, "BOSS4\\BOSSHEAD4.BMP");
   if (!Create_BOB(&bossHead,0,0,64,64,2,
              BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_FRAME,DDSCAPS_SYSTEMMEMORY,0,16))
       return(0);
   Load_Frame_BOB(&bossHead,&bitmap8bit,0,0,0,BITMAP_EXTRACT_MODE_CELL);
   Load_Frame_BOB(&bossHead,&bitmap8bit,1,64,0,1);
   	Load_Animation_BOB(&bossHead,0,2,projectile_anim[0]);
			Set_Animation_BOB(&bossHead, 0);
			Set_Anim_Speed_BOB(&bossHead, 3);

   Unload_Bitmap_File(&bitmap8bit);
	   
	   // kill texture memory
		Destroy_BOB(&textures);


		//LOAD JEEP
	Load_Bitmap_File(&bitmap8bit, "BITMAPS\\JEEP.BMP");
	if (!Create_BOB(&jeep,0,0,280,376,1,
               BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_FRAME,DDSCAPS_SYSTEMMEMORY,0,16))
		return(0);
	Load_Frame_BOB(&jeep,&bitmap8bit,0,0,0,1);
	 Unload_Bitmap_File(&bitmap8bit);
	 jeep_x = world_x +320;
	 jeep_y = world_y +440;


		//LOAD TERRORIST
		Load_Bitmap_File(&bitmap8bit, "BITMAPS\\TERRORIST.BMP");
           if (!Create_BOB(&levelPerson,0,0,200,300,2,
                      BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_FRAME,DDSCAPS_SYSTEMMEMORY,0,16))
               return(0);
           Load_Frame_BOB(&levelPerson,&bitmap8bit,0,0,0,1);
		   Load_Frame_BOB(&levelPerson,&bitmap8bit,1,200,0,1);
           Unload_Bitmap_File(&bitmap8bit);


		   Load_Bitmap_File(&bitmap8bit, "BITMAPS\\POWERLASER.BMP");
           if (!Create_BOB(&powerup,0,0,64,64,1,
                      BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_FRAME,DDSCAPS_SYSTEMMEMORY,0,16))
               return(0);
           Load_Frame_BOB(&powerup,&bitmap8bit,0,0,0,1);

		   //the world
	
			// load in texture maps
		   	 Load_Bitmap_File(&bitmap8bit, "BOSS4\\ROADB4.BMP");
			// create the texture bob
			if (!Create_BOB(&textures,0,0,800, 600 ,10, 
               BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_FRAME,DDSCAPS_SYSTEMMEMORY,0,16))
			return(0);

			// load each texture bitmap into the texture BOB object
			for (int index = 0; index < NUM_TEXTURES; index++)
				Load_Frame_BOB(&textures,&bitmap8bit,index,0,0,1); 
			// unload the texture map bitmap
			Unload_Bitmap_File(&bitmap8bit);

		
			
    
FILE * pFile;

   pFile = fopen ("BOSS4\\Level8.txt","r");
    // world-x, y
	fscanf (pFile, "%d", &world_x);
	fscanf (pFile, "%d", &world_y);
	
	// jeep_x,y
	fscanf (pFile, "%d", &jeep_x);
	fscanf (pFile, "%d", &jeep_y);
	
	// gas and gasdrop
	fscanf (pFile, "%d", &fuel);
	fscanf (pFile, "%f", &gasDown);
	// color scan
	fscanf (pFile, "%d", &color_scan);
	   
	//MAX_UNIVERSE_X  MAX_UNIVERSE_Y
	fscanf (pFile, "%d", &MAX_UNIVERSE_X);
	fscanf (pFile, "%d", &MAX_UNIVERSE_Y);
	//BOARD_WIDTH BOARD_HEIGHT
	 fscanf (pFile, "%d", &BOARD_WIDTH);
	fscanf (pFile, "%d", &BOARD_HEIGHT);

for(int k =0; k <NUM_OBJECTS_UNIVERSE;k++){
	fscanf (pFile, "%d", &worldObjects[k].type);
	fscanf (pFile, "%d", &worldObjects[k].x);
	fscanf (pFile, "%d", &worldObjects[k].y);
	worldObjects[k].currF = 0;
	itemsFlag[k] = 0;
   }

   fclose (pFile);
   	
   DSound_Stop_All_Sounds();
			DSound_Set_Volume(7,500);
			if(DSound_Status_Sound(7) != DSBSTATUS_PLAYING)
               DSound_Play(7,0);

		//load, delete and play music
			DMusic_Delete_MIDI(0);
			DMusic_Load_MIDI("BOSS4\\BOSS4.mid");
			dm_perf->SetGlobalParam(GUID_PerfMasterTempo, &tempo, sizeof(float));
			DMusic_Play(0);

          }break;

	case 9 : {

		//badger level
		show_PowerUp = 0; //reset the powerup

		Destroy_BOB(&textures);

		//LOAD JEEP
	Load_Bitmap_File(&bitmap8bit, "BITMAPS\\JEEP.BMP");
	if (!Create_BOB(&jeep,0,0,280,376,1,
               BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_FRAME,DDSCAPS_SYSTEMMEMORY,0,16))
		return(0);
	Load_Frame_BOB(&jeep,&bitmap8bit,0,0,0,1);
	 Unload_Bitmap_File(&bitmap8bit);
	 jeep_x = world_x +320;
	 jeep_y = world_y +440;

		Load_Bitmap_File(&bitmap8bit, "BONUS\\BADGER.BMP");
   if (!Create_BOB(&adult1,0,0,48,48,2,
              BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_FRAME,DDSCAPS_SYSTEMMEMORY,0,16))
       return(0);
   Load_Frame_BOB(&adult1,&bitmap8bit,0,0,0,BITMAP_EXTRACT_MODE_CELL);
   Load_Frame_BOB(&adult1,&bitmap8bit,1,48,0,1);
   Unload_Bitmap_File(&bitmap8bit);
   
   
   Load_Bitmap_File(&bitmap8bit, "BONUS\\PIKACHU.BMP");
   if (!Create_BOB(&adult2,0,0,56,44,2,
              BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_FRAME,DDSCAPS_SYSTEMMEMORY,0,16))
       return(0);
   Load_Frame_BOB(&adult2,&bitmap8bit,0,0,0,1);
   Load_Frame_BOB(&adult2,&bitmap8bit,1,56,0,1);
   Unload_Bitmap_File(&bitmap8bit);

Load_Bitmap_File(&bitmap8bit, "BONUS\\AOL.BMP");
   if (!Create_BOB(&old,0,0,64,64,2,
              BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_FRAME,DDSCAPS_SYSTEMMEMORY,0,16))
       return(0);
   Load_Frame_BOB(&old,&bitmap8bit,0,0,0,1);
   Load_Frame_BOB(&old,&bitmap8bit,1,64,0,1);
   Unload_Bitmap_File(&bitmap8bit);


			// load in texture maps
		   	 Load_Bitmap_File(&bitmap8bit, "BONUS\\BONUS.BMP");
			// create the texture bob
			if (!Create_BOB(&textures,0,0,800, 600 ,10, 
               BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_FRAME,DDSCAPS_SYSTEMMEMORY,0,16))
			return(0);

			// load each texture bitmap into the texture BOB object
			for (int index = 0; index < NUM_TEXTURES; index++)
				Load_Frame_BOB(&textures,&bitmap8bit,index,0,0,1); 
			// unload the texture map bitmap

		  Unload_Bitmap_File(&bitmap8bit);
		  

			


FILE * pFile;

   pFile = fopen ("BONUS\\BonusLevel.txt","r");
    // world-x, y
	fscanf (pFile, "%d", &world_x);
	fscanf (pFile, "%d", &world_y);
	
	// jeep_x,y
	fscanf (pFile, "%d", &jeep_x);
	fscanf (pFile, "%d", &jeep_y);
	
	// gas and gasdrop
	fscanf (pFile, "%d", &fuel);
	fscanf (pFile, "%f", &gasDown);
	// color scan
	fscanf (pFile, "%d", &color_scan);
	   
	//MAX_UNIVERSE_X  MAX_UNIVERSE_Y
	fscanf (pFile, "%d", &MAX_UNIVERSE_X);
	fscanf (pFile, "%d", &MAX_UNIVERSE_Y);
	//BOARD_WIDTH BOARD_HEIGHT
	 fscanf (pFile, "%d", &BOARD_WIDTH);
	fscanf (pFile, "%d", &BOARD_HEIGHT);

for(int k =0; k <NUM_OBJECTS_UNIVERSE;k++){
	fscanf (pFile, "%d", &worldObjects[k].type);
	fscanf (pFile, "%d", &worldObjects[k].x);
	fscanf (pFile, "%d", &worldObjects[k].y);
	worldObjects[k].currF = 0;
	itemsFlag[k] = 0;
   }

   fclose (pFile);

			}break;

   default:break;
   }// end of switch
	return(1);
	
	
}//end Load_Level
////////////////////////////////////////////////////////////////





//* POWERUP *///////////////////////////////////////////////////
void PowerUp(){
	
	
	object_ptr = &projectile;
	object_ptr->x = projectile.x;
	object_ptr->y = projectile.y;
	
	//if(DSound_Status_Sound(4) != DSBSTATUS_PLAYING)
             //DSound_Play(4,0);

	if(currLevel == 1 || currLevel == 5 || currLevel == 8||currLevel ==4)
	{
		if(projectile.y >= 0)
		{
			Animate_BOB(&projectile);
			projectile.y -= 10;
		}

		else
		{
			fire_PowerUp = 0;
			projectile.y = jeep.y;
			projectile.x = jeep.x + 30;
			//projectile.x +=30;
		}
	}
	else if(currLevel == 3 || currLevel == 7|| currLevel==2||currLevel==6)
	{
		if(projectile.x <= 800)//<=BOARD_WIDTH)
		{
			Animate_BOB(&projectile);
			projectile.x += 10;
		}

		else
		{
			fire_PowerUp = 0;
			projectile.y = jeep.y + 50;
			//projectile.y +=30;
			projectile.x = jeep.x;
		}
	}
	Draw_BOB(object_ptr, lpddsback);

}//end PowerUp
////////////////////////////////////////////////////////////////////




//* COLLISION_DETECTION *//////////////////////////////////////////
void Collision_Detection(int index, int type){

		scroll_speedx = 15;
				scroll_speedy = 15;

   if(Collision_BOBS(&jeep, &tree,150,150,tree.width,tree.height))
       {
           //lose gas
           loseGas += 0.0005;
           scroll_speedx = 3; //slowed down a lot
		   scroll_speedy = 3; //slowed down a lot
           if(DSound_Status_Sound(0) != DSBSTATUS_PLAYING)
             DSound_Play(0,0);
       }

   if(Collision_BOBS(&jeep, &pothole,150,150,pothole.width,pothole.height))
       {
       //lose gas
       loseGas += 0.0005;
       scroll_speedx = 3;
	   scroll_speedy = 3;
       if(DSound_Status_Sound(0) != DSBSTATUS_PLAYING)
         DSound_Play(0,0);
       }

   if(Collision_BOBS(&jeep, &car,150,150,64,64))
       {
       //lose gas
       loseGas += 0.0005;
       scroll_speedx = 3;
	   scroll_speedy = 3;
           if(DSound_Status_Sound(0) != DSBSTATUS_PLAYING)
             DSound_Play(0,0);
       }

	 if(Collision_BOBS(&jeep, &fly,150,150,64,64) && type == FLY)
       {
       if(itemsFlag[index] == 0)
           {  
		  hit += 1;
           score += 25;
			//lose gas
			itemsFlag[index] = 1; //you dead
           worldObjects[index].currF =1;
           loseGas += 0.0005;
           
           if(DSound_Status_Sound(2) != DSBSTATUS_PLAYING)
               DSound_Play(2,0);
		   

			
           }

       }
	   if(Collision_BOBS(&projectile, &fly,150,150,64,64) && type == FLY)
       {
       if(itemsFlag[index] == 0)
           {  
		   fire_PowerUp=0;
		  hit += 1;
           score += 25;
			//lose gas
			itemsFlag[index] = 1; //you dead
           worldObjects[index].currF =1;
           loseGas += 0.0005;
           
           if(DSound_Status_Sound(2) != DSBSTATUS_PLAYING)
               DSound_Play(2,0);
		   

			
           }

       }

   if(Collision_BOBS(&projectile, &adult2,150,150,64,64)   && type == ADULT2)
       {
		
       if(itemsFlag[index] == 0 && currLevel % 2 == 1)
           {
           fire_PowerUp = 0;//if killed by powerup
           hit += 1;
           score += 10;
           itemsFlag[index] = 1; //you dead
           worldObjects[index].currF =1;
           if(DSound_Status_Sound(2) != DSBSTATUS_PLAYING)
               DSound_Play(2,0);
           }
	   else if(itemsFlag[index] == 0 && currLevel % 2 == 0)
           {
           fire_PowerUp = 0;//if killed by powerup
           innocent+=1;
           itemsFlag[index] = 1; //you dead
           worldObjects[index].currF =1;
           if(DSound_Status_Sound(2) != DSBSTATUS_PLAYING)
               DSound_Play(2,0);
           }
       }
   if(Collision_BOBS(&projectile, &adult1,150,150,64,64)&&type == ADULT1)
       {
		
       if(itemsFlag[index] == 0 && currLevel % 2 == 1)
           {
           fire_PowerUp = 0;//if killed by powerup
           hit += 1;
           score += 10;
           itemsFlag[index] = 1; //you dead
           worldObjects[index].currF =1;
           if(DSound_Status_Sound(2) != DSBSTATUS_PLAYING)
               DSound_Play(2,0);
           }
	   else if(itemsFlag[index] == 0 && currLevel % 2 == 0)
           {
           fire_PowerUp = 0;//if killed by powerup
           innocent+=1;
           itemsFlag[index] = 1; //you dead
           worldObjects[index].currF =1;
           if(DSound_Status_Sound(2) != DSBSTATUS_PLAYING)
               DSound_Play(2,0);
           }
       }


   if(Collision_BOBS(&jeep, &adult1,150,150,64,64)   && type==ADULT1)
       {
   		
       if(itemsFlag[index] == 0 && currLevel % 2 == 1 && currLevel != 9)
           {
   
           hit += 1;
           score += 10;
           itemsFlag[index] = 1; //you dead
           worldObjects[index].currF =1;
           if(DSound_Status_Sound(2) != DSBSTATUS_PLAYING)
               DSound_Play(2,0);
           }
	   else if(itemsFlag[index] == 0 && currLevel % 2 == 0)
           {
   
           innocent+=1;
           itemsFlag[index] = 1; //you dead
           worldObjects[index].currF =1;
           if(DSound_Status_Sound(2) != DSBSTATUS_PLAYING)
               DSound_Play(2,0);
           } else if(itemsFlag[index] == 0 && currLevel == 9)
	   {
		   //kill badger
		   //badgerDead+=1;
		   itemsFlag[index] = 1;
		   worldObjects[index].currF =1;
		   if(DSound_Status_Sound(8) != DSBSTATUS_PLAYING)
               DSound_Play(8,0);

	   }
       }
   if(Collision_BOBS(&jeep, &adult2,150,150,64,64)    && type == ADULT2)
       {
   		
       if(itemsFlag[index] == 0 && currLevel % 2 == 1 && currLevel != 9)
           {
   
           hit += 1;
           score += 10;
           itemsFlag[index] = 1; //you dead
           worldObjects[index].currF =1;
           if(DSound_Status_Sound(2) != DSBSTATUS_PLAYING)
               DSound_Play(2,0);
           }
	   else if(itemsFlag[index] == 0 && currLevel % 2 == 0)
           {
   
           innocent+=1;
           itemsFlag[index] = 1; //you dead
           worldObjects[index].currF =1;
           if(DSound_Status_Sound(2) != DSBSTATUS_PLAYING)
               DSound_Play(2,0);
           } else if(itemsFlag[index] == 0 && currLevel == 9)
	   {
		   //kill badger
		   //badgerDead+=1;
		   itemsFlag[index] = 1;
		   worldObjects[index].currF =1;
		   if(DSound_Status_Sound(9) != DSBSTATUS_PLAYING)
               DSound_Play(9,0);

	   }
       }


 if(Collision_BOBS(&projectile, &old,150,150,64,64) && type == OLD)
       {
       if(itemsFlag[index] == 0 && currLevel != 9)
           {
		   fire_PowerUp = 0;//if killed by powerup
           hit += 1;
           score += 20;
           itemsFlag[index] = 1; //you dead
           worldObjects[index].currF =1;
           if(DSound_Status_Sound(1) != DSBSTATUS_PLAYING)
               DSound_Play(1,0);
           }
	   else if(itemsFlag[index] == 0 && currLevel == 9)
	   {
		   //kill badger
		   //badgerDead+=1;
		   itemsFlag[index] = 1;
		   worldObjects[index].currF =1;
		   if(DSound_Status_Sound(10) != DSBSTATUS_PLAYING)
               DSound_Play(10,0);

	   }
       }
DInput_Read_Keyboard();
   if(Collision_BOBS(&jeep, &building1,150,150,building1.width,building1.height))
           {  
           if(currLevel != 6)
			{
			//lose gas
           loseGas += 0.0005;
		   if(currLevel==1||currLevel==2||currLevel==4||currLevel == 5||currLevel==8){
           if(keyboard_state[DIK_UP]||keyboard_state[DIK_DOWN]||joy_state.lY!=0){
			   //jeep_y+=scroll_speedy;
			   scroll_speedy = 0;
			   scroll_speedx = 15;
		   }else if(keyboard_state[DIK_LEFT]||keyboard_state[DIK_RIGHT]||joy_state.lX!=0){
				scroll_speedx = 0;
				scroll_speedy = 15;
		   }
		   }else
		   if(keyboard_state[DIK_LEFT]||keyboard_state[DIK_RIGHT]||joy_state.lX!=0){
				scroll_speedx = 0;
				scroll_speedy = 15;
		   }else if(keyboard_state[DIK_UP]||keyboard_state[DIK_DOWN]||joy_state.lY!=0){
			   //jeep_y+=scroll_speedy;
			   scroll_speedy = 0;
			   scroll_speedx = 15;
		   }
           
           //if(DSound_Status_Sound(0) != DSBSTATUS_PLAYING)
             //  DSound_Play(0,0);
		   
			}
           }

   if(Collision_BOBS(&jeep, &building2,150,150,building2.width,building2.height))
           {  
           if(currLevel != 6)
			{
			//lose gas
           loseGas += 0.0005;
		   if(currLevel==1||currLevel==2||currLevel==4||currLevel == 5||currLevel==8){
           if(keyboard_state[DIK_UP]||keyboard_state[DIK_DOWN]||joy_state.lY!=0){
			   //jeep_y+=scroll_speedy;
			   scroll_speedy = 0;
			   scroll_speedx = 15;
		   }else if(keyboard_state[DIK_LEFT]||keyboard_state[DIK_RIGHT]||joy_state.lX!=0){
				scroll_speedx = 0;
				scroll_speedy = 15;
		   }
		   }else
		   if(keyboard_state[DIK_LEFT]||keyboard_state[DIK_RIGHT]||joy_state.lX!=0){
				scroll_speedx = 0;
				scroll_speedy = 15;
		   }else if(keyboard_state[DIK_UP]||keyboard_state[DIK_DOWN]||joy_state.lY!=0){
			   //jeep_y+=scroll_speedy;
			   scroll_speedx = 0;
			   scroll_speedy = 15;
		   }
           //if(DSound_Status_Sound(0) != DSBSTATUS_PLAYING)
             //  DSound_Play(0,0);
		  
			}
           }

   
   if(Collision_BOBS(&jeep, &levelPerson,150,150,64,64) && type == LEVELPERSON)
           {
           if(itemsFlag[index] == 0 && currLevel % 2 == 1)
               {  
		
               hit += 1;
               score += 35;
               itemsFlag[index] = 1; //you dead
               worldObjects[index].currF =1;
               if(DSound_Status_Sound(1) != DSBSTATUS_PLAYING)
                   DSound_Play(1,0);
               }
		   else if(itemsFlag[index] == 0 && currLevel % 2 == 0)
		   {
		
			   target+=1;
			   itemsFlag[index]=1;
			   worldObjects[index].currF=1;
			   if(DSound_Status_Sound(1) != DSBSTATUS_PLAYING)
                   DSound_Play(1,0);
		   }

           }  
     if(Collision_BOBS(&projectile, &levelPerson,150,150,64,64) && type == LEVELPERSON)
           {
           if(itemsFlag[index] == 0 && currLevel % 2 == 1)
               {  
			   fire_PowerUp = 0;//if killed by powerup
               hit += 1;
               score += 35;
               itemsFlag[index] = 1; //you dead
               worldObjects[index].currF =1;
               if(DSound_Status_Sound(1) != DSBSTATUS_PLAYING)
                   DSound_Play(1,0);
               }
		   else if(itemsFlag[index] == 0 && currLevel % 2 == 0)
		   {
			   fire_PowerUp = 0;
			   target+=1;
			   itemsFlag[index]=1;
			   worldObjects[index].currF=1;
			   if(DSound_Status_Sound(1) != DSBSTATUS_PLAYING)
                   DSound_Play(1,0);
		   }

           }  


   if(Collision_BOBS(&jeep, &powerup,150,150,64,64) && type == POWERUP)
    {
           //find out which level player is on
           //load appropriate powerup (USE A VARIABLE FOR FILE NAME)
           show_PowerUp = 1;
		   if(DSound_Status_Sound(3) != DSBSTATUS_PLAYING)
               DSound_Play(3,0);
           
		   if(currLevel == 1)
		   {
			   //BULLET
			Load_Bitmap_File(&bitmap8bit, "BITMAPS\\BULLET.BMP");
			if (!Create_BOB(&projectile,0,0,64,64,2,
                      BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_FRAME,DDSCAPS_SYSTEMMEMORY,0,16))
                   ;
			Load_Frame_BOB(&projectile,&bitmap8bit,0,0,0,1);
		   Load_Frame_BOB(&projectile,&bitmap8bit,1,64,0,1);
           Unload_Bitmap_File(&bitmap8bit);
		   }else if(currLevel == 2)
		   {
			   //BULLET
			Load_Bitmap_File(&bitmap8bit, "BITMAPS\\BULLETR.BMP");
			if (!Create_BOB(&projectile,0,0,64,64,2,
                      BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_FRAME,DDSCAPS_SYSTEMMEMORY,0,16))
                   ;
			Load_Frame_BOB(&projectile,&bitmap8bit,0,0,0,1);
		   Load_Frame_BOB(&projectile,&bitmap8bit,1,64,0,1);
           Unload_Bitmap_File(&bitmap8bit);
		   }
		   else if(currLevel ==3)
		   {
			//DUAL ROCKETS
			Load_Bitmap_File(&bitmap8bit, "BITMAPS\\ROCKETR.BMP");
			if (!Create_BOB(&projectile,0,0,120,48,2,
                      BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_FRAME,DDSCAPS_SYSTEMMEMORY,0,16))
                   ;
			Load_Frame_BOB(&projectile,&bitmap8bit,0,0,0,1);
		   Load_Frame_BOB(&projectile,&bitmap8bit,1,120,0,1);
           Unload_Bitmap_File(&bitmap8bit);
		   }else if(currLevel ==4)
		   {
			//DUAL ROCKETS
			Load_Bitmap_File(&bitmap8bit, "BITMAPS\\ROCKET.BMP");
			if (!Create_BOB(&projectile,0,0,48,120,2,
                      BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_FRAME,DDSCAPS_SYSTEMMEMORY,0,16))
                   ;
			Load_Frame_BOB(&projectile,&bitmap8bit,0,0,0,1);
		   Load_Frame_BOB(&projectile,&bitmap8bit,1,48,0,1);
           Unload_Bitmap_File(&bitmap8bit);
		   }
		   else if(currLevel >=5 && currLevel < 7)
		   {
			   //FIREBALL
			Load_Bitmap_File(&bitmap8bit, "BITMAPS\\FIREBALL.BMP");
			if (!Create_BOB(&projectile,0,0,64,64,2,
                      BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_FRAME,DDSCAPS_SYSTEMMEMORY,0,16))
                   ;
			Load_Frame_BOB(&projectile,&bitmap8bit,0,0,0,1);
		   Load_Frame_BOB(&projectile,&bitmap8bit,1,64,0,1);
           Unload_Bitmap_File(&bitmap8bit);
		   }
		   else if(currLevel == 7)
		   {
			 //LASER BEAM
			Load_Bitmap_File(&bitmap8bit, "BITMAPS\\LASERBEAM.BMP");
			if (!Create_BOB(&projectile,0,0,64,64,2,
                      BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_FRAME,DDSCAPS_SYSTEMMEMORY,0,16))
                   ;
			Load_Frame_BOB(&projectile,&bitmap8bit,0,0,0,1);
		   Load_Frame_BOB(&projectile,&bitmap8bit,1,64,0,1);
           Unload_Bitmap_File(&bitmap8bit);
		   }
		   else if(currLevel == 8)
		   {
			 //LASER BEAM
			Load_Bitmap_File(&bitmap8bit, "BITMAPS\\LASERBEAM2.BMP");
			if (!Create_BOB(&projectile,0,0,64,64,2,
                      BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_FRAME,DDSCAPS_SYSTEMMEMORY,0,16))
                   ;
			Load_Frame_BOB(&projectile,&bitmap8bit,0,0,0,1);
		   Load_Frame_BOB(&projectile,&bitmap8bit,1,64,0,1);
           Unload_Bitmap_File(&bitmap8bit);
		   }
		   // set the animation sequence for projectile
			Load_Animation_BOB(&projectile,0,2,projectile_anim[0]);
			Set_Animation_BOB(&projectile, 0);
			Set_Anim_Speed_BOB(&projectile, 3);
      }
   if(currLevel == 8)
   {
	   if(Collision_BOBS(&jeep, &levelPerson, 150, 150, 200, 300))
	   {
		   if(itemsFlag[index] == 0)
		   {
				//you die
				lives--;
				//reset level
				target = 0;
				loadFlag = 0;
				currLevel = 8;
		   }
	   }
	   if(Collision_BOBS(&projectile, &levelPerson, 64,64,200,300))
	   {
		   //you dead
		   if(itemsFlag[index] == 0)
		   {
			target+=1;
			fire_PowerUp = 0;
			itemsFlag[index] = 1;
			worldObjects[index].currF =1;
		   }

	   }
   }








}//end Collision_Detection
////////////////////////////////////////////////////////////////////



//* AI *////////////////////////////////////////////////////////////
void AI(int index){


#define WEST_BIT     1
#define EAST_BIT     2
#define NORTH_BIT    4 
#define SOUTH_BIT    8 


int dead = 0; //determine if anyone on viewport is dead (nothing to move)
int worldIndex = index;
static int Mode,  // current pattern opcode
		   operand; // current operand 

int move = 15;


if(itemsFlag[index] != 0)//determine if killed
	dead = 1; //this dude is dead



// test if it's time to process a new instruction
if (curr_pattern == NULL)//took out the index for testing purposes
   {
	
	person_pattern_index = rand()%NUM_PATTERNS;
	curr_pattern = patterns[person_pattern_index];
   
   
   person_ip = 0;
 
   // reset counter
   person_counter= 0;

   } // end if



//process next instruction if it's time
if (--person_counter <= 0 && dead == 0)
    {
    
    Mode  = curr_pattern[person_ip++];
    operand = curr_pattern[person_ip++];

    // test what the Mode is
    switch(Mode)
        {
        case M_E:
            {
            // set direction to east
            worldObjects[worldIndex].x += move;
  
            // set animation to east
            //Set_Animation_BOB(&bob,Mode);
            
            // set counter to instuction operand
            //person_counter[index] = operand;
			person_counter = operand;

            } break;

        case M_NE:
            {
            // set direction to northeast
			worldObjects[worldIndex].x += move;
            worldObjects[worldIndex].y -= move;
  
            // set animation to northeast
           // Set_Animation_BOB(&bob,Mode);
            
            // set counter to instuction operand
            //person_counter[index] = operand;
			person_counter = operand;

            } break;

        case M_N: 
            {
            // set direction to north
			worldObjects[worldIndex].y -= move;
  
            // set animation to north
            //Set_Animation_BOB(&bob,Mode);
            
            // set counter to instuction operand
            //person_counter[index] = operand;
			person_counter = operand;

            } break;            

        case M_NW:
            {
            // set direction to northwest
			worldObjects[worldIndex].x -= move;
			worldObjects[worldIndex].y -= move;
  
            // set animation to northwest
            //Set_Animation_BOB(&bob,Mode);
            
            // set counter to instuction operand
            //person_counter[index] = operand;
			person_counter = operand;

            } break;            

        case M_W: 
            {
            // set direction to west
			worldObjects[worldIndex].x -= move;
  
            // set animation to west
            //Set_Animation_BOB(&bob,Mode);
            
            // set counter to instuction operand
            //person_counter[index] = operand;
			person_counter = operand;
			 
            } break;            

        case M_SW:
            {
            // set direction to southwest
			worldObjects[worldIndex].x -= move;
			worldObjects[worldIndex].y += move;
  
            // set animation to southwest
            //Set_Animation_BOB(&bob,Mode);
            
            // set counter to instuction operand
            //person_counter[index] = operand;
			person_counter = operand;

            } break;            

        case M_S: 
            {
            // set direction to south
			worldObjects[worldIndex].y += move;
  
            // set animation to south
            //Set_Animation_BOB(&bob,Mode);
            
            // set counter to instuction operand
            //person_counter[index] = operand;
			person_counter = operand;

            } break;            

        case M_SE:
            {
            // set direction to southeast
			worldObjects[worldIndex].x += move;
			worldObjects[worldIndex].y += move;
  
            // set animation to southeast
            //Set_Animation_BOB(&bob,Mode);
            
            // set counter to instuction operand
            //person_counter[index] = operand;
			person_counter = operand;

            } break;            

        case M_STOP: 
            {
			//no move
            // set counter to instuction operand
            //person_counter[index] = operand;
			person_counter = operand;

            } break;

        case M_RAND:
            {
            // set counter to instuction operand
            //person_counter[index] = 0;
			person_counter = 0;

            } break;

        case M_JEEP_DIST:
             {
             // test distance between an object and jeep
             // if jeep is too close, then move away

             int dx = (jeep.x - worldObjects[worldIndex].x);
             int dy = (jeep.y - worldObjects[worldIndex].y);

             // test distance against minimum loneliness
             if (sqrt((dx*dx) + (dy*dy)) < RUN_AWAY)
                {
                // the skeleton needs to be pointed away frpm the player
                // this is a bit hard because we need to point the BOB
                // in 1 of 8 directions, instead of just giving him a velocity
                // to solve the problem well break it up into a dx and a dy and then
                // use a look up table to set everything up right
   
                int direction = 0; // the bit encoded direction

                // first east-west
                if (jeep.x > worldObjects[worldIndex].x)
                   direction|=EAST_BIT;
                else
                if (jeep.x < worldObjects[worldIndex].x)
                   direction|=WEST_BIT;
                
                // now north-south
                if (jeep.y > worldObjects[worldIndex].y)
                   direction|=SOUTH_BIT;
                else
                if (jeep.y < worldObjects[worldIndex].y)
                   direction|=NORTH_BIT;
          
                // test final direction, note this could be compressed into
                // another look up table, but this is simpler 
                switch(direction)
                        {
                        case WEST_BIT:
                            {
                            // set motion
							worldObjects[worldIndex].x -= move;
  
                            // set animation 
                            //Set_Animation_BOB(&bob,M_W);
            
                            // set counter to instuction operand
							//person_counter[index] = operand;
							person_counter = operand;
                            } break;

                        case EAST_BIT:     
                             {
                            // set motion
							worldObjects[worldIndex].x += move;
                           
                            // set animation 
                           // Set_Animation_BOB(&bob,M_E);
            
                            // set counter to instuction operand
							//person_counter[index] = operand;
							person_counter = operand;

                             } break;

                        case NORTH_BIT:     
                            {
                            // set motion
							worldObjects[worldIndex].y -= move;
  
                            // set animation 
                            //Set_Animation_BOB(&bob,M_N);
            
                             // set counter to instuction operand
							//person_counter[index] = operand;
							person_counter = operand;

                            } break;

                        case SOUTH_BIT:     
                            {
                            // set motion
							worldObjects[worldIndex].y += move;
  
                            // set animation 
                            //Set_Animation_BOB(&bob,M_S);
            
                             // set counter to instuction operand
							//person_counter[index] = operand;
							person_counter = operand;
                            } break;

                        case (NORTH_BIT | WEST_BIT):
                            {
                            // set motion
							worldObjects[worldIndex].x -= move;
                            worldObjects[worldIndex].y -= move;
  
                            // set animation 
                            //Set_Animation_BOB(&bob,M_NW);
            
                             // set counter to instuction operand
							//person_counter[index] = operand;
							person_counter = operand;
                            } break;

                        case (NORTH_BIT | EAST_BIT):
                            {
                            // set motion
							worldObjects[worldIndex].x += move;
							worldObjects[worldIndex].x -= move;
  
                            // set animation 
                            //Set_Animation_BOB(&bob,M_NE);
            
                             // set counter to instuction operand
							//person_counter[index] = operand;
							person_counter = operand;
                            } break;

                        case (SOUTH_BIT | WEST_BIT):
                            {
                            // set motion
							worldObjects[worldIndex].x -= move;
							worldObjects[worldIndex].y += move;
  
                            // set animation 
                            //Set_Animation_BOB(&bob,M_SW);
            
                             // set counter to instuction operand
							//person_counter[index] = operand;
							person_counter = operand;
                            } break;

                        case (SOUTH_BIT | EAST_BIT):
                            {
                            // set motion
							worldObjects[worldIndex].x += move;
							worldObjects[worldIndex].y += move;
  
                            // set animation 
                            //Set_Animation_BOB(&bob,M_SE);
            
                             // set counter to instuction operand
							//person_counter[index] = operand;
							person_counter = operand;
                            } break;

                        default: break;

                        } // end switch 
               
                } // end if

             } break;

        case M_END: 
            {
            // no motion

			person_pattern_index = rand()%NUM_PATTERNS;
			curr_pattern = patterns[person_pattern_index];
   
			person_ip = 0;
 
			person_counter= 0;

            } break;
        
        default: break;

        } // end switch

    } // end if

}//end AI
////////////////////////////////////////////////////////////////////


//* DRAW_OBJECTS *///////////////////////////////////////////////////////
void Draw_Objects(){

	int x0,x1,y0,y1,type, width,height;


	for (int index = 0; index < NUM_OBJECTS_UNIVERSE; index++)
    {
	
	// get the object position
    x0 = worldObjects[index].x;
    y0 = worldObjects[index].y;
 
    // now translate to screen coords, could have done above, but clearer
    x0 = x0 - world_x;
    y0 = y0 - world_y;


	// get type, so we can compute size
	type = worldObjects[index].type;

	
    // compute size

    switch(type)
          {
          case TREE:
          {
          // get size of this object
          width = tree.width;
          height = tree.height;
		  
          // point to it to render it easier without another switch
          object_ptr = &tree;
		  
		  
          
		  } break;
 
          case POTHOLE:        
          {
          // get size of this object
          width = pothole.width;
          height = pothole.height;
 
          // point to it to render it easier without another switch
          object_ptr = &pothole;

		  
          } break;
 
          case CAR:       
          {
          // get size of this object
          width = car.width;
          height = car.height;
 
          // point to it to render it easier without another switch
          object_ptr = &car;

		  
          } break;   
		  
		  case FLY:       
          {
          // get size of this object
          width = fly.width;
          height = fly.height;
 
          // point to it to render it easier without another switch
          object_ptr = &fly;
		  fly.curr_frame =worldObjects[index].currF;
			
		  

		 
          } break;  
	
		  case ADULT1:       
          {
          // get size of this object
          width = adult1.width;
          height = adult1.height;

          // point to it to render it easier without another switch
          object_ptr = &adult1;
		  adult1.curr_frame =worldObjects[index].currF;

		  
          } break;  

		  case ADULT2:       
          {
          // get size of this object
          width = adult2.width;
          height = adult2.height;
		
          // point to it to render it easier without another switch
          object_ptr = &adult2;
		  adult2.curr_frame =worldObjects[index].currF;
			

          } break;  

		  case OLD:       
          {
          // get size of this object
          width = old.width;
          height = old.height;
 
          // point to it to render it easier without another switch
          object_ptr = &old;
		  old.curr_frame =worldObjects[index].currF;


          } break;  

		  case BUILDING1:       
          {
          // get size of this object
          width = building1.width;
          height = building1.height;
 
          // point to it to render it easier without another switch
          object_ptr = &building1;

		  
          } break;  

		  case BUILDING2:       
          {
          // get size of this object
          width = building2.width;
          height = building2.height;
 
          // point to it to render it easier without another switch
          object_ptr = &building2;

		  
          } break;  

		  case LEVELPERSON:       
          {
          // get size of this object
          width = levelPerson.width;
          height = levelPerson.height;
 
          // point to it to render it easier without another switch
          object_ptr = &levelPerson;
		  levelPerson.curr_frame =worldObjects[index].currF;

		 
          } break;  

		  case POWERUP:       
          {
          // get size of this object
          width = powerup.width;
          height = powerup.height;
		  
		  if(show_PowerUp == 1)//prevent rendering
		  {
			  x0 = SCREEN_WIDTH + 100;
		  }
          // point to it to render it easier without another switch
          object_ptr = &powerup;

		  
          } break;  

          default: break;
 
          } // end switch

		
		x1 = x0 + width - 1;
		y1 = y0 + height - 1;

		// do clip
       if ((x0 >= SCREEN_WIDTH) || (x1 < 0) || (y0 >= SCREEN_HEIGHT) || (y1 < 0))
           continue; // don't need to render this one
       else
       {
		   // render this object
          object_ptr->x = x0;
          object_ptr->y = y0;
		  jeep.x = jeep_x - world_x;
		  jeep.y = jeep_y - world_y;
		  
		  if(fire_PowerUp == 0)
		  {
			projectile.x = jeep.x+30;
			projectile.y = jeep.y;
			
		  }

           //Artificial Intelligence
           if(type == FLY || type == ADULT1 || type == ADULT2 || type == OLD || type == LEVELPERSON)
           {
			   if(currLevel == 8 && type == LEVELPERSON)
				   Terrorist_AI(index); //if on last boss
			   else{
				//AI for movable objects
				if(itemsFlag[index]==0)
					AI(index);
			   }
               
           }
           
			x0 = worldObjects[index].x;
			y0 = worldObjects[index].y;
           
          Collision_Detection(index, type);
          Draw_BOB(object_ptr, lpddsback);
           
       }

   } // end for index

   
   //draw jeep
   x0=jeep_x-world_x;
   y0=jeep_y-world_y;
   width = jeep.width;
   height = jeep.height;
   object_ptr = &jeep;
   object_ptr->x = x0;//-world_x;
   object_ptr->y = y0;//-world_y;
  if(currLevel==1||currLevel==4||currLevel == 5||currLevel==8){
   Draw_Scaled_BOB(object_ptr, 105,140, lpddsback);
   }else{
   Draw_Scaled_BOB(object_ptr, 140,105, lpddsback);
   }

}//Draw_Objects
/////////////////////////////////////////////////////////////////////




//* Gas *////////////////////////////////////////////////////
void Gas(float minus){

Find_Bounding_Box_Poly2D(&gas, gas_min_x, gas_max_x, gas_min_y, gas_max_y);

int left = gas_max_x - gas_min_x;

if(left <= 1)
{
	loadFlag = 0;
	lives--;
}

if(lives == 0)
{
	gameOver = 1;
}

if(currLevel % 2 == 0 || currLevel == 9)
{
	sprintf(buffer, "%d", left);
	Draw_Text_GDI(buffer,50,30,RGB(255,255,255),lpddsback);//timer for boss
}
else
{
	Draw_Filled_Polygon2D(&gas, (UCHAR *)ddsd.lpSurface, ddsd.lPitch);//gas meter on levels
}

Scale_Polygon2D(&gas, minus, scalesize);//always scale

}//end Gas
/////////////////////////////////////////////////////////////////////



//* GAME_GUI */////////////////////////////////////////////////////
int Game_Gui(void){

bool basecase=true;
	
while(basecase == true)
{
	DDraw_Fill_Surface(lpddsback, 0);
	int keydown = 0;

	DInput_Read_Keyboard();
	DInput_Read_Joystick();

	if(keyboard_state[DIK_E])
		basecase=false;


	if (keyboard_state[DIK_UP]||joy_state.lY <0){	
		if(selected ==1)
			selected=7;
		selected--;
		
		while(keydown ==0)
		{
			DInput_Read_Keyboard();
			DInput_Read_Joystick();
			if ((keyboard_state[DIK_UP]&0x80)||joy_state.lY <0)
				keydown = 0;
			else 
				keydown =1;
		};
	}

	if (keyboard_state[DIK_DOWN]||joy_state.lY > 0){	
		if(selected==6)
			selected =0;
		selected++;
	
		while(keydown ==0) 
		{
			DInput_Read_Keyboard();
			DInput_Read_Joystick();
			if ((keyboard_state[DIK_DOWN]&0x80)||joy_state.lY > 0)
				keydown = 0;
			else 
				keydown =1;
		};
	}

	
	
if (keyboard_state[DIK_RETURN]||joy_state.rgbButtons[0]){
	switch(selected){
	case 1:{
		basecase = false;
		pause = 0;
		currLevel = 1;//first level
		loadFlag = 0;//load level
		lives = 7;
		hit = 0;
		score = 0;
		basecase = false;
		   }break;
	case 2:{
		pause = -1;
		   }break;
	case 3:{
		basecase = false;
		pause = 0;
		   }break;
	case 4:{
		Game_Shutdown();
		PostMessage(main_window_handle, WM_DESTROY,0,0);
		   }break;
	case 5:{
		basecase =false;
		pause = 2; //for Game_Main
		Load_Bitmap_File(&bitmap8bit, "GUI\\ABOUTSCREEN.BMP");
		if (!Create_BOB(&about,0,0,800,600,1,BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_FRAME,DDSCAPS_SYSTEMMEMORY,0,16))
			return(0);
		Load_Frame_BOB(&about,&bitmap8bit,0,0,0,1);
		Unload_Bitmap_File(&bitmap8bit); 
		   }break;
	case 6:{
		basecase = false;
		pause = 3; //for Game_Main
		Load_Bitmap_File(&bitmap8bit, "GUI\\HELP.BMP");
		if (!Create_BOB(&help,0,0,800,600,1,BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_FRAME,DDSCAPS_SYSTEMMEMORY,0,16))
			return(0);
		Load_Frame_BOB(&help,&bitmap8bit,0,0,0,1);
		Unload_Bitmap_File(&bitmap8bit); 
		   }break;
	default: break;
	}// end switch
}// end if


// draw the menu, it has own flip and wait statments
// since it is in a loop, no clear screen
  
int width = menu.width;
int height = menu.height;
object_ptr = &menu;
object_ptr->x = gui[0].x;
object_ptr->y = gui[0].y;
Draw_BOB(object_ptr, lpddsback);

width = menu.width;
height = menu.height;
object_ptr = &menuSelect;
object_ptr->x = gui[selected].x;
object_ptr->y = gui[selected].y;
Draw_BOB(object_ptr, lpddsback);



// flip the surfaces
DDraw_Flip();

// sync to 30 fps
Wait_Clock(30);

}// end while
	return(1);

}//end Game Gui
////////////////////////////////////////////////////////////////////


//* TERRORIST_AI() *//////////////////////////////////////////////////
void Terrorist_AI(int index)
{

	if(itemsFlag[index] == 0)
	{
		if(worldObjects[index].x > jeep_x)
			worldObjects[index].x -= 5;
		if(worldObjects[index].x < jeep_x)
			worldObjects[index].x += 5;
		if(worldObjects[index].y > jeep_y)
			worldObjects[index].y -= 5;
		if(worldObjects[index].y < jeep_y)
			worldObjects[index].y += 5;
	}
	
}//end Terrorist_AI
//////////////////////////////////////////////////////////////////////




//* GAME_MODE *//////////////////////////////////////////////////////
void Game_Mode(int mode)
{
	

	object_ptr = &gamemode;
	object_ptr->x = gamemode.x;
	object_ptr->y = gamemode.y;
	Draw_BOB(object_ptr, lpddsback);

	

	if(mode == 0)//GAME OVER
	{
		Load_Bitmap_File(&bitmap8bit, "GUI\\GAMEOVER.BMP");
		if (!Create_BOB(&gamemode,0,0,800,600,1,BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_FRAME,DDSCAPS_SYSTEMMEMORY,0,16))
				;
		Load_Frame_BOB(&gamemode,&bitmap8bit,0,0,0,1);
		Unload_Bitmap_File(&bitmap8bit);
	}

	if (keyboard_state[DIK_RETURN]||joy_state.rgbButtons[0]){
		if(mode == 0)
		{
			gameOver = 0;
			lives = 7;
			currLevel = 1;
			score =0;
			hit =0;
			Game_Gui();
		}
		else if(mode == 1)
		{
			if(currLevel == 8)
			{
				loadFlag = 0;
				currLevel++;
				levelComplete = 0;
				//load, delete and play music
				DMusic_Play(0);
			}
			else{
			loadFlag = 0;
			levelComplete = 0;
			currLevel++;
			}
		}
	}

	if(mode == 1)//LEVEL COMPLETE
	{
		
		if(currLevel == 1)
		{

			Load_Bitmap_File(&bitmap8bit, "GUI\\rivsplash.BMP");
			if (!Create_BOB(&gamemode,0,0,800,600,1,BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_FRAME,DDSCAPS_SYSTEMMEMORY,0,16))
				;
			Load_Frame_BOB(&gamemode,&bitmap8bit,0,0,0,1);
			Unload_Bitmap_File(&bitmap8bit);

			sprintf(buffer, "Riverhead Boss:  Riverhead Jail----Run over all guards without hitting inmates");
			Draw_Text_GDI(buffer,100,130,RGB(0,0,0),lpddsback);
			
		}
		else if(currLevel == 2)
		{
			

			Load_Bitmap_File(&bitmap8bit, "GUI\\SBSPLASH.BMP");
			if (!Create_BOB(&gamemode,0,0,800,600,1,BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_FRAME,DDSCAPS_SYSTEMMEMORY,0,16))
				;
			Load_Frame_BOB(&gamemode,&bitmap8bit,0,0,0,1);
			Unload_Bitmap_File(&bitmap8bit);

			sprintf(buffer, "Destination: Stony Brook");
			Draw_Text_GDI(buffer,100,130,RGB(0,0,0),lpddsback);

		}
		else if(currLevel == 3)
		{
			Load_Bitmap_File(&bitmap8bit, "GUI\\SBSPLASH.BMP");
			if (!Create_BOB(&gamemode,0,0,800,600,1,BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_FRAME,DDSCAPS_SYSTEMMEMORY,0,16))
				;
			Load_Frame_BOB(&gamemode,&bitmap8bit,0,0,0,1);
			Unload_Bitmap_File(&bitmap8bit);

			sprintf(buffer, "Stony Brook Boss: SBU----Find the slackers of McKenna's class");
			Draw_Text_GDI(buffer,100,130,RGB(0,0,0),lpddsback);

		}
		else if(currLevel == 4)
		{
			Load_Bitmap_File(&bitmap8bit, "GUI\\BELMONTSPLASH.BMP");
			if (!Create_BOB(&gamemode,0,0,800,600,1,BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_FRAME,DDSCAPS_SYSTEMMEMORY,0,16))
				;
			Load_Frame_BOB(&gamemode,&bitmap8bit,0,0,0,1);
			Unload_Bitmap_File(&bitmap8bit);

			sprintf(buffer, "Destination: Belmont");
			Draw_Text_GDI(buffer,100,130,RGB(0,0,0),lpddsback);
		}
		else if(currLevel == 5)
		{
			Load_Bitmap_File(&bitmap8bit, "GUI\\BELMONTSPLASH.BMP");
			if (!Create_BOB(&gamemode,0,0,800,600,1,BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_FRAME,DDSCAPS_SYSTEMMEMORY,0,16))
				;
			Load_Frame_BOB(&gamemode,&bitmap8bit,0,0,0,1);
			Unload_Bitmap_File(&bitmap8bit);

			sprintf(buffer, "Belmont Boss: Belmont Race Track--- Destroy all horses without hitting stablesmen");
			Draw_Text_GDI(buffer,100,130,RGB(0,0,0),lpddsback);
			
		}
		else if(currLevel == 6)
		{
			Load_Bitmap_File(&bitmap8bit, "GUI\\NYCSPLASH.BMP");
			if (!Create_BOB(&gamemode,0,0,800,600,1,BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_FRAME,DDSCAPS_SYSTEMMEMORY,0,16))
				;
			Load_Frame_BOB(&gamemode,&bitmap8bit,0,0,0,1);
			Unload_Bitmap_File(&bitmap8bit);

			sprintf(buffer, "Destination: New York");
			Draw_Text_GDI(buffer,100,130,RGB(0,0,0),lpddsback);
		}
		else if(currLevel == 7)
		{
			Load_Bitmap_File(&bitmap8bit, "GUI\\NYCSPLASH.BMP");
			if (!Create_BOB(&gamemode,0,0,800,600,1,BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_FRAME,DDSCAPS_SYSTEMMEMORY,0,16))
				;
			Load_Frame_BOB(&gamemode,&bitmap8bit,0,0,0,1);
			Unload_Bitmap_File(&bitmap8bit);

			sprintf(buffer, "NYC Boss: Midtown Tunne---Destroy the Terrorists in their Cars!!");
			Draw_Text_GDI(buffer,100,130,RGB(0,0,0),lpddsback);

		}
		else if(currLevel == 8)
		{
			Load_Bitmap_File(&bitmap8bit, "GUI\\GAMECOMPLETE.BMP");
			if (!Create_BOB(&gamemode,0,0,800,600,1,BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_FRAME,DDSCAPS_SYSTEMMEMORY,0,16))
				;
			Load_Frame_BOB(&gamemode,&bitmap8bit,0,0,0,1);
			Unload_Bitmap_File(&bitmap8bit);
			
			//load, delete and play music
			DMusic_Delete_MIDI(0);
			DMusic_Load_MIDI("SOUNDS\\GAMECOMPLETE.mid");
			dm_perf->SetGlobalParam(GUID_PerfMasterTempo, &tempo, sizeof(float));
			DMusic_Play(0);

		}


	
	}

}//end Game_Mode
////////////////////////////////////////////////////////////////////



//* BOSS LEVEL *///////////////////////////////////////////////////
void Boss_Level(){

	//timer is checked in gas function



	if(currLevel%2 == 0 && currLevel!=8)
	{
		if(innocent == 10)
		{
			innocent = 0;
			target = 0;
			lives--;
			loadFlag = 0;
		}
		else if(target == 20)
		{
			target = 0;
			innocent = 0;
			levelComplete = 1;
		}
	}

	if(currLevel == 8)
	{
		if(target == 17)
			levelComplete = 1;
	}





}//end Boss Level
///////////////////////////////////////////////////////////////////


/****************END OF FILE******************************/
/*******DEATH RUN LONG ISLAND BY BASH KONTROLLER**********/
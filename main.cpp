/**************PONG VIDEO GAME**************/
/**************Using SDL Library************/
// specific headers
#include "SDL.h"
#include "SDL_opengl.h"
#include "SDL_mixer.h"
#include <iostream>

//integers
int paddle_width = 30;
int paddle_height = 116;

int width = 32;
int height = 34;

int srcX = 0;
int srcY = 0;

//player's coord
int p1_x = 0;
int p1_y = 0;

//CPU's coord
int p2_x = 570;
int p2_y = 0;

//ball's coord
int ball_x = 300;
int ball_y = 200;

//arrow menu coord
int arrow_x = 180;
int arrow_y = 210;

//arrow options coord
int O_arrow_x = 85;
int O_arrow_y = 160;

//score
int PLAYERscore = 0;
int CPUscore = 0;

int checkScore;
int difficulty = 0; //1 - Easy, 2 - Medium, 3 - Hard

void Title(SDL_Surface* screen);
void Draw_CPU(SDL_Surface* screen);
void Draw_Ball(SDL_Surface* screen);
int checkWin(SDL_Surface*,SDL_Surface*,SDL_Surface* screen);
void startNew(SDL_Surface* screen);
bool winner; //false if you lose, true if you win
bool gameStart = false;
bool pressedRight = false;
bool at_the_bottom = false;
bool pressedDown = false;

int dir = 1;

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination ) {
    //Make a temporary rectangle to hold the offsets
    SDL_Rect offset;
    //Give the offsets to the rectangle
    offset.x = x;
    offset.y = y;

    //Blit the surface
    SDL_BlitSurface( source, NULL, destination, &offset );
}

void draw_sprite(int srcX, int srcY,
  			 int dstX, int dstY,
				 int width, int height,
				 SDL_Surface* source, SDL_Surface* dest)
{
	SDL_Rect src;
	src.x = srcX;
	src.y = srcY;
	src.w = width;
	src.h = height;

	SDL_Rect dst;
	dst.x = dstX;
	dst.y = dstY;
	dst.w = width;
	dst.h = height;

	SDL_BlitSurface(source, &src, dest, &dst);
}



//start of the program
int main( int argc, char* args[] )
{
  //initialize SDL
  SDL_Init(SDL_INIT_VIDEO);
  Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) ;

  SDL_Surface *screen = NULL;
  SDL_Surface *User_Paddle = NULL;
  SDL_Surface *background = NULL;
  SDL_Surface* title = NULL;
  SDL_Surface* arrow = NULL;
  SDL_Surface* line = NULL;
  SDL_Surface* player1 = NULL;
  SDL_Surface* cpu = NULL;
  SDL_Surface* score1 = NULL;
  SDL_Surface* score2 = NULL;
  SDL_Surface* options = NULL;
  
  //The music that will be played 
  Mix_Music *music = NULL; 
  Mix_Chunk *point = NULL;
	  
	
  //Caption of the window
  SDL_WM_SetCaption( "Pong", NULL );

  //Size of the window
  screen = SDL_SetVideoMode(600,400,32, SDL_SWSURFACE );

  //Load files
  User_Paddle = SDL_LoadBMP("paddle.bmp");
  background = SDL_LoadBMP("background.bmp");
  title = SDL_LoadBMP("title.bmp");
  arrow = SDL_LoadBMP("arrow.bmp");
  line = SDL_LoadBMP("line.bmp");
  player1 = SDL_LoadBMP("player1.bmp");
  cpu = SDL_LoadBMP("cpu.bmp");
  options = SDL_LoadBMP("options.bmp");

  //Load the music 
  music = Mix_LoadMUS( "Excite Bike.wav" );
  point = Mix_LoadWAV("point.wav");
  

  //to block out background color of arrow
  SDL_SetColorKey(arrow,SDL_SRCCOLORKEY,SDL_MapRGB(arrow->format,0,0,0));


  //Handles the main loop
  bool isRunning = true;

  //used to move player while holding down key
  bool keys[323] = {false};

  //For handling with event
  SDL_Event event;

x:
   //Main game loop
  while ( isRunning )
    {
		
      //EVENTS
      if ( SDL_PollEvent(&event) )
	{
	  //if the window was closed
	  if ( event.type == SDL_QUIT )
	    {
	      isRunning = false;
	    }

	  //If a button was released and the button is escape
	  if ( event.type == SDL_KEYDOWN  )
	    {
		  keys[event.key.keysym.sym] = true;

		  if(event.key.keysym.sym == SDLK_DOWN)
			 arrow_y = 240;
			if(event.key.keysym.sym == SDLK_UP)
			 arrow_y = 210;
			if(event.key.keysym.sym == SDLK_RETURN && arrow_y == 210){
				gameStart = true;
				break;
			}
			if(event.key.keysym.sym == SDLK_RETURN && arrow_y == 240)
				break;

			if(arrow_y > 240)
				arrow_y = 240;
			if(arrow_y < 210)
				arrow_y = 210;

		  if(event.key.keysym.sym == SDLK_ESCAPE)
			  SDL_Quit();
	    }

	}
	  

	  //If there is no music playing 
	  if( Mix_PlayingMusic() == 0 ) { 
		  //Play the music 
		  if( Mix_PlayMusic( music, -1 ) == -1 ) 
		  { return 1; 
		  } 
	  } 
	  apply_surface(0,0,title,screen);
	  draw_sprite(0,0,arrow_x,arrow_y,77,56,arrow,screen);

	  SDL_Flip(screen);

	

  }

   //Main game loop
  while ( isRunning && gameStart == false)
    {
		
      //EVENTS
      if ( SDL_PollEvent(&event) )
	{
	  //if the window was closed
	  if ( event.type == SDL_QUIT )
	    {
	      isRunning = false;
	    }

	  //If a button was released and the button is escape
	  if ( event.type == SDL_KEYDOWN  )
	    {
		  keys[event.key.keysym.sym] = true;

		  if(event.key.keysym.sym == SDLK_RIGHT && pressedRight == false){
			 O_arrow_x += 115;
		  }
		  if(event.key.keysym.sym == SDLK_RIGHT && pressedRight == true){
			 O_arrow_x += 145;
		  }
		  if(event.key.keysym.sym == SDLK_LEFT && pressedRight == false)
			 O_arrow_x -= 130;
		  if(event.key.keysym.sym == SDLK_LEFT && pressedRight == true){
		     pressedRight = false;
			 O_arrow_x -= 145;
		  }
		  if(event.key.keysym.sym == SDLK_DOWN && pressedDown == false){
			    O_arrow_x = 200;
				O_arrow_y = 260;		  
				pressedDown = true;
		  }
		  if(event.key.keysym.sym == SDLK_UP && pressedDown == true){
			    O_arrow_x = 85;
				O_arrow_y = 160;
				pressedDown = false;
		  }
		  if(event.key.keysym.sym == SDLK_RETURN && O_arrow_x == 85 && O_arrow_y == 160)
				difficulty = 1;			
		  if(event.key.keysym.sym == SDLK_RETURN && O_arrow_x == 200 && O_arrow_y == 160)
			    difficulty = 2;
		  if(event.key.keysym.sym == SDLK_RETURN && O_arrow_x == 345 && O_arrow_y == 160)
			    difficulty = 3;
		  if(event.key.keysym.sym == SDLK_RETURN && O_arrow_x == 200 && O_arrow_y == 260)
			    goto x;

			if(O_arrow_x > 345)
				O_arrow_x = 345;
			if(O_arrow_x < 85)
				O_arrow_x = 85;
			pressedRight = true;

		  if(event.key.keysym.sym == SDLK_ESCAPE)
			  SDL_Quit();
	    }

	}
	  

	  //If there is no music playing 
	  if( Mix_PlayingMusic() == 0 ) { 
		  //Play the music 
		  if( Mix_PlayMusic( music, -1 ) == -1 ) 
		  { return 1; 
		  } 
	  } 
	  apply_surface(0,0,options,screen);
	  draw_sprite(0,0,O_arrow_x,O_arrow_y,77,56,arrow,screen);

	  SDL_Flip(screen);

	

  }

  //Stop the music 
  Mix_HaltMusic(); 

  //Main game loop
  while ( isRunning && gameStart == true)
    {
		
      //EVENTS
      if ( SDL_PollEvent(&event) )
	{
	  //if the window was closed
	  if ( event.type == SDL_QUIT )
	    {
	      isRunning = false;
	    }

	  //If a button was released and the button is escape
	  if ( event.type == SDL_KEYDOWN  )
	    {
		  keys[event.key.keysym.sym] = true;

		  if(event.key.keysym.sym == SDLK_ESCAPE)
			  SDL_Quit();
	    }

	  //allow user to hold the key down to consistantly move
	  if( event.type == SDL_KEYUP )
	  {
		  keys[event.key.keysym.sym] = false;
	  }

	}

			if(event.key.keysym.sym == SDLK_DOWN)
			 p1_y += 1;
			if(event.key.keysym.sym == SDLK_UP)
			 p1_y -= 1;
			if(p1_y < 0)
			   p1_y = 0;
			if(p1_y + height > 320)
				p1_y = 320 - height;


	apply_surface(0,0,background,screen);
	apply_surface(300,0,line,screen);
	apply_surface(150,380,player1,screen);
	apply_surface(450,380,cpu,screen);
	apply_surface(250,0,score1,screen);
	apply_surface(350,0,score2,screen);
	draw_sprite(srcX,srcY,p1_x,p1_y,paddle_width,paddle_height,User_Paddle,screen);

	Draw_CPU(screen);
	Draw_Ball(screen);
	checkScore = checkWin(score1,score2,screen);

	if(checkScore == 1){		
		Mix_PlayChannel( -1, point, 0 );
		CPUscore++;
	}

	if(checkScore == 0){
		Mix_PlayChannel( -1, point, 0 );
		PLAYERscore++;
	}


	if(CPUscore == 0)
		score2 = SDL_LoadBMP("0.bmp");
	if(CPUscore == 1)
		score2 = SDL_LoadBMP("1.bmp");
	if(CPUscore == 2)
		score2 = SDL_LoadBMP("2.bmp");
	if(CPUscore == 3)
		score2 = SDL_LoadBMP("3.bmp");
	if(CPUscore == 4)
		score2 = SDL_LoadBMP("4.bmp");
	if(CPUscore == 5){
		score2 = SDL_LoadBMP("5.bmp");
		winner = false;
		break;
	}
	if(PLAYERscore == 0)
		score1 = SDL_LoadBMP("0.bmp");
	if(PLAYERscore == 1)
		score1 = SDL_LoadBMP("1.bmp");
	if(PLAYERscore == 2)
		score1 = SDL_LoadBMP("2.bmp");
	if(PLAYERscore == 3)
		score1 = SDL_LoadBMP("3.bmp");
	if(PLAYERscore == 4)
		score1 = SDL_LoadBMP("4.bmp");
	if(PLAYERscore == 5){
		score1 = SDL_LoadBMP("5.bmp");
		winner = true;
		break;
	}

	SDL_Flip(screen);

	

  }

  while(isRunning && winner == true){
	SDL_Surface* win = NULL;
	win = SDL_LoadBMP("win.bmp");

	//The music that will be played 
    Mix_Music *win_music = NULL; 
	win_music = Mix_LoadMUS( "win_music.wav" );

	//EVENTS
      if ( SDL_PollEvent(&event) )
	{
	  //if the window was closed
	  if ( event.type == SDL_QUIT )
	    {
	      isRunning = false;
	    }

	  //If a button was released and the button is escape
	  if ( event.type == SDL_KEYDOWN  )
	    {
		  keys[event.key.keysym.sym] = true;

		  if(event.key.keysym.sym == SDLK_ESCAPE)
			  SDL_Quit();
	    }

	  }
  
	apply_surface(150,100,win,screen);
	
	//If there is no music playing 
	  if( Mix_PlayingMusic() == 0 ) { 
		  //Play the music 
		  if( Mix_PlayMusic( win_music, -1 ) == -1 ) 
		  { return 1; 
		  } 
	  } 

	SDL_Flip(screen);

  
  }

  while(isRunning && winner == false){
	SDL_Surface* lose = NULL;
	lose = SDL_LoadBMP("lose.bmp");

	//The music that will be played 
    Mix_Music *lose_music = NULL; 
	lose_music = Mix_LoadMUS( "lose_music.wav" );

	//EVENTS
      if ( SDL_PollEvent(&event) )
	{
	  //if the window was closed
	  if ( event.type == SDL_QUIT )
	    {
	      isRunning = false;
	    }

	  //If a button was released and the button is escape
	  if ( event.type == SDL_KEYDOWN  )
	    {
		  keys[event.key.keysym.sym] = true;

		  if(event.key.keysym.sym == SDLK_ESCAPE)
			  SDL_Quit();
	    }

	  }
  
	apply_surface(150,100,lose,screen);
	
	//If there is no music playing 
	  if( Mix_PlayingMusic() == 0 ) { 
		  //Play the music 
		  if( Mix_PlayMusic( lose_music, -1 ) == -1 ) 
		  { return 1; 
		  } 
	  } 

	SDL_Flip(screen);

  
  }

  SDL_Quit();

  return 0;

}

int checkWin(SDL_Surface* score1, SDL_Surface* score2, SDL_Surface* screen)
{

    if ( ball_x < p1_x){
		startNew(screen);
		return 1;
		}   
    else if ( ball_x > p2_x){ 
        startNew(screen);
		return 0;
    }    

	return 2;
   
}    

void startNew(SDL_Surface* screen){
p1_x = 0;
p1_y = 0;

p2_x = 570;
p2_y = 0;

ball_x = 300;
ball_y = 200;

}

void Draw_CPU(SDL_Surface* screen){

SDL_Surface *CPU_Paddle = NULL;
CPU_Paddle = SDL_LoadBMP("paddle.bmp");

if(p2_y < 0){
   at_the_bottom = false;
   p2_y = 0;
}

if(at_the_bottom == false && difficulty == 0)
p2_y += 1;

if(at_the_bottom == true && difficulty == 0)
p2_y -= 1;

if(at_the_bottom == false && difficulty == 1)
p2_y += 1;

if(at_the_bottom == true && difficulty == 1)
p2_y -= 1;

if(at_the_bottom == false && difficulty == 2)
p2_y += 3;

if(at_the_bottom == true && difficulty == 2)
p2_y -= 3;

if(at_the_bottom == false && difficulty == 3)
p2_y += 6;

if(at_the_bottom == true && difficulty == 3)
p2_y -= 6;

if(p2_y + height > 320){
	at_the_bottom = true;
   p2_y = 320 - height;
  }

draw_sprite(0,0,p2_x,p2_y,paddle_width,paddle_height,CPU_Paddle,screen);

}

void Title(SDL_Surface* screen){
	SDL_Surface* title = NULL;
	SDL_Surface* arrow = NULL;
	
	title = SDL_LoadBMP("title.bmp");
	arrow = SDL_LoadBMP("arrow.bmp");

	apply_surface(0,0,title,screen);	


}

void Draw_Ball(SDL_Surface* screen)
{
	  SDL_Surface *ball = NULL;
	  ball = SDL_LoadBMP("ball.bmp");
      Mix_Chunk * hit_sound = NULL;
	  hit_sound = Mix_LoadWAV("hit.wav");

	int ball_tempX = ball_x;
	int ball_tempY = ball_y;

    if (dir == 1 && ball_x > 5 && ball_y > 5){
     
         if( ball_x == p1_x + 15 && ball_y >= p1_y && ball_y <= p1_y + 116){
                  dir = rand()% 2 + 3;
         }else{    
                 --ball_x;
                 --ball_y;
         }    
              
    } else if (dir == 2 && ball_x > -1 && ball_y < 380){
         if( ball_x == p1_x + 15 && ball_y >= p1_y && ball_y <= p1_y + 116){
				//Play the hit_paddle effect 
				Mix_PlayChannel( -1, hit_sound, 0 );
                dir = rand()% 2 + 3;
         }else{    
                 --ball_x;
                 ++ball_y;
         }

    } else if (dir == 3 && ball_x < 580 && ball_y > 5){

         if( ball_x + 5 == p2_x && ball_y >= p2_y && ball_y <= p2_y + 116){
				//Play the scratch effect 
				Mix_PlayChannel( -1, hit_sound, 0 );
                dir = rand()% 2 + 1;
         }else{    
                 ++ball_x;
                 --ball_y;
         }

    } else if (dir == 4 && ball_x < 580 && ball_y < 380){

         if( ball_x + 5 == p2_x && ball_y >= p2_y && ball_y <= p2_y + 116){
                  dir = rand()% 2 + 1;
         }else{    
                 ++ball_x;
                 ++ball_y;
         }

    } else { 

        if (dir == 1 || dir == 3)    ++dir;
        else if (dir == 2 || dir == 4)    --dir;

    }    


	  draw_sprite(0,0,ball_x,ball_y,width,height,ball,screen);

}

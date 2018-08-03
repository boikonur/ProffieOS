#define NUM_STARS 100 // Number of Stars
#define CANVAS_WIDTH 126 // Width of the viewport (aka the body width)
#define CANVAS_HEIGHT 31 // Height of the viewport (aka the body height)
int16_t x=CANVAS_WIDTH/2; // Center of the width of the viewport (width/2)
int16_t y=CANVAS_HEIGHT/2; // Center of the height of the viewport (height/2)
int16_t z=(CANVAS_WIDTH+CANVAS_HEIGHT)/2; // Hypothetical z-value representing where we are on the screen
#define STAR_COLER_RATIO 1/z // Determines how big to draw the star
#define STAR_RATIO 1 // Just a constant effecting the way stars move
#define STAR_SPEED 1 // The speed of the star. Yes, all star's have the same speed.
int16_t STAR_SPEEDPrev=0; // Play around with the values for star speed
int16_t Particle[NUM_STARS][5]; // Data structure to hold the position of all the stars


void InitLightSpeed(){
	for(int i=0;i<NUM_STARS;i++){
		Particle[i][0]= random()*CANVAS_WIDTH*2-CANVAS_HEIGHT*2; //Actual X-coordinate of position in prespective of viewer
		Particle[i][1]= random()*CANVAS_HEIGHT*2-y*2; //Actual Y-coordinate of position in prespective of viewer
		Particle[i][2]= random()*z;    //Actual Z-coordinate of position in prespective of viewer
		Particle[i][3]=0;  //Calculated X (represents X-coordinate on screen)
		Particle[i][4]=0;  //Calculated Y (represents Y-coordinate on screen)
	}
	ClearScreen();
}

void AnimateLightSpeed(){

	ClearScreen();

	for(int i=0; i<NUM_STARS; i++){
		// Flag for if the star is offscreen (we don't want to draw it)
		bool test=true;
		/* Save the stars calculated position so we can use it for drawing */
		uint8_t starXPrev=Particle[i][3];
		uint8_t starYPrev=Particle[i][4];
	  // Particle[i][0]+=MoveX>>4;
		// Particle[i][1]+=MoveY>>4;
		Particle[i][2]-=STAR_SPEED;
		/* Check the boundary conditions to make sure stars aren't offscreen. */
		if(Particle[i][0]>x<<1){ 
			Particle[i][0]-=CANVAS_WIDTH<<1; 
			test=false; 
		} 
		if(Particle[i][0]<-x<<1){ 
			Particle[i][0]+=CANVAS_WIDTH<<1; 
			test=false;
		}
		if(Particle[i][1]>y<<1){ 
			Particle[i][1]-=CANVAS_HEIGHT<<1; 
			test=false; 
		} 
		if(Particle[i][1]<-y<<1){ 
			Particle[i][1]+=CANVAS_HEIGHT<<1; 
			test=false; 
		}
		if(Particle[i][2]>z){ 
			Particle[i][2]-=z; 
			test=false;
		} 
		if(Particle[i][2]<0){ 
			Particle[i][2]+=z; 
			test=false; 
		}
		// Our calculated position and where the star is going to be drawn on the screen
		Particle[i][3]=x + (Particle[i][0]/Particle[i][2]) * STAR_RATIO;
		Particle[i][4]=y + (Particle[i][1]/Particle[i][2]) * STAR_RATIO;
		// Actually draw the object, if the star isn't offscreen
		if( (starXPrev > 0)&&
        (starXPrev < CANVAS_WIDTH)&&
        (starYPrev > 0)&&
        (starYPrev < CANVAS_HEIGHT)&&
        test){
			// Note: all stars, even though appear the be dots, are actually drawn as lines
			//lineWidth=(1-STAR_COLER_RATIO*star[i][2])*2;
		DrawLine(starXPrev, starYPrev, Particle[i][3], Particle[i][4], WHITE);
		}
	}
}
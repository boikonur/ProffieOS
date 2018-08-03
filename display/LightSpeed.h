uint8_t n=100; // Number of Stars
uint8_t w=127; // Width of the viewport (aka the body width)
uint8_t h=32; // Height of the viewport (aka the body height)
uint8_t x=w/2; // Center of the width of the viewport (width/2)
uint8_t y=h/2; // Center of the height of the viewport (height/2)
uint8_t z=(w+h)/2; // Hypothetical z-value representing where we are on the screen
uint8_t starColorRatio=1/z; // Determines how big to draw the star
uint8_t starRatio=256; // Just a constant effecting the way stars move
uint8_t starSpeed=4; // The speed of the star. Yes, all star's have the same speed.
uint8_t starSpeedPrev=0; // Play around with the values for star speed, I noticed a cool effect if we made the star speed 0. Hence, I created a variable to save the star speed in those cases
uint8_t star[n][5]; // Data structure to hold the position of all the stars

void InitLightSpeed(){
	/* Initialize the stars.
	Since the ship is in the middle, we assume
	Each star has the following properties:
	1.[0] Actual X-coordinate of position in prespective of ship
	2.[1] Actual Y-coordinate of position in prespective of ship
	3.[2] Actual Z-coordinate of position in prespective of ship
	4.[3] Calculated X (represents X-coordinate on screen)
	5.[4] Calculated Y (represents Y-coordinate on screen)
	*/
	for(int i=0;i<n;i++){
		star[i][0]= random(w);//Math.random()*w*2-x*2;
		star[i][1]= random(h); //Math.random()*h*2-y*2;
		star[i][2]= random(z);//Math.round(Math.random()*z);
		star[i][3]=0;
		star[i][4]=0;
	}

	ClearScreen();

}
void AnimateLightSpeed(){

	ClearScreen();

	for(int i=0;i<n;i++){
		// Flag for if the star is offscreen (we don't want to draw it)
		int test=true;
		/* Save the stars calculated position so we can use it for drawing */
		int starXPrev=star[i][3];
		int starYPrev=star[i][4];
		/* Update the Star */
		// star[i][0]+=mouseX>>4;
		// star[i][1]+=mouseY>>4;
		star[i][2]-=starSpeed;
		/* Check the boundary conditions to make sure stars aren't offscreen. */
		if(star[i][0]>x<<1){ 
			star[i][0]-=w<<1; 
			test=false; 
		} 
		if(star[i][0]<-x<<1){ 
			star[i][0]+=w<<1; 
			test=false;
		}
		if(star[i][1]>y<<1){ 
			star[i][1]-=h<<1; 
			test=false; 
		} 
		if(star[i][1]<-y<<1){ 
			star[i][1]+=h<<1; 
			test=false; 
		}
		if(star[i][2]>z){ 
			star[i][2]-=z; 
			test=false;
		} 
		if(star[i][2]<0){ 
			star[i][2]+=z; 
			test=false; 
		}
		// Our calculated position and where the star is going to be drawn on the screen
		star[i][3]=x + (star[i][0]/star[i][2]) * starRatio;
		star[i][4]=y + (star[i][1]/star[i][2]) * starRatio;
		// Actually draw the object, if the star isn't offscreen
		if(starXPrev>0&&starXPrev<w&&starYPrev>0&&starYPrev<h&&test){
			// Note: all stars, even though appear the be dots, are actually drawn as lines
			//lineWidth=(1-starColorRatio*star[i][2])*2;
			DrawLine(starXPrev, starYPrev, star[i][3], star[i][4], WHITE);
		}
	}

}

// To start the animation
void start(){
    InitLightSpeed();
	AnimateLightSpeed();
}


void loop(){
AnimateLightSpeed();
	//TODO: Try effect below
		// starSpeedPrev = (starSpeed != 0) ? starSpeed:starSpeedPrev;
		// starSpeed = (starSpeed != 0) ? 0:starSpeedPrev;	
}

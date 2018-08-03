#ifndef DISPLAY_SSD1306_H
#define DISPLAY_SSD1306_H

#define NUM_STARS 100 // Number of Stars
int16_t w=127; // Width of the viewport (aka the body width)
int16_t h=32; // Height of the viewport (aka the body height)
int16_t x=w/2; // Center of the width of the viewport (width/2)
int16_t y=h/2; // Center of the height of the viewport (height/2)
int16_t z=(w+h)/2; // Hypothetical z-value representing where we are on the screen
int16_t starColorRatio=1/z; // Determines how big to draw the star
#define STAR_RATIO 2 // Just a constant effecting the way stars move
int16_t starSpeed=4; // The speed of the star. Yes, all star's have the same speed.
int16_t starSpeedPrev=0; // Play around with the values for star speed, I noticed a cool effect if we made the star speed 0. Hence, I created a variable to save the star speed in those cases
int16_t Particle[NUM_STARS][5]; // Data structure to hold the position of all the stars


struct Glyph {
  int8_t skip;
  int8_t xoffset;
  int8_t yoffset;
  uint8_t columns;
  const uint32_t* data;
};

const uint32_t BatteryBar16_data[] = {
   0b00000000000000000000000000000000UL,
   0b00000111111111111111111111100000UL,
   0b00011111111111111111111111111000UL,
   0b00111111111111111111111111111100UL,
   0b01111111111111111111111111111110UL,
   0b11111111111111111111111111111111UL,
   0b11111111111111111111111111111111UL,
   0b11111111111111111111111111111111UL,
   0b11111111111111111111111111111111UL,
   0b11111111111111111111111111111111UL,
   0b11111111111111111111111111111111UL,
   0b01111111111111111111111111111110UL,
   0b00111111111111111111111111111100UL,
   0b00011111111111111111111111111000UL,
   0b00000111111111111111111111100000UL,
};

const uint32_t BatteryIcon[] = {
    0b00111100UL,
    0b00100100UL,
    0b11111111UL,
    0b10000001UL,
    0b10000001UL,
    0b10000001UL,
    0b10000001UL,
    0b10000001UL,
    0b10000001UL,
    0b10000001UL,
    0b10000001UL,
    0b10000001UL,
    0b10000001UL,
    0b11111111UL,
};

#define BLACK 0
#define WHITE 1
#define INVERSE 2

#define GLYPHDATA(X) NELEM(X), X

const Glyph BatteryBar16 = { 16, 0, 0, GLYPHDATA(BatteryBar16_data) };

const Glyph BatteryIndicator = { 15, 0, 0, GLYPHDATA(BatteryIcon) };

#include "StarJedi10Font.h"

#define FONT_NAME Starjedi10pt7bGlyphs
#include "saber_logo.h"
const Glyph SaberLogo = { 0, 0, 0, GLYPHDATA(saberLogoLS6) };
class SSD1306 : public I2CDevice, Looper, StateMachine, SaberBase, CommandParser {
public:
  static const int WIDTH = 128;
  static const int HEIGHT = 32;
  const char* name() override { return "SSD1306"; }

  enum Commands {
    SETCONTRAST = 0x81,
    DISPLAYALLON_RESUME = 0xA4,
    DISPLAYALLON = 0xA5,
    NORMALDISPLAY = 0xA6,
    INVERTDISPLAY = 0xA7,
    DISPLAYOFF = 0xAE,
    DISPLAYON = 0xAF,

    SETDISPLAYOFFSET = 0xD3,
    SETCOMPINS = 0xDA,

    SETVCOMDETECT = 0xDB,

    SETDISPLAYCLOCKDIV = 0xD5,
    SETPRECHARGE = 0xD9,

    SETMULTIPLEX = 0xA8,

    SETLOWCOLUMN = 0x00,
    SETHIGHCOLUMN = 0x10,

    SETSTARTLINE = 0x40,

    MEMORYMODE = 0x20,
    COLUMNADDR = 0x21,
    PAGEADDR   = 0x22,

    COMSCANINC = 0xC0,
    COMSCANDEC = 0xC8,

    SEGREMAP = 0xA0,

    CHARGEPUMP = 0x8D,

    EXTERNALVCC = 0x1,
    SWITCHCAPVCC = 0x2,

    // Scrolling commands
    ACTIVATE_SCROLL = 0x2F,
    DEACTIVATE_SCROLL = 0x2E,
    SET_VERTICAL_SCROLL_AREA = 0xA3,
    RIGHT_HORIZONTAL_SCROLL = 0x26,
    LEFT_HORIZONTAL_SCROLL = 0x27,
    VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL = 0x29,
    VERTICAL_AND_LEFT_HORIZONTAL_SCROLL = 0x2A,
  };

  enum Screen {
    SCREEN_STARTUP,
    SCREEN_SAVER,
    SCREEN_MESSAGE,
    SCREEN_PLI,
  };

  SSD1306() : I2CDevice(0x3C), CommandParser() {}
  void Send(int c) { writeByte(0, c); }

 void DrawPixel(int16_t  x, int16_t  y, uint8_t color){

    if (y<0 || x< 0) return;
    if (y>HEIGHT || x> WIDTH) return;
 
  switch (color)
    {
      case WHITE:   frame_buffer_[x] |= 0x1 << y; break;
      case BLACK:   frame_buffer_[x] &= ~(0x1 << y); break;
      case INVERSE: frame_buffer_[x] ^= (0x1 << y); break;
    }
 }
  void Draw(const Glyph& glyph, int x, int y) {
    x += glyph.xoffset;
    y += glyph.yoffset;
    int begin = max(0, -x);
    int end = min(glyph.columns, WIDTH - x);
    uint32_t *pos = frame_buffer_ + x;
    if (y > 0) {
      for (int i = begin; i < end; i++) pos[i] |= glyph.data[i] << y;
    } else if (y < 0) {
      for (int i = begin; i < end; i++) pos[i] |= glyph.data[i] >> -y;
    } else {
      for (int i = begin; i < end; i++) pos[i] |= glyph.data[i];
    }
  }

#ifndef _swap_int16_t
#define _swap_int16_t(a, b) { int16_t t = a; a = b; b = t; }
#endif

void DrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t color) {
    int16_t steep = abs(y1 - y0) > abs(x1 - x0);
    if (steep) {
        _swap_int16_t(x0, y0);
        _swap_int16_t(x1, y1);
    }

    if (x0 > x1) {
        _swap_int16_t(x0, x1);
        _swap_int16_t(y0, y1);
    }

    int16_t dx, dy;
    dx = x1 - x0;
    dy = abs(y1 - y0);

    int16_t err = dx / 2;
    int16_t ystep;

    if (y0 < y1) {
        ystep = 1;
    } else {
        ystep = -1;
    }

    for (; x0<=x1; x0++) {
        if (steep) {
            DrawPixel(y0, x0, color);
        } else {
            DrawPixel(x0, y0, color);
        }
        err -= dy;
        if (err < 0) {
            y0 += ystep;
            err += dx;
        }
    }
}

  void DrawCircle(int16_t  x0, int16_t  y0, int16_t  r, int16_t  color) {
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

    DrawPixel(x0  , y0+r, color);
    DrawPixel(x0  , y0-r, color);
    DrawPixel(x0+r, y0  , color);
    DrawPixel(x0-r, y0  , color);

    while (x<y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        DrawPixel(x0 + x, y0 + y, color);
        DrawPixel(x0 - x, y0 + y, color);
        DrawPixel(x0 + x, y0 - y, color);
        DrawPixel(x0 - x, y0 - y, color);
        DrawPixel(x0 + y, y0 + x, color);
        DrawPixel(x0 - y, y0 + x, color);
        DrawPixel(x0 + y, y0 - x, color);
        DrawPixel(x0 - y, y0 - x, color);
    }
}

void DrawTriangle(int16_t  x0, int16_t  y0, int16_t  x1, int16_t  y1, int16_t  x2, int16_t  y2, int16_t  color) {
    DrawLine(x0, y0, x1, y1, color);
    DrawLine(x1, y1, x2, y2, color);
    DrawLine(x2, y2, x0, y0, color);
}

void DrawFullRect(int16_t x, int16_t y, int16_t w, int16_t h, uint8_t color) {
    for (int16_t i=x; i<x+w; i++) {
        DrawLine(i, y, i, y+h-1, color);         
    }
}

void ClearScreen(){
  memset(frame_buffer_, 0, sizeof(frame_buffer_));
}

   bool Parse(const char* cmd, const char* arg) override {
    if (!strcmp(cmd, "ssd")) {     
          SB_Message(arg);     
         return true;        
    }
    return false;
  }

  void Help() override {
    STDOUT.print("ssd \"text\"");
    STDOUT.println(" - Prints on LCD ");
 }

  void DrawBatteryBar(const Glyph& bar) {
    int start, end;
    if (bar.skip < bar.columns) {
      start = -bar.skip;
      end = WIDTH + bar.columns - 1;
    } else {
      start = 0;
      end = WIDTH;
    }
    int max_bars = (end - start) / bar.skip;
    int pos = start;
    int bars = floor(
        battery_monitor.battery_percent() * (0.5 + max_bars) / 100);
    for (int i = 0; i < bars; i++) {
      Draw(bar, pos, 0);
      pos += bar.skip;
    }
  }
  void DrawText(const char* str,
                int x, int y,
                const Glyph* font) {
    while (*str) {
      if (*str == '\n') {
        x = 0;
        y += 16;
      } else if (*str >= 0x20 && *str <= 0x7f) {
        Draw(font[*str - 0x20], x, y);
        x += font[*str - 0x20].skip;
      }
      str++;
    }
  }

void DrawScreenSaver( const char* text){
   static int yPosition = 0;
   static int xPosition = 0;
   static int xDirection = 1;
   static int yDirection = 1;
    if (xPosition == 0){
      xDirection = 1;
    }
    else 
    if (xPosition == WIDTH){
      xDirection = -1;
    }

    if (yPosition == 0){
      yDirection = 1;
    }
    else 
    if (yPosition == HEIGHT){
      yDirection = -1;
    }
 
    DrawText(text, xPosition, yPosition, FONT_NAME); 
    xPosition += xDirection;
    yPosition += yDirection;  
}



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
	for(int i=0;i<NUM_STARS;i++){
		Particle[i][0]= random()*w*2-x*2;
		Particle[i][1]= random()*h*2-y*2;
		Particle[i][2]= (random()*z);  //round
		Particle[i][3]=0;
		Particle[i][4]=0;
	}

	ClearScreen();

}
void AnimateLightSpeed(){

	//ClearScreen();

	for(int i=0;i<NUM_STARS;i++){
		// Flag for if the star is offscreen (we don't want to draw it)
		int test=true;
		/* Save the stars calculated position so we can use it for drawing */
		uint8_t starXPrev=Particle[i][3];
		uint8_t starYPrev=Particle[i][4];

		Particle[i][2]-=starSpeed;
		/* Check the boundary conditions to make sure stars aren't offscreen. */
		if(Particle[i][0]>x<<1){ 
			Particle[i][0]-=w<<1; 
			test=false; 
		} 
		if(Particle[i][0]<-x<<1){ 
			Particle[i][0]+=w<<1; 
			test=false;
		}
		if(Particle[i][1]>y<<1){ 
			Particle[i][1]-=h<<1; 
			test=false; 
		} 
		if(Particle[i][1]<-y<<1){ 
			Particle[i][1]+=h<<1; 
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
		if(
      (starXPrev > 0)&&
        (starXPrev < w )&&
        (starYPrev > 0)&&
        (starYPrev < h)&&
        test
        ){
			// Note: all stars, even though appear the be dots, are actually drawn as lines
			//lineWidth=(1-starColorRatio*star[i][2])*2;
			DrawLine(starXPrev, starYPrev, Particle[i][3], Particle[i][4], WHITE);

		}
	}

}

void RainScreen(){
  
}

void DrawScreenSaver(int16_t x, int16_t y) {
 Draw( BatteryIndicator,x,y);
 int battery_bars = floor(clamp(battery_monitor.battery_percent(),0,100) * (0.5 + 8) / 100);
 DrawFullRect(x+4+8-battery_bars , y+2,battery_bars ,4,WHITE);
 DrawText(GfxBatteryPercentage(),BatteryIndicator.skip, y+10, FONT_NAME);
}
char* GfxBatteryPercentage(void){
  static char bat_info[6];
  static uint32_t ref_time=0;

  if (uint32_t(millis()- ref_time) > 2000){
    ref_time=millis();
    int perc = floor(battery_monitor.battery_percent() );
    sprintf(bat_info, "%d%%", perc);   
  }
  return bat_info;
}

  void FillFrameBuffer() {
    memset(frame_buffer_, 0, sizeof(frame_buffer_));

    if (millis() - displayed_when_ > 5000)
      screen_ = SCREEN_SAVER;

    switch (screen_) {
      case SCREEN_STARTUP:
        DrawText("==SabeR===", 0,15, FONT_NAME);
        DrawText("++Teensy++",-4,31, FONT_NAME);
        InitLightSpeed();
        break;

      case SCREEN_SAVER:        
       // DrawScreenSaver(GfxBatteryPercentage());   
           if (millis() - updateLightspeed_ > 50)
        AnimateLightSpeed();

     
        break;

      case SCREEN_PLI:
        DrawBatteryBar(BatteryBar16);
        break;

      case SCREEN_MESSAGE:
        if (strchr(message_, '\n')) {
          DrawText(message_, 0, 15, FONT_NAME);
        } else {
          DrawText(message_, 0, 23, FONT_NAME);
        }
    }
  }

  void SB_Message(const char* text) override {
    strncpy(message_, text, sizeof(message_));
    message_[sizeof(message_)-1] = 0;
    displayed_when_ = millis();
    screen_ = SCREEN_MESSAGE;
  }

  void SB_Top() override {
    STDOUT.print("display fps: ");
    loop_counter_.Print();
    STDOUT.println("");
  }

  void Loop() override {
    STATE_MACHINE_BEGIN();
    while (!i2cbus.inited()) YIELD();

    // Init sequence
    Send(DISPLAYOFF);                    // 0xAE
    Send(SETDISPLAYCLOCKDIV);            // 0xD5
    Send(0x80);                                  // the suggested ratio 0x80
    
    Send(SETMULTIPLEX);                  // 0xA8
    Send(HEIGHT - 1);
    
    Send(SETDISPLAYOFFSET);              // 0xD3
    Send(0x0);                                   // no offset
    Send(SETSTARTLINE | 0x0);            // line #0
    Send(CHARGEPUMP);                    // 0x8D
    Send(0x14);
    Send(MEMORYMODE);                    // 0x20
    Send(0x01);                          // vertical address mode
    Send(SEGREMAP | 0x1);
    Send(COMSCANDEC);

    Send(SETCOMPINS);                    // 0xDA
    Send(0x02);  // may need to be 0x12 for some displays
    Send(SETCONTRAST);                   // 0x81
    Send(0x8F);

    Send(SETPRECHARGE);                  // 0xd9
    Send(0xF1);
    Send(SETVCOMDETECT);                 // 0xDB
    Send(0x40);
    Send(DISPLAYALLON_RESUME);           // 0xA4
    Send(NORMALDISPLAY);                 // 0xA6
    
    Send(DEACTIVATE_SCROLL);

    Send(DISPLAYON);                     //--turn on oled panel

    STDOUT.println("Display initialized.");
    screen_ = SCREEN_STARTUP;
    displayed_when_ = millis();
    updateLightspeed_= millis();
    
    while (true) {
      FillFrameBuffer();
      Send(COLUMNADDR);
      Send(0);   // Column start address (0 = reset)
      Send(WIDTH-1); // Column end address (127 = reset)

      Send(PAGEADDR);
      Send(0); // Page start address (0 = reset)
      switch (HEIGHT) {
        case 64:
          Send(7); // Page end address
          break;
        case 32:
          Send(3); // Page end address
          break;
        case 16:
          Send(1); // Page end address
          break;
        default:
          STDOUT.println("Unknown display height");
      }

      //STDOUT.println(TWSR & 0x3, DEC);
        
      // I2C
      for (i=0; i < WIDTH * HEIGHT / 8; ) {
        // send a bunch of data in one xmission
        Wire.beginTransmission(address_);
        Wire.write(0x40);
        for (uint8_t x=0; x<16; x++) {
          Wire.write(((unsigned char*)frame_buffer_)[i]);
          i++;
        }
        Wire.endTransmission();
        YIELD();
      }
      loop_counter_.Update();
    }
    
    STATE_MACHINE_END();
  }

private:
  uint16_t i;
  uint32_t frame_buffer_[WIDTH];
  LoopCounter loop_counter_;
  char message_[32];
  uint32_t displayed_when_;

    uint32_t updateLightspeed_;
  Screen screen_;


};

#endif

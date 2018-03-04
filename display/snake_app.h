/*Snake Application */

#define GO_RIGHT 0
#define GO_UP    1
#define GO_LEFT  2
#define GO_DOWN  3

const uint32_t sPixel[]  = {
 0b1111UL,
 0b1101UL,
 0b1011UL,
 0b1111UL,
};

const Glyph SnakePixel = { 0, 0, 0, GLYPHDATA(sPixel) };

class SNAKE_APP : public CommandParser{
  
  public:
   const char* name() override { return "Snake Application"; }

    SNAKE_APP() : CommandParser() {
      //Init constr
    }

  static const int APP_WIDTH = 128;
  static const int APP_HEIGHT = 32;
  struct FOOD {
    int x;
    int y;
    int yes;  
  };

  struct SNAKE {
    int x[200];
    int y[200];
    int node;
    int dir;
  };

  SnakeInit() {
    //FOOD food = {5, 50, 1};//(,97)
    //SNAKE snake = {{9,5}, {30,30}, 2, GO_RIGHT, 0};
    snake.x[0]=16; 
    snake.y[0]=16;
    snake.x[1]=11; 
    snake.y[1]=16;
    snake.dir=GO_RIGHT;
    snake.node=2;
    food.x=16;
    food.y=16;
    food.yes=1;
  }

bool Parse(const char* cmd, const char* arg) override {  
  if (!strcmp(cmd, "w")) {  
      snake.dir = GO_UP;   
      return true;               
    }
  if (!strcmp(cmd, "a")) {  
      snake.dir = GO_LEFT;
      return true;              
    }
  if (!strcmp(cmd, "s")) {  
      snake.dir = GO_DOWN;     
      return true;              
    }
  if (!strcmp(cmd, "d")) {  
      snake.dir = GO_RIGHT;
      return true;               
    }
    return false;
  }      

void DrawSnakePixel(int x, int y) {
  display.Draw(eleGlyph,x,y);
}

void DrawSnake() {
    for(i=0; i< snake.node; i++)
          DrawSnakePixel(snake.x[i], snake.y[i]);

}

void DrawFood(){
    DrawSnakePixel(food.x, food.y);   
}

void SnakeUI() {
  DrawLine(0, 0, 0, 31, WHITE);
  DrawLine(127, 0, 127, 31, WHITE);
  DrawLine(0, 0, 127, 0, WHITE);
  DrawLine(0, 31, 127, 31, WHITE);
  // DrawText(10,16, "score:"); 
}

void SnakeGO_UPdate() {
  static uint32_t ref_time=0;

  if (uint32_t(millis()- ref_time) > 100) {
    ref_time=millis();    

#if 0
  STDOUT.println("SNAK EAT");
  STDOUT.println( snake.node);
  STDOUT.print( "food.x:");
  STDOUT.println( food.x);
  STDOUT.print( "food.y:");
  STDOUT.println( food.y);
  STDOUT.print( "snake0.x:");
  STDOUT.println( snake.x[0]);
  STDOUT.print( "snake0.y:");
  STDOUT.println( snake.y[0]);
#endif

  switch(snake.dir) {
    case GO_RIGHT: 
          snake.x[0] += 4;
          if(snake.x[0]>=APP_WIDTH) {
            snake.x[0] = 0;
          } break;
    case GO_UP: 
          snake.y[0] -= 4;
          if(snake.y[0]<=0) {
            snake.y[0] = APP_HEIGHT-4;
          } break;
    case GO_LEFT: 
          snake.x[0] -= 4;
          if(snake.x[0]<=0) {
            snake.x[0] = APP_WIDTH-4;
          } break;
    case GO_DOWN:
          snake.y[0] += 4;
          if(snake.y[0]>=APP_HEIGHT-1) {
            snake.y[0] = 4;
          } break;
  }  
   
   if((snake.x[0] == food.x) && (snake.y[0] == food.y)) {
    snake.x[0] = food.x;
    snake.y[0] = food.y;
    snake.node++;
    food.yes = 0;
    score += 1;
    STDOUT.println("SNAK EAT");
    STDOUT.println( snake.node);
    //food.x=random(1, 120 ); //TODO new food  ( place it correct)
    //food.y=random(3, 29 );
  }
     }
  for(int t=snake.node-1;t>0;t--) {
    snake.x[i] = snake.x[t-1];
    snake.y[i] = snake.y[t-1];
  }
}

  void Loop() override { 
      //SnakeInit() 
      SnakeUI();
      DrawSnake();
      DrawFood();
      SnakeGO_UPdate();    
}
  
  private:
    FOOD food;
    SNAKE snake;
    uint32_t score = 0;
};
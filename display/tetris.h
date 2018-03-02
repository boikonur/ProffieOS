#include "../common/application.h"
#ifndef DISPLAY_TETRIS_H
#define DISPLAY_TETRIS_H

#define FIELD_W 8
#define FIELD_H 48
#define NUM_FIGURES 7
#define inv(x) ((x * (-1)) + 3)

const int fig_map[7][4][4] = {
    {{1, 1, 0, 0},
     {1, 1, 0, 0},
     {0, 0, 0, 0},
     {0, 0, 0, 0}},
    {{1, 0, 0, 0},
     {1, 0, 0, 0},
     {1, 0, 0, 0},
     {1, 0, 0, 0}},
    {{0, 0, 1, 0},
     {1, 1, 1, 0},
     {0, 0, 0, 0},
     {0, 0, 0, 0}},
    {{1, 1, 1, 0},
     {0, 0, 1, 0},
     {0, 0, 0, 0},
     {0, 0, 0, 0}},
    {{0, 1, 1, 0},
     {1, 1, 0, 0},
     {0, 0, 0, 0},
     {0, 0, 0, 0}},
    {{1, 1, 0, 0},
     {0, 1, 1, 0},
     {0, 0, 0, 0},
     {0, 0, 0, 0}},
    {{1, 1, 1, 0},
     {0, 1, 0, 0},
     {0, 0, 0, 0},
     {0, 0, 0, 0}}};

class TETRIS_APP : public Application,  Looper, StateMachine, SaberBase, CommandParser  {
  public:

     const char* name() override { 
        return "Tetris App";
    }

    TETRIS_APP() : Application(), CommandParser(){
        frame_buffer[FIELD_W][FIELD_H] = {0};
        bTetris_en=false;
        speed_=50;
        bInit_=false;
    }

    void PutPixel(int x, int y, int color, uint32_t *buf) {
        if (x < 0 || x >= 96)
            return;
        if (y < 0 || y >= 16)
            return;
        uint8_t b = 1 << (y % 8);
        buf += (y / 8) * 96 + (x % 96);
        if (color)
            *buf |= b;
        else
            *buf &= ~b;
    }

    void DrawField(int x, int y, uint32_t *buf, int buff[FIELD_W][FIELD_H]) {
        int i, j, w, h;
        int p = 2;
        for (i = 0; i < FIELD_H; i++)
            for (j = 0; j < FIELD_W; j++)
                for (w = 0; w < p; w++)
                    for (h = 0; h < p; h++)
                        PutPixel(96 - 1 - (x + i * p + w), 16 - 1 - (y + j * p + h), buff[j][i], buf);
    }

    void PrintLayout(void) {
        int i, j;
        static int buff[FIELD_W][FIELD_H];

        for (i = 0; i < FIELD_H; i++)
            for (j = 0; j < FIELD_W; j++)
                buff[j][i] = frame_buffer[j][i];

        for (i = 0; i < 4; i++)
            for (j = 0; j < 4; j++)
                if (map[j][i])
                    buff[j + px_][i + py_] = 1;

        DrawField(0, 0, buf, buff);
    }

    bool valid(int x, int y) {
        int i, j;
        if (x < 0)
            return false;
        for (i = 0; i < 4; i++)
            for (j = 0; j < 4; j++)
                if (map[j][i])
                {
                    if ((j + x >= FIELD_W) || (i + y >= FIELD_H))
                        return true;
                    if (frame_buffer[j + x][i + y])
                        return false;
                }
        return true;
    }

    void RotateMap(void) {
        int _map[4][4];
        int i, j, sx = 4, sy = 4;

        for (i = 0; i < 4; i++)
            for (j = 0; j < 4; j++)
            {
                _map[j][i] = map[j][i];
                if (map[j][i])
                {
                    if (i < sx)
                        sx = i;
                    if (inv(j) < sy)
                        sy = inv(j);
                }
                map[j][i] = 0;
            }

        for (i = 0; i < 4; i++)
            for (j = 0; j < 4; j++)
                if (_map[inv(i)][j])
                    map[j - sx][i - sy] = 1;

        if (!valid(px_, py_))
            for (i = 0; i < 4; i++)
                for (j = 0; j < 4; j++)
                    map[j][i] = _map[j][i];
    }

    void DeleteLine(void) {
        int i, j, k, cl;

        for (i = FIELD_H - 1; i >= 0; i--)
        {
            cl = 1;

            for (j = 0, cl = 1; j < FIELD_W; j++)
                if (!frame_buffer[j][i])
                    cl = 0;

            if (cl)
            {
                score_ += (((i * (-1)) + FIELD_H) * 10);
                for (k = i; k > 0; k--)
                {
                    for (j = 0; j < FIELD_W; j++)
                    {
                        frame_buffer[j][k] = frame_buffer[j][k - 1];
                    }
                }
                i++;
                PrintLayout();
            }
        }
    }

    void CreateMap(void) {
        int i, j;
        for (i = 0; i < 4; i++)
            for (j = 0; j < 4; j++)
                map[j][i] = fig_map[nextmap_][j][i];
        py_ = 0;
        px_ = FIELD_W / 2;
        nextmap_ = random(NUM_FIGURES);
    }

    void ClearScreen(void) {
        int i, j;
        for (i = 0; i < FIELD_H; i++)
            for (j = 0; j < FIELD_W; j++)
                frame_buffer[j][i] = 0;
    }

    void StartGame(void) {
        STDOUT.println("Start Tetris");
        ClearScreen();
        px_ = FIELD_W / 2;
        py_ = 0;
        score_ = 0;
        nextmap_ = random(NUM_FIGURES);
        CreateMap();
    }

    bool GameOver(void) {
        int i;
        for (i = 0; i < FIELD_W; i++)
            if (frame_buffer[i][0])
                return true;
        return false;
    }

    void AdvanceFigure(uint32_t speed) {
        static uint32_t step_tyme=0;
         int i, j;

        if (uint32_t(millis()- step_tyme) > speed) {
            step_tyme=millis();

             if (!valid(px_, py_ + 1)) {
            for (i = 0; i < 4; i++)
                for (j = 0; j < 4; j++)
                    if (map[j][i])
                        frame_buffer[px_ + j][py_ + i] = 1;

            CreateMap();
            DeleteLine();
        }
        else
            py_++;
        }   
    }

    void DropFigure(void) {
        int i, j;
        for (; valid(px_, py_ + 1); py_++)
            ;
        for (i = 0; i < 4; i++)
            for (j = 0; j < 4; j++)
                if (map[j][i])
                    frame_buffer[px_ + j][py_ + i] = 1;
    }

    bool Parse(const char *cmd, const char *arg) override
    {
        if (!strcmp(cmd, "tetris"))
        {
            if (!strcmp(arg, "1")) { //move left
                if (valid(px_ - 1, py_))
                    px_--;
            }
            if (!strcmp(arg, "3")) { //move right
                if (valid(px_ + 1, py_))
                    px_++;
            }
            if (!strcmp(arg, "2")) { //rotate fig
                RotateMap();
            }
            if (!strcmp(arg, "A")) {  //TODO: disable tetris app 
                bTetris_en = !bTetris_en;
            }
            if (!strcmp(arg, "B")) {
                DropFigure();
                PrintLayout();
                DeleteLine();
                CreateMap();
            }
            return true;
        }

        return false;
    }

    void DisplayDrawArea(uint8_t x0, uint8_t y0, uint8_t wide, uint8_t high, uint32_t *ptr)  {
            //use ssd drawing
            display.DrawArea(x0, y0, wide, high, ptr);
    }

    void loop() {

        if(bTetris_en) {
            //exit
      

        if (!bInit_) {
            StartGame();
            bInit_ = true;
        }

        AdvanceFigure(speed_);  
        PrintLayout();

        if (GameOver())
            StartGame();

        DisplayDrawArea(0, 0, 96, 16, buf); 
    }
    }

  private:
    uint32_t speed_;
    int bTetris_en;
    int frame_buffer[FIELD_W][FIELD_H];
    int px_, py_, score_, nextmap_;
    uint32_t buf[192];
    int map[4][4];
    bool bInit_;
};

#endif //DISPLAY_TETRIS_H
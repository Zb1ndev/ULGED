#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#include "raylib.h"

#pragma region Timer

  typedef struct Timer {
    double startTime;   // Start time (seconds)
    double lifeTime;    // Lifetime (seconds)
  } Timer;

  void StartTimer(Timer *timer, double lifetime) {
      timer->startTime = GetTime();
      timer->lifeTime = lifetime;
  }

  bool TimerDone(Timer timer) {
    return GetTime() - timer.startTime >= timer.lifeTime;
  }

  double GetElapsed(Timer timer) {
    return GetTime() - timer.startTime;
  }

#pragma endregion
#pragma region Globals

int viewedLineNum = 0;
int maxLineNum = 9999;
int highlightedLine = 0;
int lnBufferLength = 0;
Timer _backspaceTimer;
bool useCustomTitleBar = false;

#pragma endregion 
#pragma region Utilities

  float PercentX(float _percent) {
    float _conversion = GetRenderWidth() * _percent;
    return _conversion;
  }

  float PercentY(float _percent) {
    float _conversion = GetRenderHeight() * _percent;
    return _conversion;
  }

  bool CompareString(const char* _a, const char* _b, int _length) {
    _a = TextToLower(_a);
    for(size_t i = 0; i < _length; ++i)
      if (_a[i] != _b[i])
        return false;
    return true;
  }

  int GetNumAfterCommand(const char* _command, int _commandLength, int _argIndex) {
    char _buf[5];
    for (int i = 0; i < 5; i++) {
      int _index = (_commandLength + 1) + i;
      if (((int)_command[_index]) < 48 || ((int)_command[_index]) > 57)
        return TextToInteger(_buf);
      _buf[i] = _command[_index];
    }
    return TextToInteger(_buf);
  }

#pragma endregion
#pragma region Commands

  void RunCommand(const char* _command) {

    if (CompareString(_command, "goto", 4)) {
      int _num = GetNumAfterCommand(_command,4,0);
      highlightedLine = _num;
      int _wantedPos = (_num - (int)(lnBufferLength/2))-1;
      viewedLineNum = (
        (_num  <= maxLineNum && _num != 0) ? 
          _wantedPos > 0 ? 
            _wantedPos 
            : 0
        : viewedLineNum
      );
    }

    if (CompareString(_command, "close", 5))
      CloseWindow();
    if (CompareString(_command, "minimize", 8))
      MinimizeWindow();
    if (CompareString(_command, "fullscreen", 10))
      MaximizeWindow();
    if (CompareString(_command, "restore", 7))
      RestoreWindow();
    if (CompareString(_command, "set monitor",11))
      SetWindowMonitor(GetNumAfterCommand(_command,11,0));

  }

#pragma endregion

int main(void) {
  
  /* Decide Whether or not to use the Custom Title bar 
   - ( No worky on Ubuntu cuz Wayland :( ) 
   - ( Could use macro but im lazy ) */
  if (useCustomTitleBar) SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_UNDECORATED);
  else SetConfigFlags(FLAG_WINDOW_RESIZABLE);

  InitWindow(1280, 720, "Text Editor");
  SetExitKey(KEY_NULL);
  SetTargetFPS(60);
  
  #pragma region Window Function Buttons

    bool _closeHover, 
        _minimizeHover, 
        _fullscreenHover, 
        _moveHover;

  #pragma endregion
  #pragma region Custom Font 

    float _fontSize = 20.0f;
    int _lineSpaceing = 200;
    Font _font = LoadFontEx (
      "/home/zbinden/Coding/TextEditor/src/assets/Font.ttf", 
      32, 0, 250
    );
    SetTextureFilter(_font.texture, TEXTURE_FILTER_BILINEAR);

  #pragma endregion
  #pragma region Text Editor
  
    char _textBuffer[1];
    int _cursorIndex = 0;

  #pragma endregion
  #pragma region Command Palette

    char _paletteBuffer[40]; 
    int _paletteCursorIndex = 0;
    bool _commandPaletteOpen = false;
    Color _paletteColor = (Color){20,20,20,255};

  #pragma endregion

  while (!WindowShouldClose()) {

    #pragma region Window Functions

      if (useCustomTitleBar) {

        _closeHover = CheckCollisionPointCircle(GetMousePosition(), (Vector2){PercentX(1.0f)-14.0f,14.0f}, 7.0f); 
        _fullscreenHover = CheckCollisionPointCircle(GetMousePosition(), (Vector2){PercentX(1.0f)-35.0f,14.0f}, 7.0f); 
        _minimizeHover = CheckCollisionPointCircle(GetMousePosition(), (Vector2){PercentX(1.0f)-56.0f,14.0f}, 7.0f); 
        _moveHover = CheckCollisionPointCircle(GetMousePosition(), (Vector2){PercentX(1.0f)-77.0f,14.0f}, 7.0f); 

        if (_closeHover && IsMouseButtonPressed(0))
            CloseWindow();
        if (_fullscreenHover && IsMouseButtonPressed(0))
          if (IsWindowMaximized())
            RestoreWindow();
          else
            MaximizeWindow();
        if (_minimizeHover && IsMouseButtonPressed(0))
            MinimizeWindow();
        if (_moveHover && IsMouseButtonDown(0)) {
          Vector2 _mouseDelta = GetMouseDelta();
          Vector2 _windowPos = GetWindowPosition();
          SetWindowPosition((_windowPos.x + _mouseDelta.x),(_windowPos.y + _mouseDelta.y));
        }
      
      }

    #pragma endregion
    #pragma region Command Palette
      if (IsKeyPressed(KEY_P) && IsKeyDown(KEY_LEFT_CONTROL)) {
        _paletteCursorIndex = 0;
        for(size_t i = 0; i < sizeof(_paletteBuffer); ++i)
          _paletteBuffer[i] = 0;
        _commandPaletteOpen = !_commandPaletteOpen;
      }
      
      if (!_commandPaletteOpen) {

        /* Zoom Controls */ {
          if (IsKeyPressed(KEY_EQUAL) && IsKeyDown(KEY_LEFT_CONTROL))
            if (_fontSize < 60.0f)
              _fontSize+=5.0f;
          if (IsKeyPressed(KEY_MINUS) && IsKeyDown(KEY_LEFT_CONTROL))
            if (_fontSize > 15.0f)
              _fontSize-=5.0f;
        }
        /* Scroll Controls */ {
          if (GetMouseWheelMove() < 0 || IsKeyDown(KEY_DOWN) && IsKeyDown(KEY_LEFT_CONTROL))
            if (viewedLineNum < maxLineNum-1)
              viewedLineNum++;
          if (GetMouseWheelMove() > 0 || IsKeyDown(KEY_UP) && IsKeyDown(KEY_LEFT_CONTROL) )
            if (viewedLineNum > 0)
              viewedLineNum--;
        }

      } else {

        int _key = GetCharPressed();
        if (_key >=32 && _key <= 125 && _paletteCursorIndex < 39) {
          _paletteBuffer[_paletteCursorIndex] = (char)_key;
          _paletteCursorIndex++;
        }

        if (IsKeyDown(KEY_BACKSPACE) && TimerDone(_backspaceTimer)) {
          if (_paletteCursorIndex > 0) {
            _paletteCursorIndex--;
            _paletteBuffer[_paletteCursorIndex] = 0;
            StartTimer(&_backspaceTimer, 0.1);
          }
        }

        if (IsKeyPressed(KEY_ENTER)) {
          _paletteCursorIndex = 0;
          RunCommand(_paletteBuffer);
          for(size_t i = 0; i < sizeof(_paletteBuffer); ++i)
            _paletteBuffer[i] = 0;
          _commandPaletteOpen = false;
          
        }

        if (IsKeyPressed(KEY_ESCAPE)) _commandPaletteOpen = false;

      }

    #pragma endregion

    // Renderer Block ( its a block :0  )
    BeginDrawing(); {

      ClearBackground((Color){ 13, 13, 13, 255 });

      #pragma region Draw Window Functions 
        if (useCustomTitleBar) {
          DrawCircle(PercentX(1.0f)-14.0f, 14.0f, 7.0f, _closeHover ? DARKGRAY : RED);
          DrawCircle(PercentX(1.0f)-35.0f, 14.0f, 7.0f, _fullscreenHover ? DARKGRAY : GREEN);
          DrawCircle(PercentX(1.0f)-56.0f, 14.0f, 7.0f, _minimizeHover ? DARKGRAY : YELLOW);
          DrawCircle(PercentX(1.0f)-77.0f, 14.0f, 7.0f, _moveHover ? BLACK : GRAY);
        }
      #pragma endregion
      #pragma region Draw Line Numbers
        if (maxLineNum > 0) {
          int _wantedLength = (int)((PercentY(1.0f) / (_fontSize + 4)) * 7.0f);
          const int _length = ((_wantedLength % 2) == 1) ? _wantedLength : _wantedLength + 1;  
          assert(maxLineNum <= 99999);
          for (int l = 7; l < _length; l+=7) {
            char _num[9]; 
            int _ln = (int)((l/7)) + (viewedLineNum-1);
            if (_ln < maxLineNum) {
              sprintf(_num, "| %d", _ln+1);
              Color _color = (_ln+1 == highlightedLine) ? (Color){255, 255, 255, 255} : (Color){50,50,50,255};
              DrawTextEx(_font, _num, (Vector2){ 10.0f, ((_fontSize + 6) * ((l/7)-1)) + 10}, _fontSize, 2, _color);
            }
          }
        }
        DrawRectangleRec((Rectangle){(80.0f + (_fontSize * 2)),5.0f,2.0f,PercentY(1.0f)}, (Color){17,17,17,255});
      #pragma endregion
      #pragma region Draw Information Bar 

        DrawRectangle(0, GetRenderHeight()-20, GetRenderWidth(), 20, WHITE);
        DrawTextEx(_font, "-- INSERT -- | ", (Vector2){5.0f, GetRenderHeight()-20}, 20.0f, 2, (Color){0,0,0,255});

        char _lineCount[19];
        sprintf(_lineCount, "Lines : %d |", maxLineNum);
        DrawTextEx(_font, _lineCount, (Vector2){125.0f, GetRenderHeight()-20}, 20.0f, 2, (Color){0,0,0,255});

        char _shownLineCount[22];
        float _lineCountspacing = 
          (maxLineNum < 10) ? 8.0f : 
          (maxLineNum < 100) ? 17.0f : 
          (maxLineNum < 1000) ? 30.0f :
          (maxLineNum < 10000) ? 40.0f :
          52.0f;
        lnBufferLength = ((int)(PercentY(1.0f) / (_fontSize + 4)));
        sprintf(_shownLineCount,"LN Buf Len : %d", lnBufferLength);
        DrawTextEx(_font, _shownLineCount, (Vector2){212.0f + _lineCountspacing, GetRenderHeight()-20}, 20.0f, 2, (Color){0,0,0,255});

      #pragma endregion
      #pragma region Draw Command Palette 
        if (_commandPaletteOpen) {

          DrawRectangleRounded((Rectangle){PercentX(.5f) - 201.0f, 14.0f, 402.0f, 32.0f}, 1.0f, 0, WHITE); // Outline
          DrawRectangleRounded((Rectangle){PercentX(.5f) - 200.0f, 15.0f, 400.0f, 30.0f}, 1.0f, 0, _paletteColor); // Fill

          DrawTextEx(_font, _paletteBuffer, (Vector2){PercentX(.5f) - 170.0f, 22.0f},16.0f,2,(Color){150,150,150,255});
          DrawTextEx(_font, "+", (Vector2){PercentX(.5f) - 190.0f, 20.0f},20.0f,2,(Color){255,255,255,255});
          DrawTextEx(_font, "| [ENTER]", (Vector2){PercentX(.5f) + 120.0f, 21.0f},16.0f,2,(Color){60,60,60,255});

        }
      #pragma endregion

    } EndDrawing();
    
  }

  UnloadFont(_font);
  CloseWindow();
  
  return 0;

}

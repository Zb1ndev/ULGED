# Ultra Light-weight Graphical Editor ( ULGED ) 
ULGED is a Graphical Text / Code Editor written in C using [Raylib](https://github.com/raysan5/raylib). It features many of the comforts of normal and more popular editors
inside a smaller package. The entire application can be used without the need of a mouse, and it's use is actually recommended with just
a keyboard. 

## Controls
- Zoom In / Out : ```CTRL + '='``` ```CTRL + '-'```
- Scroll Up / Down : ```CTRL + 'UP'``` ```CTRL + 'DOWN'``` or ```SCROLL_WHEEL``` 
- Open Command Palette : ```CTRL + 'P'``` 

## Commands
The command palette features a plethura of commands that can control everything from window properties to navigation. List of available commands : **Note : all commands are non-k-sensitive, though need to have a space in between arguments, also commands marked with an * are not supported by Wayland**
- ```Goto``` + ``` Line Number ``` : Goes to the requested **Line Number**
- ```Close``` : Closes the Editor
- ```Minimize``` : Minimizes the Editor
- ```Fullscreen``` : Maximizes the Editor
- ```Restore``` : Resores the Editor to its original size ( use after ```fullscreen``` )
- ```Set Monitor``` + ``` Monitor ID ``` * : Sets the monitor that renders the window to the **Monitor ID** 

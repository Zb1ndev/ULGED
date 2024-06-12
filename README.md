# Ultra Lightweight Graphical Editor ( ULGED ) 
ULGED is a Graphical Text / Code Editor written in C using Raylib. It features many of the comforts of normal and more popular editors
inside a smaller package. The entire application can be used without the need of a mouse, and it's use is actually recommended with just
a keyboard. 

## Controls
- Zoom In / Out : ```CTRL + '='``` ```CTRL + '-'```
- Scroll Up / Down : ```CTRL + 'UP'``` ```CTRL + 'DOWN'``` or ```SCROLL_WHEEL``` 
- Open Command Palette : ```CTRL + 'P'``` 

## Commands
The command palette features a plethura of commands that can control everything from window properties to navigation. List of available commands : **Note : all commands are non-k-sensitive, though need to have a space in between arguments**
- ```goto``` + ``` Line Number ``` : Goes to the requested **Line Number**
- ```close``` : Closes the Editor
- ```minimize``` : Minimizes the Editor
- ```fullscreen``` : Maximizes the Editor
- ```restore``` : Resores the Editor to its original size ( use after ```fullscreen``` )
- ```set monitor``` + ``` Monitor ID ``` : Sets the monitor that renders the window to the **Monitor ID** 

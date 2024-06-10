#include <iostream>
#include <vector>
#include <string>

using namespace std;

const string DIR = "/home/zbinden/Coding/TextEditor/";
vector<string> commands;

void RunCMDs() {

  string _buildCMD = "";
  size_t _cmdCount = commands.size();
  for (size_t c = 0; c < _cmdCount; c++) {
    _buildCMD += commands[c];
  }

  system(_buildCMD.c_str());
  system((DIR + "build/TextEditor").c_str());

}

void AddCMD(string _cmd) {
  commands.push_back((_cmd + (char)(0x20)));
}

void AddCMD(string _flag, string _args) {
  string _cmd = _flag + (char)(0x20) + _args + (char)(0x20);
  commands.push_back(_cmd);
}

int main() {

  AddCMD("gcc");
  AddCMD("-o", DIR + "build/TextEditor");

  AddCMD(DIR + "src/main.c");
  
  AddCMD("-l", "raylib");
  AddCMD("-lGL");
  AddCMD("-lm");
  AddCMD("-ldl");
  AddCMD("-lX11");

  RunCMDs();

}
/*  DYNAMO:- Event driven molecular dynamics simulator     http://www.marcusbannerman.co.uk/dynamo    Copyright (C) 2010  Marcus N Campbell Bannerman <m.bannerman@gmail.com>    This program is free software: you can redistribute it and/or    modify it under the terms of the GNU General Public License    version 3 as published by the Free Software Foundation.    This program is distributed in the hope that it will be useful,    but WITHOUT ANY WARRANTY; without even the implied warranty of    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the    GNU General Public License for more details.    You should have received a copy of the GNU General Public License    along with this program.  If not, see <http://www.gnu.org/licenses/>.*/#pragma once#define GL_GLEXT_PROTOTYPES#include <GL/glew.h>#include <GL/freeglut.h>#include <coil/coilWindow.hpp>#include <map>class CoilMaster{  private:    static void CallBackDisplayFunc();  static void CallBackKeyboardFunc(unsigned char key, int x, int y);  static void CallBackKeyboardUpFunc(unsigned char key, int x, int y);  static void CallBackMotionFunc(int x, int y);  static void CallBackMouseFunc(int button, int state, int x, int y);  static void CallBackMouseWheelFunc(int button, int dir, int x, int y);  static void CallBackPassiveMotionFunc(int x, int y);  static void CallBackReshapeFunc(int w, int h);   static void CallBackSpecialFunc(int key, int x, int y);     static void CallBackSpecialUpFunc(int key, int x, int y);     static void CallBackVisibilityFunc(int visible);      CoilMaster(int argc, char** argv);  ~CoilMaster();    std::map<int,CoilWindow*> _viewPorts;public:  static void CallBackIdleFunc();     inline static   CoilMaster& getInstance(int argc = 0, char** argv = NULL)  {    static CoilMaster singletonInstance(argc, argv);    return singletonInstance;  }    void  CallGlutCreateWindow(const char * setTitle, CoilWindow * glutWindow);  void  startMainLoop();};
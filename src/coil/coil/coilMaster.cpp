/*  DYNAMO:- Event driven molecular dynamics simulator     http://www.marcusbannerman.co.uk/dynamo    Copyright (C) 2010  Marcus N Campbell Bannerman <m.bannerman@gmail.com>    This program is free software: you can redistribute it and/or    modify it under the terms of the GNU General Public License    version 3 as published by the Free Software Foundation.    This program is distributed in the hope that it will be useful,    but WITHOUT ANY WARRANTY; without even the implied warranty of    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the    GNU General Public License for more details.    You should have received a copy of the GNU General Public License    along with this program.  If not, see <http://www.gnu.org/licenses/>.*/#include <coil/coilMaster.hpp>#include <coil/coilWindow.hpp>#include <GL/freeglut_ext.h>#include <stdexcept>                         CoilMaster::CoilMaster(int argc, char** argv){  if (!argc || (argv == NULL))    throw std::runtime_error("You must pass argc and argv the first time you use CoilMaster::getInstance()");  glutInit(&argc, argv);}CoilMaster::~CoilMaster(){} void CoilMaster::CallBackDisplayFunc(void){   int windowID = glutGetWindow();   CoilMaster::getInstance()._viewPorts[windowID]->CallBackDisplayFunc();}void CoilMaster::CallBackKeyboardFunc(unsigned char key, int x, int y){   int windowID = glutGetWindow();   CoilMaster::getInstance()._viewPorts[windowID]->CallBackKeyboardFunc(key, x, y);}void CoilMaster::CallBackKeyboardUpFunc(unsigned char key, int x, int y){   int windowID = glutGetWindow();   CoilMaster::getInstance()._viewPorts[windowID]->CallBackKeyboardUpFunc(key, x, y);}void CoilMaster::CallBackMotionFunc(int x, int y){   int windowID = glutGetWindow();   CoilMaster::getInstance()._viewPorts[windowID]->CallBackMotionFunc(x, y);}void CoilMaster::CallBackMouseFunc(int button, int state, int x, int y){   int windowID = glutGetWindow();   CoilMaster::getInstance()._viewPorts[windowID]->CallBackMouseFunc(button, state, x, y);}void CoilMaster::CallBackMouseWheelFunc(int button, int dir, int x, int y){   int windowID = glutGetWindow();   CoilMaster::getInstance()._viewPorts[windowID]->CallBackMouseWheelFunc(button, dir, x, y);}void CoilMaster::CallBackPassiveMotionFunc(int x, int y){   int windowID = glutGetWindow();   CoilMaster::getInstance()._viewPorts[windowID]->CallBackPassiveMotionFunc(x, y);}void CoilMaster::CallBackReshapeFunc(int w, int h){   int windowID = glutGetWindow();   CoilMaster::getInstance()._viewPorts[windowID]->CallBackReshapeFunc(w, h);}void CoilMaster::CallBackSpecialFunc(int key, int x, int y){   int windowID = glutGetWindow();   CoilMaster::getInstance()._viewPorts[windowID]->CallBackSpecialFunc(key, x, y);}   void CoilMaster::CallBackSpecialUpFunc(int key, int x, int y){   int windowID = glutGetWindow();   CoilMaster::getInstance()._viewPorts[windowID]->CallBackSpecialUpFunc(key, x, y);}   void CoilMaster::CallBackVisibilityFunc(int visible){   int windowID = glutGetWindow();   CoilMaster::getInstance()._viewPorts[windowID]->CallBackVisibilityFunc(visible);}void CoilMaster::CallGlutCreateWindow(const char * setTitle, CoilWindow * coilWindow){   // Open new window, record its windowID ,    int windowID = glutCreateWindow(setTitle);   coilWindow->SetWindowID(windowID);   // Store the address of new window in global array    // so CoilMaster can send events to propoer callback functions.   CoilMaster::getInstance()._viewPorts[windowID] = coilWindow;   // Hand address of universal static callback functions to Glut.   // This must be for each new window, even though the address are constant.   glutDisplayFunc(CallBackDisplayFunc);      //Idling is handled in coilMasters main loop   glutIdleFunc(NULL);   glutKeyboardFunc(CallBackKeyboardFunc);   glutKeyboardUpFunc(CallBackKeyboardUpFunc);   glutSpecialFunc(CallBackSpecialFunc);   glutSpecialUpFunc(CallBackSpecialUpFunc);   glutMouseFunc(CallBackMouseFunc);   glutMouseWheelFunc(CallBackMouseWheelFunc);   glutMotionFunc(CallBackMotionFunc);   glutPassiveMotionFunc(CallBackPassiveMotionFunc);   glutReshapeFunc(CallBackReshapeFunc);    glutVisibilityFunc(CallBackVisibilityFunc);}void CoilMaster::startMainLoop(){  while (1)    for (std::map<int,CoilWindow*>::iterator iPtr = _viewPorts.begin();	 iPtr != _viewPorts.end(); ++iPtr)      {	glutSetWindow(iPtr->first);	glutMainLoopEvent();	iPtr->second->CallBackIdleFunc();      }}
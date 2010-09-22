/*  DYNAMO:- Event driven molecular dynamics simulator 
    http://www.marcusbannerman.co.uk/dynamo
    Copyright (C) 2010  Marcus N Campbell Bannerman <m.bannerman@gmail.com>

    This program is free software: you can redistribute it and/or
    modify it under the terms of the GNU General Public License
    version 3 as published by the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "Triangles.hpp"
#include <iostream>

RTriangles::RTriangles():
  _colBuffSize(0),
  _posBuffSize(0),
  _normBuffSize(0),
  _elementBuffSize(0)
{}

RTriangles::~RTriangles()
{
  if (_colBuffSize)
    glDeleteBuffersARB(1, &_colBuff);

  if (_posBuffSize)
    glDeleteBuffersARB(1, &_posBuff);

  if (_normBuffSize)
    glDeleteBuffersARB(1, &_normBuff);

  if (_elementBuffSize)
    glDeleteBuffersARB(1, &_elementBuff);
}

void 
RTriangles::glRender()
{
  glBindBufferARB(GL_ARRAY_BUFFER, _colBuff);
  glColorPointer(4, GL_FLOAT, 0, 0);  
  
  glBindBufferARB(GL_ARRAY_BUFFER, _posBuff);
  glVertexPointer(3, GL_FLOAT, 0, 0);
  
  glBindBufferARB(GL_ARRAY_BUFFER, _normBuff);
  glNormalPointer(GL_FLOAT, 0, 0);
  
  glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, _elementBuff);
  
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_COLOR_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY); 
  
  switch (_RenderMode)
    {
    case TRIANGLES:
      glDrawElements(GL_TRIANGLES, _elementBuffSize, GL_UNSIGNED_INT, 0);
      break;
    case LINES:
      glDrawElements(GL_LINES, _elementBuffSize, GL_UNSIGNED_INT, 0);
      break;
    case POINTS:
      glDrawElements(GL_POINTS, _elementBuffSize, GL_UNSIGNED_INT, 0);
      break;
    }
 
  glDisableClientState(GL_VERTEX_ARRAY);	
  glDisableClientState(GL_COLOR_ARRAY);	
  glDisableClientState(GL_NORMAL_ARRAY);
}

void 
RTriangles::setGLColors(std::vector<float>& VertexColor)
{
  if (!VertexColor.size())
    throw std::runtime_error("VertexColor.size() == 0!");

  if (VertexColor.size() % 4)
    throw std::runtime_error("VertexColor.size() is not a multiple of 4!");

  if (_posBuffSize)
    if ((VertexColor.size() / 4) != (_posBuffSize / 3))
      throw std::runtime_error("VertexColor.size()/4 != posBuffSize/3");

  if (_colBuffSize)
    glDeleteBuffers(1, &_colBuff);

  _colBuffSize = VertexColor.size();
  
  glGenBuffersARB(1, &_colBuff);
  
  glBindBufferARB(GL_ARRAY_BUFFER, _colBuff);
  glBufferDataARB(GL_ARRAY_BUFFER, VertexColor.size() * sizeof(float), &VertexColor[0], 
	       GL_STREAM_DRAW);
}

void 
RTriangles::setGLPositions(std::vector<float>& VertexPos)
{
  if (!VertexPos.size())
    throw std::runtime_error("VertexPos.size() == 0!");
  
  if (VertexPos.size() % 3)
    throw std::runtime_error("VertexPos.size() is not a multiple of 3!");

  if (_colBuffSize)
    if ((_colBuffSize / 4) != (VertexPos.size() / 3))
      throw std::runtime_error("VertexPos.size()/3 != colBuffSize/4 ");
  
  if (_normBuffSize)
    if (_normBuffSize != VertexPos.size())
      throw std::runtime_error("VertexPos.size() != normBuffSize!");

  if (_posBuffSize)
    glDeleteBuffers(1, &_posBuff);

  _posBuffSize = VertexPos.size();

  glGenBuffersARB(1, &_posBuff);

  glBindBufferARB(GL_ARRAY_BUFFER, _posBuff);
  glBufferDataARB(GL_ARRAY_BUFFER, VertexPos.size() * sizeof(float), &VertexPos[0], 
	       GL_STREAM_DRAW);
}

void 
RTriangles::initOCLVertexBuffer(cl::Context& Context, bool _hostTransfers)
{
  _clbuf_Positions = cl::GLBuffer(Context, CL_MEM_READ_WRITE, _posBuff, GL_ARRAY_BUFFER,
				  _hostTransfers);
}

void 
RTriangles::initOCLColorBuffer(cl::Context& Context, bool _hostTransfers)
{
  _clbuf_Colors = cl::GLBuffer(Context, CL_MEM_READ_WRITE, _colBuff, GL_ARRAY_BUFFER,
			       _hostTransfers);
}
void 
RTriangles::initOCLNormBuffer(cl::Context& Context, bool _hostTransfers)
{
  _clbuf_Normals = cl::GLBuffer(Context, CL_MEM_READ_WRITE, _normBuff, GL_ARRAY_BUFFER,
			       _hostTransfers);
}
void 
RTriangles::initOCLElementBuffer(cl::Context& Context, bool _hostTransfers)
{
  _clbuf_Elements = cl::GLBuffer(Context, CL_MEM_READ_WRITE, _normBuff, GL_ELEMENT_ARRAY_BUFFER,
				 _hostTransfers);
}


void 
RTriangles::setGLNormals(std::vector<float>& VertexNormals)
{
  if (!VertexNormals.size())
    throw std::runtime_error("VertexNormals.size() == 0!");

  if (VertexNormals.size() % 3)
    throw std::runtime_error("VertexNormals.size() is not a multiple of 3!");

  if (_posBuffSize)
    if (VertexNormals.size() != _posBuffSize)
      throw std::runtime_error("VertexNormals.size() != posBuffsize!");

  if (_normBuffSize)
    glDeleteBuffers(1, &_normBuff);

  _normBuffSize = VertexNormals.size();

  glGenBuffersARB(1, &_normBuff);

  glBindBufferARB(GL_ARRAY_BUFFER, _normBuff);
  glBufferDataARB(GL_ARRAY_BUFFER, VertexNormals.size() * sizeof(float), &VertexNormals[0], 
	       GL_STATIC_DRAW);
}

void 
RTriangles::setGLElements(std::vector<int>& Elements)
{
  if (!Elements.size())
    throw std::runtime_error("Elements.size() == 0!");

  if (Elements.size() % 3) 
    throw std::runtime_error("Elements.size() is not a multiple of 3!");

  if (_elementBuffSize)
    glDeleteBuffers(1, &_elementBuff);
  
  _elementBuffSize = Elements.size();

  glGenBuffersARB(1, &_elementBuff);

  glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, _elementBuff);
  glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER, Elements.size() * sizeof(int), &Elements[0], 
	       GL_STATIC_DRAW);
}
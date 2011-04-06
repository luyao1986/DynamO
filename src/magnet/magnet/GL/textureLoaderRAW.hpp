/*    DYNAMO:- Event driven molecular dynamics simulator 
 *    http://www.marcusbannerman.co.uk/dynamo
 *    Copyright (C) 2009  Marcus N Campbell Bannerman <m.bannerman@gmail.com>
 *
 *    This program is free software: you can redistribute it and/or
 *    modify it under the terms of the GNU General Public License
 *    version 3 as published by the Free Software Foundation.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once

#include <magnet/GL/texture.hpp>
#include <magnet/math/vector.hpp>
#include <magnet/clamp.hpp>

namespace magnet {
  namespace GL {
    namespace detail {
      //Allows simpler programming in calcVolData when accessing the
      //volume out of bounds it returns a 0 val
      inline size_t coordCalc(GLint x, GLint y, GLint z, 
			      GLint width, GLint height, GLint depth)
      {
	x = clamp(x, 0, width  - 1);
	y = clamp(y, 0, height - 1);
	z = clamp(z, 0, depth  - 1);
	return x + width * (y + height * z);
      }
    }

    //! Loads RAW volume data from a file into a Texture3D. 
    //
    // Load fails if the file is not big enough to fill the passed
    //texture
    inline void loadVolumeFromRawFile(std::string filename, Texture3D& tex, size_t bytes = 1)
    {
      GLint _width = tex.getWidth();
      GLint _height = tex.getHeight();
      GLint _depth = tex.getDepth();

      std::ifstream file(filename.c_str(), std::ifstream::binary);
	
      std::vector<unsigned char> inbuffer(_width * _height * _depth);

      switch (bytes)
	{
	case 1:
	  {
	    file.read(reinterpret_cast<char*>(&inbuffer[0]), inbuffer.size());
	    if (file.fail()) M_throw() << "Failed to load the texture from the file";
	  }
	  break;
	case 2:
	  {
	    std::vector<uint16_t> tempBuffer(_width * _height * _depth);
	    file.read(reinterpret_cast<char*>(&tempBuffer[0]), 2 * tempBuffer.size());
	    if (file.fail()) M_throw() << "Failed to load the texture from the file";
	    for (size_t i(0); i < tempBuffer.size(); ++i)
	      inbuffer[i] = uint8_t(tempBuffer[i] >> 8);
	  }
	  break;
	default:
	  M_throw() << "Cannot load at that bit depth yet";
	}

      std::vector<GLubyte> voldata(4 * _width * _height * _depth);
      
      for (GLint z(0); z < _depth; ++z)
	for (GLint y(0); y < _height; ++y)
	  for (GLint x(0); x < _width; ++x)
	    {
	      Vector sample1(inbuffer[detail::coordCalc(x - 1, y, z, _width, _height, _depth)],
			     inbuffer[detail::coordCalc(x, y - 1, z, _width, _height, _depth)],
			     inbuffer[detail::coordCalc(x, y, z - 1, _width, _height, _depth)]);

	      Vector sample2(inbuffer[detail::coordCalc(x + 1, y, z, _width, _height, _depth)],
			     inbuffer[detail::coordCalc(x, y + 1, z, _width, _height, _depth)],
			     inbuffer[detail::coordCalc(x, y, z + 1, _width, _height, _depth)]);
		
	      //Note, we store the negative gradient (we point down
	      //the slope)
	      Vector grad = sample1 - sample2;

	      float nrm = grad.nrm();
	      if (nrm > 0) grad /= nrm;
		
	      size_t coord = x + _width * (y + _height * z);
	      voldata[4 * coord + 0] = uint8_t((grad[0] * 0.5 + 0.5) * 255);
	      voldata[4 * coord + 1] = uint8_t((grad[1] * 0.5 + 0.5) * 255);
	      voldata[4 * coord + 2] = uint8_t((grad[2] * 0.5 + 0.5) * 255);
	      voldata[4 * coord + 3] 
		= inbuffer[detail::coordCalc(x, y, z, _width, _height, _depth)];
	    }
      
//      //Gaussian blur the system
//      const double weights[5] = {0.0544886845,0.244201342,0.4026199469,0.244201342,0.0544886845};
//
//      for (size_t component(0); component < 3; ++component)
//	for (GLint z(0); z < _depth; ++z)
//	  for (GLint y(0); y < _height; ++y)
//	    for (GLint x(0); x < _width; ++x)
//	      {
//		double sum(0);
//		for (int i(0); i < 5; ++i)
//		  {
//		    GLint pos[3] = {x,y,z};
//		    pos[component] += i - 2;
//		    sum += weights[i] 
//		      * voldata[4 * detail::coordCalc(pos[0], pos[1], pos[2], 
//						      _width, _height, _depth) + component];
//		  }
//		voldata[4 * detail::coordCalc(x, y, z, _width, _height, _depth) 
//			+ component] = sum;
//	      }
//      
//      //Renormalize the gradients
//      for (GLint z(0); z < _depth; ++z)
//	for (GLint y(0); y < _height; ++y)
//	  for (GLint x(0); x < _width; ++x)
//	    {
//	      std::vector<GLubyte>::iterator iPtr = voldata.begin()
//		+ 4 * detail::coordCalc(x, y, z, _width, _height, _depth);
//	      
//	      Vector grad(*(iPtr + 0) - 128.0, *(iPtr + 1) - 128.0, *(iPtr + 2) - 128.0);
//	      float nrm = grad.nrm();
//	      if (nrm > 0) grad /= nrm;
//
//	      *(iPtr + 0) = uint8_t((grad[0] * 0.5 + 0.5) * 255);
//	      *(iPtr + 1) = uint8_t((grad[1] * 0.5 + 0.5) * 255);
//	      *(iPtr + 2) = uint8_t((grad[2] * 0.5 + 0.5) * 255);
//	    }

      tex.subImage(voldata, GL_RGBA);
    }
  }
}

	//Sphere test pattern
	//for (size_t z(0); z < _depth; ++z)
	//  for (size_t y(0); y < _height; ++y)
	//    for (size_t x(0); x < _width; ++x)
	//      inbuffer[x + _width * (y + _height * z)] 
	//	= (std::sqrt(  std::pow(x - _width / 2.0, 2) 
	//		     + std::pow(y - _height / 2.0, 2) 
	//		     + std::pow(z - _depth / 2.0, 2))
	//		     < 122.0) ? 255.0 : 0;

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

#pragma once

#include <gtkmm.h>
#include "filter.hpp"
#include <magnet/GL/SSAO.hpp>
#include <magnet/GL/SSAOBlur.hpp>

namespace coil 
{
  class SSAOWrapper: public filter
  {
  public:
    SSAOWrapper();
    ~SSAOWrapper();

    inline virtual size_t type_id() { return detail::filterEnum<SSAOWrapper>::val; }    
    inline virtual bool isEditable() { return true; }
    inline virtual void invoke(GLuint colorTextureUnit, size_t width, size_t height);

    inline virtual bool needsNormalDepth()  { return true; }
    virtual void edit();

  protected:
    magnet::GL::SSAO _filter;
    Glib::RefPtr<Gtk::Builder> _refXml;
    GLfloat _radius;
    GLfloat _totStrength;
    GLfloat _strength;
    GLfloat _offset;
    GLfloat _falloff;

    GLuint _randomTexture;

    void settingsCallback();
  };

  class SSAOBlurWrapper: public filter
  {
  public:
    SSAOBlurWrapper();
    ~SSAOBlurWrapper();

    inline virtual size_t type_id() { return detail::filterEnum<SSAOBlurWrapper>::val; }    
    inline virtual bool isEditable() { return true; }
    inline virtual void invoke(GLuint colorTextureUnit, size_t width, size_t height);

    inline virtual bool needsNormalDepth()  { return true; }
    virtual void edit();

  protected:
    magnet::GL::SSAOBlur _filter;
    Glib::RefPtr<Gtk::Builder> _refXml;
    GLfloat _radius;
    GLfloat _totStrength;
    GLfloat _strength;
    GLfloat _offset;
    GLfloat _falloff;

    GLuint _randomTexture;

    void settingsCallback();
  };
}
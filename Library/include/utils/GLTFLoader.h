/*
    This file is a part of Stonefish.

    Stonefish is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Stonefish is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

//
//  GeometryFileUtil.h
//  Stonefish
//
//  Created by Patryk Cieslak on 22/11/2018.
//  Copyright (c) 2018-2019 Patryk Cieslak. All rights reserved.
//

#ifndef __GLTFLOADER__
#define __GLTFLOADER__

#include "StonefishCommon.h"
#include "graphics/OpenGLDataStructs.h"

namespace sf
{

    //! A function to load geometry from a STL file.
    /*!
     \param path a path to the file
     \param scale a scale to apply to the data
     \return a pointer to an allocated mesh structure
     */
    Mesh *LoadGLTF(const std::string &path, GLfloat scale);

}

#endif

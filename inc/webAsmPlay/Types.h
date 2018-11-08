/**
╭━━━━╮╱╱╱╱╱╱╱╱╱╭╮╱╭━━━╮╱╱╱╱╱╱╭╮
┃╭╮╭╮┃╱╱╱╱╱╱╱╱╱┃┃╱┃╭━╮┃╱╱╱╱╱╱┃┃
╰╯┃┃╰╯╭━╮╭━━╮╭╮┃┃╱┃┃╱╰╯╭━━╮╭━╯┃╭━━╮
╱╱┃┃╱╱┃╭╯┃╭╮┃┣┫┃┃╱┃┃╱╭╮┃╭╮┃┃╭╮┃┃┃━┫
╱╱┃┃╱╱┃┃╱┃╭╮┃┃┃┃╰╮┃╰━╯┃┃╰╯┃┃╰╯┃┃┃━┫
╱╱╰╯╱╱╰╯╱╰╯╰╯╰╯╰━╯╰━━━╯╰━━╯╰━━╯╰━━╯
 // This software is provided 'as-is', without any express or implied
 // warranty.  In no event will the authors be held liable for any damages
 // arising from the use of this software.
 // Permission is granted to anyone to use this software for any purpose,
 // including commercial applications, and to alter it and redistribute it
 // freely, subject to the following restrictions:
 // 1. The origin of this software must not be misrepresented; you must not
 //    claim that you wrote the original software. If you use this software
 //    in a product, an acknowledgment in the product documentation would be
 //    appreciated but is not required.
 // 2. Altered source versions must be plainly marked as such, and must not be
 //    misrepresented as being the original software.
 // 3. This notice may not be removed or altered from any source distribution.

  \author Matthew Tang
  \email trailcode@gmail.com
  \copyright 2018
*/

#ifndef __WEB_ASM_PLAY_TYPES_H__
#define __WEB_ASM_PLAY_TYPES_H__

#include <tuple>
#include <vector>
#include <memory>

class Attributes;

namespace geos
{
    namespace geom
    {
        class Geometry;
        class Polygon;
        class LineString;
        class Point;
    }
}

typedef std::tuple<double, double, double, double> AABB2D;

typedef std::pair<Attributes *, geos::geom::Geometry   *> AttributedGeometry;
typedef std::pair<Attributes *, geos::geom::LineString *> AttributedLineString;
typedef std::pair<Attributes *, geos::geom::Point      *> AttributedPoint;

typedef std::pair<const geos::geom::Geometry *, const size_t> ColoredGeometry;

typedef std::vector<ColoredGeometry> ColoredGeometryVec;

typedef std::tuple<const geos::geom::Geometry *, const size_t, const double> ColoredExtrudedGeometry;

typedef std::vector<ColoredExtrudedGeometry> ColoredExtrudedGeometryVec; 

typedef std::vector<const geos::geom::Geometry *> ConstGeosGeomVec;

typedef std::vector<float> FloatVec;

typedef std::vector<uint32_t> Uint32Vec;

typedef std::vector<std::string> StrVec;

#endif // __WEB_ASM_PLAY_TYPES_H__


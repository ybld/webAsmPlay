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
#pragma once

#include <functional>
#include <string>
#include <tuple>
#include <vector>
#include <memory>
#include <glm/vec2.hpp>
#include <boost/geometry.hpp>

#ifndef __EMSCRIPTEN__

#include <tbb/concurrent_queue.h>

//typedef tbb::concurrent_queue<std::function<void ()>> EventQueue;
//mutable 
//typedef boost::lockfree::queue<std::function<void(int id)> *> EventQueue;
//#include <boost/lockfree/queue.hpp>
//typedef boost::lockfree::queue<std::function<void()> *> EventQueue;

typedef tbb::concurrent_queue<std::function<void()> *> EventQueue;

#endif

class Attributes;
class Edge;
class Renderable;

namespace geos
{
    namespace geom
    {
        class Geometry;
        class Polygon;
        class LineString;
        class Point;
        class MultiPolygon;
    }
}

namespace boostGeom
{
	typedef double CoordinateType;
	typedef boost::geometry::model::d2::point_xy<CoordinateType> Point;
	typedef boost::geometry::model::box<Point> Box;
	typedef boost::geometry::model::linestring<Point> LineString;
	typedef boost::geometry::model::polygon<Point> Polygon;
	typedef boost::geometry::model::multi_polygon<Polygon> MultiPolygon;
}

typedef std::tuple<double, double, double, double> AABB2D;

typedef std::pair<Attributes *, geos::geom::Geometry   *> AttributedGeometry;
typedef std::pair<Attributes *, geos::geom::LineString *> AttributedLineString;
typedef std::pair<Attributes *, geos::geom::Point      *> AttributedPoint;

typedef std::tuple<Attributes *, geos::geom::Polygon *, double> AttributedPoligonalArea;

inline Attributes          *& attrs(AttributedPoligonalArea & v) { return std::get<0>(v) ;}
inline geos::geom::Polygon *& poly (AttributedPoligonalArea & v) { return std::get<1>(v) ;}
inline double               & area (AttributedPoligonalArea & v) { return std::get<2>(v) ;}

inline Attributes          * attrs(const AttributedPoligonalArea & v) { return std::get<0>(v) ;}
inline geos::geom::Polygon * poly (const AttributedPoligonalArea & v) { return std::get<1>(v) ;}
inline double                area (const AttributedPoligonalArea & v) { return std::get<2>(v) ;}

typedef std::tuple<Attributes *, boostGeom::Polygon,	double> AttributedPoligonalAreaBoost;
typedef std::pair<Attributes *, boostGeom::LineString> AttributedLineStringBoost;

typedef std::pair<const geos::geom::Geometry *, const size_t> ColoredGeometry;

typedef std::pair<glm::dvec2, Edge *> PointOnEdge;

typedef std::vector<ColoredGeometry> ColoredGeometryVec;

typedef std::tuple<const geos::geom::Geometry *, const size_t, const double, const double> ColoredExtrudedGeometry;

typedef std::vector<ColoredExtrudedGeometry> ColoredExtrudedGeometryVec; 

typedef std::vector<const geos::geom::Geometry *> ConstGeosGeomVec;

typedef std::vector<float> FloatVec;

typedef std::vector<uint32_t> Uint32Vec;

typedef std::vector<std::string> StrVec;

typedef std::function<void ()> Updatable;
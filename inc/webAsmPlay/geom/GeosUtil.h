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

#include <vector>
#include <memory>
#include <initializer_list>
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <geos/geom/Point.h>
#include <geos/geom/LineString.h>
#include <geos/geom/Polygon.h>
#include <geos/geom/MultiPolygon.h>
#include <nlohmann/json.hpp>

#define _SCOPED_GEOS_GEOMETRY_COMBINE1(X, Y) X##Y
#define _SCOPED_GEOS_GEOMETRY_COMBINE(X, Y) _SCOPED_GEOS_GEOMETRY_COMBINE1(X, Y)

#define scopedGeosGeometry(geom) geom; _ScopedGeosGeometry _SCOPED_GEOS_GEOMETRY_COMBINE(scopedGeosGeom, __LINE__)(geom);

class _ScopedGeosGeometry
{
public:

    _ScopedGeosGeometry(geos::geom::Geometry * geom);
    
    ~_ScopedGeosGeometry();
    
private:

    geos::geom::Geometry * m_geom;
};

namespace geosUtil
{
    // TODO Inline a these!

    geos::geom::Geometry::Ptr makeBox(const glm::dvec2 & min, const glm::dvec2 & max);

    geos::geom::Geometry::Ptr makeWireBox(const glm::dvec2 & min, const glm::dvec2 & max);

    geos::geom::Geometry::Ptr unionPolygons(const std::initializer_list<geos::geom::Geometry::Ptr> & polys);

    std::vector<const geos::geom::LineString *> getExternalRings(const geos::geom::Geometry * geom);

    std::vector<const geos::geom::LineString *> getExternalRings(const geos::geom::Geometry::Ptr & geom);

    void getExternalRings(std::vector<const geos::geom::LineString *> & rings, const geos::geom::Geometry * geom);

    std::vector<geos::geom::Geometry::Ptr> __(const std::vector<const geos::geom::LineString *> & lineStrings);

    inline geos::geom::Coordinate ___(const glm::dvec2 & pos) { return geos::geom::Coordinate(pos.x, pos.y) ;}

    glm::dvec2 __(const geos::geom::Coordinate & point);

    std::vector<glm::dvec2> __(const std::vector<geos::geom::Coordinate> & coords);

    std::vector<glm::dvec2> __(const std::vector<geos::geom::Coordinate> * coords);

    std::vector<glm::dvec2> __(const std::unique_ptr<std::vector<geos::geom::Coordinate> > & coords);

    inline glm::dvec4 __(const geos::geom::Coordinate & v, const double z, const double w) { return glm::dvec4(v.x, v.y, z, w) ;}

    bool contains(const std::unique_ptr<geos::geom::Geometry> & A, const std::unique_ptr<geos::geom::Geometry> & B);

    bool contains(const geos::geom::Geometry * A, const geos::geom::Geometry * B);

    std::unique_ptr<geos::geom::Geometry> difference(const std::unique_ptr<geos::geom::Geometry> & A, const std::unique_ptr<geos::geom::Geometry> & B);
    
    bool difference(const std::unique_ptr<geos::geom::Geometry> & A,
                    const std::unique_ptr<geos::geom::Geometry> & B,
                    std::unique_ptr<geos::geom::Geometry>       & out);

    bool subtract(  geos::geom::Geometry       *& A,
                    const geos::geom::Geometry * B);

    bool subtract(  std::unique_ptr<geos::geom::Geometry>       & A,
                    const std::unique_ptr<geos::geom::Geometry> & B);

    glm::dvec2 getStartPoint(const geos::geom::LineString * ls);
    glm::dvec2 getEndPoint  (const geos::geom::LineString * ls);

    void transformInPlace(std::vector<geos::geom::Coordinate> & points, const glm::dmat4 & trans);

    geos::geom::LineString * getLineString(const std::vector<glm::dvec2> & verts);

    inline geos::geom::Coordinate append2f(float *& dataStream, const geos::geom::Coordinate & v)
    {
        *dataStream = (float)v.x; ++dataStream;
        *dataStream = (float)v.y; ++dataStream;

        return v;
    }

    inline geos::geom::Coordinate append2d(std::vector<double> & data, const geos::geom::Coordinate & v)
    {
        data.push_back(v.x);
        data.push_back(v.y);

        return v;
    }

	void addPoint(nlohmann::json& coordinates, const geos::geom::Point* P);

	void addLineString(nlohmann::json& coordinates, const geos::geom::LineString* lineString);

	nlohmann::json addLineString(const geos::geom::LineString* lineString);

	void addPolygon(nlohmann::json& coordinates, const geos::geom::Polygon* polygon);

	nlohmann::json addPolygon(const geos::geom::Polygon* polygon);

	std::string writeGeoJsonFile(const std::string& fileName, const std::vector<geos::geom::Geometry *> & geoms);

	std::string writeGeoJsonFile(const std::string& fileName, const std::vector<geos::geom::Geometry::Ptr> & geoms);

	std::string writeGeoJsonFile(const std::string& fileName, const std::vector<const geos::geom::Geometry *> & geoms);

	std::string writeGeoJsonFile(const std::string& fileName, const geos::geom::Geometry * geom);

	std::string writeGeoJsonFile(const std::string& fileName, geos::geom::Geometry::Ptr & geom);

	geos::geom::Point * getPoint(const glm::dvec2 & pos);

	inline geos::geom::Point		* geosPoint			(geos::geom::Geometry* geom) { return dynamic_cast<geos::geom::Point*>			(geom); }
	inline geos::geom::LineString	* geosLineString	(geos::geom::Geometry* geom) { return dynamic_cast<geos::geom::LineString*>		(geom); }
	inline geos::geom::Polygon		* geosPolygon		(geos::geom::Geometry* geom) { return dynamic_cast<geos::geom::Polygon*>		(geom); }
	inline geos::geom::MultiPolygon	* geosMultiPolygon	(geos::geom::Geometry* geom) { return dynamic_cast<geos::geom::MultiPolygon*>	(geom); }

	inline geos::geom::Point		* geosPoint			(geos::geom::Geometry::Ptr & geom) { return dynamic_cast<geos::geom::Point*>		(geom.get()); }
	inline geos::geom::LineString	* geosLineString	(geos::geom::Geometry::Ptr & geom) { return dynamic_cast<geos::geom::LineString*>	(geom.get()); }
	inline geos::geom::Polygon		* geosPolygon		(geos::geom::Geometry::Ptr & geom) { return dynamic_cast<geos::geom::Polygon*>		(geom.get()); }
	inline geos::geom::MultiPolygon	* geosMultiPolygon	(geos::geom::Geometry::Ptr & geom) { return dynamic_cast<geos::geom::MultiPolygon*>	(geom.get()); }

	inline const geos::geom::Point			* geosPointConst		(const geos::geom::Geometry* geom) { return dynamic_cast<const geos::geom::Point*>			(geom); }
	inline const geos::geom::LineString		* geosLineStringConst	(const geos::geom::Geometry* geom) { return dynamic_cast<const geos::geom::LineString*>		(geom); }
	inline const geos::geom::Polygon		* geosPolygonConst		(const geos::geom::Geometry* geom) { return dynamic_cast<const geos::geom::Polygon*>		(geom); }
	inline const geos::geom::MultiPolygon	* geosMultiPolygonConst	(const geos::geom::Geometry* geom) { return dynamic_cast<const geos::geom::MultiPolygon*>	(geom); }

	inline const geos::geom::Point			* geosPointConst		(const geos::geom::Geometry::Ptr & geom) { return dynamic_cast<const geos::geom::Point*>		(geom.get()); }
	inline const geos::geom::LineString		* geosLineStringConst	(const geos::geom::Geometry::Ptr & geom) { return dynamic_cast<const geos::geom::LineString*>	(geom.get()); }
	inline const geos::geom::Polygon		* geosPolygonConst		(const geos::geom::Geometry::Ptr & geom) { return dynamic_cast<const geos::geom::Polygon*>		(geom.get()); }
	inline const geos::geom::MultiPolygon	* geosMultiPolygonConst	(const geos::geom::Geometry::Ptr & geom) { return dynamic_cast<const geos::geom::MultiPolygon*>	(geom.get()); }
};

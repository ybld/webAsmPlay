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
#include <glm/mat4x4.hpp>
#include <geos/geom/Geometry.h>
#include <webAsmPlay/geom/BoostGeomUtil.h>
#include <webAsmPlay/Types.h>

class Shader;
class Canvas;
class VertexArrayObject;

class Renderable
{
public:

    typedef std::function<void (Renderable *)> OnDelete;

    static Renderable * create( const geos::geom::Geometry::Ptr		& geom,
                                const glm::dmat4					& trans			= glm::mat4(1.0),
                                const AABB2D						& boxUV			= AABB2D(),
								const bool							  swapUV_Axis   = false);

    static Renderable * create( const geos::geom::Geometry			* geom,
                                const glm::dmat4					& trans			= glm::mat4(1.0),
                                const AABB2D						& boxUV			= AABB2D(),
								const bool							  swapUV_Axis   = false);

	static Renderable * create( const boostGeom::Polygon			& polygon,
								const glm::dmat4					& trans         = glm::dmat4(1.0),
                                const size_t						  symbologyID   = 0,
                                const AABB2D						& boxUV         = AABB2D(),
								const bool							  swapUV_Axis   = false);

	static Renderable * create( const boostGeom::MultiPolygon		& multiPoly,
								const glm::dmat4					& trans         = glm::dmat4(1.0),
                                const size_t						  symbologyID   = 0,
                                const AABB2D						& boxUV         = AABB2D(),
								const bool							  swapUV_Axis   = false);

	virtual ~Renderable();

    virtual void render(Canvas * canvas, const size_t renderStage) = 0;

	virtual void render(const glm::mat4 & model,
						const glm::mat4 & view,
						const glm::mat4 & projection,
						const size_t	  renderStage);

    void addOnDeleteCallback(const OnDelete & callback);

    Shader * getShader() const;
    
	Renderable * setShader(Shader * shader);

    bool getRenderFill() const;
    bool getRenderOutline() const;

    Renderable * setRenderFill(const bool render);
    Renderable * setRenderOutline(const bool render);

	virtual void ensureVAO() {}

	size_t getNumTriangles() const;

//protected:

    Renderable( const bool isMulti          = false,
                const bool renderFill       = false,
                const bool renderOutline    = false);

	Renderable( VertexArrayObject	* vertexArrayObject,
                const bool			  renderFill			= false,
                const bool			  renderOutline			= false);

    Renderable(const Renderable &)              = delete;
    Renderable(      Renderable &&)             = delete;
    Renderable & operator=(const Renderable &)  = delete;
    
    std::vector<OnDelete> m_DeleteCallbacks;

    Shader * m_shader = nullptr;

	bool m_isMulti			= false;
    bool m_renderFill		= true;
    bool m_renderOutline	= true;

	VertexArrayObject * m_vertexArrayObject = nullptr;
};


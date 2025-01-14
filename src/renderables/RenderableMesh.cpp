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

#include <chrono>
#include <geos/geom/Polygon.h>
#include <geos/geom/MultiPolygon.h>
#include <geos/geom/LineString.h>
#include <webAsmPlay/Util.h>
#include <webAsmPlay/VertexArrayObject.h>
#include <webAsmPlay/shaders/Shader.h>
#include <webAsmPlay/renderables/RenderableMesh.h>

using namespace std;
using namespace std::chrono;
using namespace glm;
using namespace geos;
using namespace geos::geom;

Renderable * RenderableMesh::create(	const ColoredExtrudedGeometryVec & polygons,
										const dmat4                      & trans,
										const bool                         showProgress)
{
	return create(getTesselations(	polygons,
									trans,
									showProgress));
}

Tessellations RenderableMesh::getTesselations(	const ColoredExtrudedGeometryVec & polygons,
												const dmat4                      & trans,
												const bool                         showProgress)
{
	Tessellations tessellations;

	getTesselations(tessellations,
					polygons,
					trans,
					showProgress);

	return tessellations;
}

void RenderableMesh::getTesselations(	Tessellations					 & tessellations,
										const ColoredExtrudedGeometryVec & polygons,
										const dmat4                      & trans,
										const bool                         showProgress)
{
	time_point<system_clock> startTime;

	if(showProgress) { startTime = system_clock::now() ;}

    for(size_t i = 0; i < polygons.size(); ++i)
    {
        if(showProgress) { doProgress("(6/6) Creating geometry:", i, polygons.size(), startTime) ;}

        const auto geom        = get<0>(polygons[i]);
        const auto symbologyID = get<1>(polygons[i]);
        const auto height      = get<2>(polygons[i]);
        const auto minHeight   = get<3>(polygons[i]);
        
        if(auto poly = dynamic_cast<const geom::Polygon *>(geom))
        {
            tessellations.push_back(Tessellation::tessellatePolygon(poly, trans, symbologyID, height, minHeight));
            
            if((*tessellations.rbegin())->isEmpty())
            {
                dmess("Warning tessellation failed!");

                tessellations.pop_back();
            }
        }
        else if(auto multiPoly = dynamic_cast<const MultiPolygon *>(geom))	{ dmessError("Have a multiPoly!")						;}
        else																{ dmessError("Warning not a polygon or multi-polygon.") ;}
    }

    if(showProgress) { GUI::progress("", 1.0) ;}
}

Renderable * RenderableMesh::create(const Tessellations & tessellations)
{
    VertexArrayObject * vao = VertexArrayObject::create(tessellations);

    if(!vao)
    {
        dmess("Error! Could not create VertexArrayObject!");

        return nullptr;
    };

    return new RenderableMesh(vao);
}

RenderableMesh::~RenderableMesh() { delete m_vertexArrayObject ;}

RenderableMesh::RenderableMesh(VertexArrayObject * vertexArrayObject) : Renderable( true,
                                                                                    GUI::s_renderSettingsFillPolygons,
                                                                                    GUI::s_renderSettingsRenderPolygonOutlines),
                                                                        m_vertexArrayObject(vertexArrayObject)
{

}

void RenderableMesh::render(Canvas * canvas, const size_t renderStage)
{
	const bool renderFill		= getRenderFill()		&& m_shader->m_shouldRender(false,	renderStage);
	const bool renderOutline	= getRenderOutline()	&& m_shader->m_shouldRender(true,	renderStage);

	if(!renderFill && !renderOutline) { return ;}

    m_vertexArrayObject->bind(m_shader);
    m_vertexArrayObject->bindTriangles();

    if(renderFill)
    {
        m_shader->bind(canvas, false, renderStage);

        m_vertexArrayObject->drawTriangles();
    }

    if(renderOutline)
    {
        m_shader->bind(canvas, true, renderStage);

        m_vertexArrayObject->bindLines();
        
        m_vertexArrayObject->drawLines();
    }
}

void RenderableMesh::ensureVAO() { m_vertexArrayObject->ensureVAO() ;}
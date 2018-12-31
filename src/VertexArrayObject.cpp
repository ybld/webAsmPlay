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

#include <glm/gtx/normal.hpp>
#include <webAsmPlay/Util.h>
#include <webAsmPlay/shaders/Shader.h>
#include <webAsmPlay/VertexArrayObject.h>

using namespace std;
using namespace glm;

VertexArrayObject * VertexArrayObject::create(const Tessellations & tessellations)
{
    if(tessellations[0]->getHeight() != 0.0)
    {
        return _create< true, // 3D extrude
                        true, // Use symbology ID
                        false // Use UV coords
                      > (tessellations, AABB2D());
    }

    return _create< false, // 3D extrude
                    true,  // Use symbology ID
                    false  // Use UV coords
                  > (tessellations, AABB2D());
}

VertexArrayObject * VertexArrayObject::create(const Tessellations & tessellations, const AABB2D & boxUV)
{
    return _create< false, // 3D extrude
                    false, // Use symbology ID
                    true   // Use UV coords
                > (tessellations, boxUV);
}

namespace
{
    template <bool USE_SYMBOLOGY_ID>
    void addVert(   FloatVec    & verts,
                    const vec3  & v,
                    const vec3  & n,
                    const float   c)
    {
        verts.push_back(v.x);
        verts.push_back(v.y);
        verts.push_back(v.z);

        verts.push_back(n.x);
        verts.push_back(n.y);
        verts.push_back(n.z);

        if(USE_SYMBOLOGY_ID) { verts.push_back(c) ;}
    }
}

template<bool IS_3D, bool USE_SYMBOLOGY_ID, bool USE_UV_COORDS>
VertexArrayObject * VertexArrayObject::_create(const Tessellations & tessellations, const AABB2D & boxUV)
{
    if(!tessellations.size()) { return NULL ;}

    FloatVec  verts;
    Uint32Vec triangleIndices;
    Uint32Vec counterVertIndices;
    Uint32Vec lineIndices;

    size_t offset = 0;

    size_t symbologyID_Stride;

    if(IS_3D) { symbologyID_Stride = 8 ;}
    else      { symbologyID_Stride = 4 ;}

    for(const auto & tess : tessellations)
    {
        // TODO try to remove hard coded values.
        const float symbologyID_value     = (float(tess->symbologyID * symbologyID_Stride) + 0.5) / 32.0;
        const float symbologyWallID_value = (float(tess->symbologyID * symbologyID_Stride) + 0.5) / 32.0 + 4.0 / 32.0;

        for(size_t i = 0; i < tess->numVerts; ++i)
        {
            const dvec2 P(tess->verts[i * 2 + 0], tess->verts[i * 2 + 1]);

            verts.push_back(P.x);
            verts.push_back(P.y);

            if(IS_3D)  
            {
                verts.push_back(tess->height);

                verts.push_back(0);
                verts.push_back(0);
                verts.push_back(1);
            }

            if(USE_SYMBOLOGY_ID) { verts.push_back(symbologyID_value) ;}

            if(USE_UV_COORDS)
            {
                const dvec2 min(get<0>(boxUV), get<1>(boxUV));
                const dvec2 max(get<2>(boxUV), get<3>(boxUV));

                const dvec2 uv = (P - min) / (max - min);

                verts.push_back(uv.x);
                verts.push_back(uv.y);
            }
        }

        for(size_t i = 0; i < tess->numTriangles * 3; ++i)
        {
            triangleIndices.push_back(tess->triangleIndices[i] + offset);
        }

        for(size_t i = 0; i < tess->numVerts; ++i)
        {
            lineIndices.push_back(i + offset);
            lineIndices.push_back((i + 1) % tess->numVerts + offset);
        }

        offset += tess->numVerts;

        if(!IS_3D) { continue ;}

        for(size_t a = 0; a < tess->numVerts; ++a)
        {
            const size_t b = (a + 1) % tess->numVerts;

            const vec2 A(tess->verts[a * 2], tess->verts[a * 2 + 1]);
            const vec2 B(tess->verts[b * 2], tess->verts[b * 2 + 1]);

            const vec3 p1(A, tess->height);
            const vec3 p2(B, tess->height);
            const vec3 p3(B, tess->minHeight);
            const vec3 p4(A, tess->minHeight);

            const vec3 normal = normalize(triangleNormal(p1, p2, p3));

            addVert<USE_SYMBOLOGY_ID>(verts, p1, normal, symbologyWallID_value);
            addVert<USE_SYMBOLOGY_ID>(verts, p2, normal, symbologyWallID_value);
            addVert<USE_SYMBOLOGY_ID>(verts, p3, normal, symbologyWallID_value);
            addVert<USE_SYMBOLOGY_ID>(verts, p4, normal, symbologyWallID_value);

            triangleIndices.push_back(offset + 0);
            triangleIndices.push_back(offset + 1);
            triangleIndices.push_back(offset + 2);

            triangleIndices.push_back(offset + 2);
            triangleIndices.push_back(offset + 3);
            triangleIndices.push_back(offset);

            lineIndices.push_back(offset + 0);
            lineIndices.push_back(offset + 3);

            offset += 4;
        }
    }

    GLuint vao  = 0;
    GLuint ebo  = 0;
    GLuint ebo2 = 0;
    GLuint vbo  = 0;
    
    GL_CHECK(glGenVertexArrays(1, &vao));
    GL_CHECK(glBindVertexArray(    vao));

    GL_CHECK(glGenBuffers(1, &vbo));
    GL_CHECK(glGenBuffers(1, &ebo));
    GL_CHECK(glGenBuffers(1, &ebo2));

    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, vbo));

    GL_CHECK(glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(GLfloat), &verts[0], GL_STATIC_DRAW));
    
    GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo));
    GL_CHECK(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * triangleIndices.size(), &triangleIndices[0], GL_STATIC_DRAW));
    
    GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo2));
    GL_CHECK(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * lineIndices.size(), &lineIndices[0], GL_STATIC_DRAW));

    //IS_3D, bool USE_SYMBOLOGY_ID, bool USE_UV_COORDS

    size_t sizeVertex = 2;
    size_t sizeColor  = 0;
    size_t sizeNormal = 0;
    size_t sizeUV     = 0;

    if(IS_3D)
    {
        sizeVertex = 3;
        sizeNormal = 3;
    }

    if(USE_SYMBOLOGY_ID)
    {
        sizeColor = 1;
    }

    if(USE_UV_COORDS)
    {
        sizeUV = 2;
    }

    const size_t totalSize = (sizeVertex + sizeColor + sizeNormal + sizeUV) * sizeof(GLfloat);

    ArrayFormat vertexFormat(sizeVertex, totalSize, 0);
    ArrayFormat normalFormat(sizeNormal, totalSize, (void *)(sizeVertex * sizeof(GLfloat)));
    ArrayFormat colorFormat (sizeColor,  totalSize, (void *)((sizeVertex + sizeNormal) * sizeof(GLfloat)));
    ArrayFormat uvFormat    (sizeUV,     totalSize, (void *)((sizeVertex + sizeNormal + sizeColor) * sizeof(GLfloat)));

    return new VertexArrayObject(vao,
                                 ebo,
                                 ebo2,
                                 vbo,
                                 triangleIndices.size(),
                                 counterVertIndices,
                                 lineIndices.size(),
                                 tessellations.size() > 1,
                                 vertexFormat,
                                 colorFormat,
                                 normalFormat,
                                 uvFormat);
}

VertexArrayObject::VertexArrayObject(   const GLuint        vao,
                                        const GLuint        ebo,
                                        const GLuint        ebo2,
                                        const GLuint        vbo,
                                        const GLuint        numTrianglesIndices,
                                        const Uint32Vec   & counterVertIndices,
                                        const size_t        numContourLines,
                                        const bool          isMulti,
                                        const ArrayFormat & vertexFormat,
                                        const ArrayFormat & colorFormat,
                                        const ArrayFormat & normalFormat,
                                        const ArrayFormat & uvFormat) : vao                (vao),
                                                                        ebo                (ebo),
                                                                        ebo2               (ebo2),
                                                                        vbo                (vbo),
                                                                        numTrianglesIndices(numTrianglesIndices),
                                                                        counterVertIndices (counterVertIndices),
                                                                        numContourLines    (numContourLines),
                                                                        _isMulti           (isMulti),
                                                                        vertexFormat       (vertexFormat),
                                                                        colorFormat        (colorFormat),
                                                                        normalFormat       (normalFormat),
                                                                        uvFormat           (uvFormat)
{

}

VertexArrayObject::~VertexArrayObject()
{
    GL_CHECK(glDeleteVertexArrays(1, &vao));
    GL_CHECK(glDeleteBuffers     (1, &vbo));
    GL_CHECK(glDeleteBuffers     (1, &ebo));
    GL_CHECK(glDeleteBuffers     (1, &ebo2));
}

void VertexArrayObject::bind(Shader * shader) const
{
    GL_CHECK(glBindVertexArray(vao));
    
    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, vbo));

    shader->setVertexArrayFormat(vertexFormat);

    if(colorFormat.size)
    {
        shader->setColorArrayFormat(colorFormat);
    }

    if(normalFormat.size)
    {
        shader->setNormalArrayFormat(normalFormat);
    }

    if(uvFormat.size)
    {
        shader->setUV_ArrayFormat(uvFormat);
    }
}

void VertexArrayObject::bindTriangles() const
{
    GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo));
}

void VertexArrayObject::bindLines() const
{
    GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo2));
}

void VertexArrayObject::drawTriangles() const
{
    GL_CHECK(glDrawElements(GL_TRIANGLES, numTrianglesIndices, GL_UNSIGNED_INT, NULL));
}

void VertexArrayObject::drawLines() const
{
    GL_CHECK(glDrawElements(GL_LINES, numContourLines, GL_UNSIGNED_INT, NULL));
}

bool VertexArrayObject::isMulti() const { return _isMulti ;}
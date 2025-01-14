﻿/**
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
\copyright 2019
*/
// Modified from: https://github.com/JoeyDeVries/LearnOpenGL

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <webAsmPlay/Debug.h>
#include <webAsmPlay/shaders/PhongShader.h>
#include <webAsmPlay/renderables/Mesh.h>

using namespace std;


/*  Functions  */
// constructor
Mesh::Mesh(	const vector<Vertex>		& vertices,
			const vector<unsigned int>	& indices,
			const vector<Texture>		& textures,
			const Material				& material) :	m_vertices	(vertices),
														m_indices	(indices),
														m_textures	(textures),
														m_material	(material)
{
    // now that we have all the required data, set the vertex buffers and its attribute pointers.
    setupMesh();
}

// render the mesh
void Mesh::draw(const SetMaterialFunctor & onMaterial)
{
	ensureVAO();

    // bind appropriate textures
    unsigned int diffuseNr  = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr   = 1;
    unsigned int heightNr   = 1;
    for(unsigned int i = 0; i < m_textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
        // retrieve texture number (the N in diffuse_textureN)
        string number;
        string name = m_textures[i].m_type;
        if(name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if(name == "texture_specular")
			number = std::to_string(specularNr++); // transfer unsigned int to stream
        else if(name == "texture_normal")
			number = std::to_string(normalNr++); // transfer unsigned int to stream
            else if(name == "texture_height")
			number = std::to_string(heightNr++); // transfer unsigned int to stream

		// now set the sampler to the correct texture unit
        // and finally bind the texture
        glBindTexture(GL_TEXTURE_2D, m_textures[i].m_ID);
    }

	if(!m_textures.size()) { onMaterial(m_material) ;}
        
    // draw mesh
    glBindVertexArray(m_VAO);

    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);

    // always good practice to set everything back to defaults once configured.
    glActiveTexture(GL_TEXTURE0);
}

/*  Functions    */
// initializes all the buffer objects/arrays
void Mesh::setupMesh()
{
    // create buffers/arrays
    
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), &m_indices[0], GL_STATIC_DRAW);	
    
    // load data into vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    // A great thing about structs is that their memory layout is sequential for all its items.
    // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
    // again translates to 3/2 floats which translates to a byte array.
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW);  

	// Flush is required if executing in a thread different from the main thread.
	glFlush();
}

void Mesh::ensureVAO()
{
	if(m_VAO) { return ;}

	glGenVertexArrays(1, &m_VAO);

	glBindVertexArray(m_VAO);
	
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

    // set the vertex attribute pointers
    // vertex Positions
    glEnableVertexAttribArray(0);	
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);	
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);	
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
    // vertex tangent
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
    // vertex bitangent
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));

    glBindVertexArray(0);
}
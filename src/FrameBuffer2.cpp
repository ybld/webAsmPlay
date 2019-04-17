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

#include <webAsmPlay/FrameBuffer2.h>

using namespace std;
using namespace glm;

FrameBuffer2::FrameBuffer2(	const ivec2					& bufferSize,
							const vector<FB_Component>	& components)
{
	glGenFramebuffers(1,				&m_renderFBO);
	glBindFramebuffer(GL_FRAMEBUFFER,	 m_renderFBO);
	
	m_textures.resize(components.size());

	glGenTextures(components.size(), &m_textures[0]);

	for (size_t i = 0; i < components.size(); ++i)
	{
		glBindTexture(GL_TEXTURE_2D, m_textures[i]);

		glTexStorage2D(GL_TEXTURE_2D, 1, components[i].m_dataType, bufferSize.x, bufferSize.y);

		for (const auto& texParam : components[i].m_textureParameters) { glTexParameteri(GL_TEXTURE_2D, get<0>(texParam), get<1>(texParam)) ;}

		glFramebufferTexture(GL_FRAMEBUFFER, components[i].m_type, m_textures[i], 0);

		if (components[i].m_type != GL_DEPTH_COMPONENT32F) { m_drawBuffers.push_back(components[i].m_type) ;}
	}

	glDrawBuffers(m_drawBuffers.size(), &m_drawBuffers[0]);
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

FrameBuffer2::~FrameBuffer2()
{
	unbind();

	GL_CHECK(glDeleteTextures(m_textures.size(), &m_textures[0]));

	GL_CHECK(glDeleteFramebuffers(1, &m_renderFBO));
}

ivec2 FrameBuffer2::getBufferSize() const
{
	return m_bufferSize;
}

void FrameBuffer2::bind(const bool clear)
{
	GL_CHECK(glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &m_prevFB));

	GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, m_renderFBO));

	if(!clear) { return ;}

	GL_CHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void FrameBuffer2::unbind() { GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, m_prevFB)); }

GLuint FrameBuffer2::getTextureID(const size_t component) const { return m_textures[component] ;}

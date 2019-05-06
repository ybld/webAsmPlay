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

#version 430 core
uniform sampler2D tex;

// Per-vertex inputs
layout (location = 0) in vec4 position;
layout(location = 1) in float vertColorIn;



uniform mat4 MV;
uniform float colorLookupOffset;
//uniform mat4 proj_matrix;

layout (std140, binding = 0) uniform constants
{
	mat4 mv_matrix;
	mat4 view_matrix;
	mat4 proj_matrix;
};

// Inputs from vertex shader
out VS_OUT
{
	vec3 N;
	vec3 L;
	vec3 V;
} vs_out;

out vec4 vertexColorNear;
out vec4 vertexColorFar;
out vec4 position_in_view_space;
noperspective out vec4 fragCoord2D;

// Position of light
uniform vec3 light_pos = vec3(100.0, 100.0, 100.0);

void main(void)
{
	vec4 vert = vec4(position.xy, 0, 1);

	// Calculate view-space coordinate
	position_in_view_space = mv_matrix * vert;

	vertexColorNear = texture(tex, vec2(vertColorIn +		 colorLookupOffset  / 32.0, 0.5));
	vertexColorFar = texture(tex,  vec2(vertColorIn + (1.0 + colorLookupOffset) / 32.0, 0.5));

	// Calculate view-space coordinate
	vec4 P = mv_matrix * vert;

	// Calculate normal in view-space
	//vs_out.N = mat3(mv_matrix) * normal;
	//vs_out.N = vec3(0, 0, 1);
	vs_out.N = mat3(mv_matrix) * vec3(0, 0, 1);

	// Calculate light vector
	vs_out.L = light_pos - P.xyz;

	// Calculate view vector
	vs_out.V = -P.xyz;
	
	// Calculate the clip-space position of each vertex
	gl_Position = proj_matrix * P;

	fragCoord2D  = proj_matrix * mv_matrix * vec4(position.xy, 0, 1);

	// Vertex in NDC-space
	fragCoord2D.xyz /= fragCoord2D.w;       // Rescale: [-1,1]^3
	fragCoord2D.w    = 1.0 / fragCoord2D.w; // Invert W

											// Vertex in window-space
	fragCoord2D.xyz *= vec3(0.5) + vec3(0.5); // Rescale: [0,1]^3

	fragCoord2D.xyz += vec3(1);

	fragCoord2D.xyz *= vec3(0.5);
}
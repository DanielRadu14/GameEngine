#pragma once

#include <sstream>
#include "Renderer.h"
#include "GameEngine/Application.h"

namespace GameEngine 
{
	std::string vertexCode;
	std::string fragmentCode;

	void static ImportShaders(Renderer *renderer)
	{
		vertexCode = R"(
			#version 330

			layout(location = 0) in vec3 v_position;
			layout(location = 1) in vec3 v_normal;

			// Uniform properties
			uniform mat4 Model;
			uniform mat4 View;
			uniform mat4 Projection;

			// Uniforms for light properties
			uniform vec3 light_position;
			uniform vec3 eye_position;
			uniform float material_kd;
			uniform float material_ks;
			uniform int material_shininess;

			uniform vec3 object_color;

			out vec3 color;

			void main()
			{
				vec3 world_pos = (Model * vec4(v_position, 1)).xyz;
				vec3 world_normal = normalize(mat3(Model) * v_normal);

				vec3 L = normalize(light_position - world_pos);
				vec3 V = normalize(eye_position - world_pos);
				vec3 H = normalize(L + V);

				float ambient_light = material_kd * 0.25;

				float diffuse_light = material_kd * max(dot(world_normal, L), 0);

				float rez = dot(world_normal, V);
				int primesteLumina = 1;
				if (rez < 0) {
					primesteLumina = 0;
				}

				float specular_light = 0;

				if (diffuse_light > 0)
				{
					specular_light = material_ks * primesteLumina * pow(max(dot(world_normal, H), 0), material_shininess);
				}

				float factorAtenuare = 500 / (distance(light_position, world_pos)*distance(light_position, world_pos));

				color = ambient_light + factorAtenuare * (diffuse_light + specular_light) * object_color;

				gl_Position = Projection * View * Model * vec4(v_position, 1.0);
			}

		)";

		fragmentCode = R"(
			#version 330
	
			in vec3 color;

			layout(location = 0) out vec4 out_color;

			void main()
			{
				out_color = vec4(color, 1.0);
			}

		)";

		renderer->shaders["IlluminatedShader"] = new Shader(vertexCode, fragmentCode);

		vertexCode = R"(
			#version 330

			layout(location = 0) in vec3 v_position;
			layout(location = 1) in vec3 v_normal;

			// Uniform properties
			uniform mat4 Model;
			uniform mat4 View;
			uniform mat4 Projection;

			uniform vec3 object_color;

			out vec3 color;

			void main()
			{

				color = object_color;

				gl_Position = Projection * View * Model * vec4(v_position, 1.0);
			}

		)";

		fragmentCode = R"(
			#version 330
	
			in vec3 color;

			layout(location = 0) out vec4 out_color;

			void main()
			{
				out_color = vec4(color, 1.0);
			}

		)";

		renderer->shaders["BasicShader"] = new Shader(vertexCode, fragmentCode);
	}
}
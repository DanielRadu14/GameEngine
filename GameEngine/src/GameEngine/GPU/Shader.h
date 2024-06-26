#pragma once

#include "string";

namespace GameEngine {

	class Shader 
	{
	public:
		Shader(const std::string& vertexCode, const std::string& fragmentCode);
		~Shader();

		void Bind() const;
		void Unbind() const;

	public:
		uint32_t program;
	};

}
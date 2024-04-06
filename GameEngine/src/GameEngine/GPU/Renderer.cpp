#include "Renderer.h"
#include "glad/glad.h"
#include <fstream> 
#include <iostream>
#include <sstream>
using namespace std;

namespace GameEngine
{
	Renderer* Renderer::s_Instance = nullptr;
	Renderer::Renderer()
	{
		s_Instance = this;
	}

	Renderer::~Renderer()
	{

	}

	std::vector<std::string> split(const std::string &s, char delim) {
		std::vector<std::string> result;
		std::stringstream ss(s);
		std::string item;

		while (getline(ss, item, delim)) {
			result.push_back(item);
		}

		return result;
	}

	Mesh* Renderer::LoadMesh(std::string name, std::string filePath)
	{
		std::vector<float> vertices;
		std::vector<unsigned int> indices;
		std::vector<glm::vec3> normals;

		ifstream inputFile(filePath);

		if (!inputFile.is_open()) {
			cerr << "Error opening the file!" << endl;
			return nullptr;
		}

		string line;
		while (getline(inputFile, line)) {

			std::vector<std::string> words = split(line, ' ');

			if (words.size() > 0 && words[0] == "v")
			{
				for (int i = 1; i < words.size(); i++)
				{
					try {
						vertices.push_back(std::stof(words[i]));
					}
					catch (...)
					{

					}
				}
			}
			else if (words.size() > 0 && words[0] == "f")
			{
				int count = 0;
				for (int i = 1; i < words.size(); i++)
				{
					std::vector<std::string> index = split(words[i], '/');
					try
					{
						if (count == 3)
						{
							int x = indices[indices.size() - 3];
							int y = indices[indices.size() - 1];
							indices.push_back(x);
							indices.push_back(y);
						}
						indices.push_back(stoi(index[0]) - 1);
						count++;
					}
					catch (...)
					{

					}
				}
			}
			if (words.size() > 0 && words[0] == "vn")
			{
				glm::vec3 normal;
				
				try {
					normal.x = std::stof(words[1]);
					normal.y = std::stof(words[2]);
					normal.z = std::stof(words[3]);
					normals.push_back(normal);
				}
				catch (...)
				{

				}
			}
		}

		inputFile.close();		
		return CreateMesh(name, vertices, indices, normals);
	}

	Mesh* Renderer::CreateMesh(std::string name, ::vector<float> vertices, std::vector<unsigned int> indices)
	{
		unsigned int VAO;
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		unsigned int VBO;
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

		unsigned int IBO;
		glGenBuffers(1, &IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		return new Mesh(name, vertices, indices, VAO);
	}

	Mesh* Renderer::CreateMesh(std::string name, ::vector<float> vertices, std::vector<unsigned int> indices, std::vector<glm::vec3> normals)
	{
		unsigned int VAO;
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		unsigned int VBO;
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		unsigned int VBO2;
		glGenBuffers(1, &VBO2);
		glBindBuffer(GL_ARRAY_BUFFER, VBO2);
		glBufferData(GL_ARRAY_BUFFER, sizeof(normals[0]) * normals.size(), &normals[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

		unsigned int IBO;
		glGenBuffers(1, &IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);	

		return new Mesh(name, vertices, indices, VAO);

	}

	void Renderer::RenderMesh(Mesh *mesh, Shader* shader, const glm::mat4 &modelMatrix, const glm::vec3 &color)
	{
		shader->Bind();

		Application* application = &Application::Get();
		Camera camera = application->GetCamera();

		int location = glGetUniformLocation(shader->program, "light_position");
		glUniform3f(location, application->lightPosition.x, application->lightPosition.y, application->lightPosition.z);

		location = glGetUniformLocation(shader->program, "eye_position");
		glUniform3f(location, camera.position.x, camera.position.y, camera.position.z);

		location = glGetUniformLocation(shader->program, "material_kd");
		glUniform1f(location, materialKd);

		location = glGetUniformLocation(shader->program, "material_ks");
		glUniform1f(location, materialKs);

		location = glGetUniformLocation(shader->program, "material_shininess");
		glUniform1i(location, materialShininess);

		location = glGetUniformLocation(shader->program, "object_color");
		glUniform3f(location, color.r, color.g, color.b);

		location = glGetUniformLocation(shader->program, "Model");
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(modelMatrix));

		location = glGetUniformLocation(shader->program, "View");
		glm::mat4 viewMatrix = camera.GetViewMatrix();
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));

		location = glGetUniformLocation(shader->program, "Projection");
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(application->GetProjectionMatrix()));

		glBindVertexArray(meshes[mesh->name]->GetVertexArray());
		glDrawElements(GL_TRIANGLES, static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
	}
}
#include "Renderer.h"
#include "glad/glad.h"
#include <fstream> 
#include <iostream>
#include <sstream>
using namespace std;

namespace GameEngine
{
	Renderer::Renderer()
	{

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
		}

		inputFile.close();		
		return CreateMesh(name, vertices, indices);
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
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

		return new Mesh(name, vertices, indices, VAO);
	}

	void Renderer::RenderMesh(Mesh *mesh, Shader* shader, const glm::mat4 &modelMatrix)
	{
		shader->Bind();

		Application* application = &Application::Get();
		Camera camera = application->GetCamera();

		int location = glGetUniformLocation(shader->program, "Model");
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
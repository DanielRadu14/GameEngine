#include "Renderer.h"
#include "glad/glad.h"

namespace GameEngine
{
	Renderer::Renderer()
	{

	}

	Renderer::~Renderer()
	{

	}

	Mesh* Renderer::CreateMesh(std::vector<float> vertices, std::vector<unsigned int> indices)
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

		return new Mesh(vertices, indices, VAO);
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

		glBindVertexArray(meshes["triangle"]->GetVertexArray());
		glDrawElements(GL_TRIANGLES, static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
	}
}
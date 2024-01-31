#include <GameEngine.h>

class ExampleLayer : public GameEngine::Layer
{
public:
	ExampleLayer() : Layer("Example")
	{

	}

	void OnUpdate() override
	{
		printf("ExampleLayer::Update\n");
	}

	void OnEvent(GameEngine::Event& event) override
	{
		printf(event.ToString().c_str());
		printf("\n");
	}
};

class Sandbox : public GameEngine::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}
	~Sandbox()
	{

	}
};

GameEngine::Application* GameEngine::CreateApplication()
{
	return new Sandbox();
}
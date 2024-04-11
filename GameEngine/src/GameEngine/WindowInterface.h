#pragma once

#include "Events/Event.h"

namespace GameEngine {

	struct WindowProps
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;
		float AspectRatio;

		WindowProps(const std::string& title = "My GameEngine",
			unsigned int width = 1280,
			unsigned int height = 720)
			: Title(title), Width(width), Height(height), AspectRatio(float(width)/height)
		{
		}
	};

	class WindowInterface
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~WindowInterface() {}

		virtual void OnUpdate(float deltaTime) = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;
		virtual float GetAspectRatio() const = 0;

		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		static WindowInterface* Create(const WindowProps& props = WindowProps());
	};

}
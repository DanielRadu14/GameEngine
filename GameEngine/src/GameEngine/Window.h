#pragma once

#include "WindowInterface.h"

#include <GLFW/glfw3.h>

struct GLFWwindow;

namespace GameEngine {

	class Window : public WindowInterface
	{
	public:
		Window(const WindowProps& props);
		virtual ~Window();

		void OnUpdate(float deltaTime) override;

		inline unsigned int GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeight() const override { return m_Data.Height; }
		inline float GetAspectRatio() const override { return m_Data.AspectRatio; }

		inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

		inline virtual void* GetNativeWindow() const { return m_Window; }
	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();
	private:
		GLFWwindow* m_Window;

		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			float AspectRatio;
			bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};

}
#pragma once

namespace GameEngine
{
	class InputInterface
	{
	public:
		static bool IsKeyPressed(int keycode) {
			return instance->IsKeyPressedImpl(keycode);
		}

		static bool IsMouseBtnPressed(int button) {
			return instance->IsMouseBtnPressedImpl(button);
		}

		static float GetMouseX() {
			return instance->GetMouseXImpl();
		}

		static float GetMouseY() {
			return instance->GetMouseYImpl();
		}

	protected:
		virtual bool IsKeyPressedImpl(int keycode) = 0;
		virtual bool IsMouseBtnPressedImpl(int button) = 0;
		virtual float GetMouseXImpl() = 0;
		virtual float GetMouseYImpl() = 0;

	private:
		static InputInterface* instance;
	};
}
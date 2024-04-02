#pragma once

#include "InputInterface.h"

namespace GameEngine
{
	class Input : public InputInterface
	{
	protected:
		virtual bool IsKeyPressedImpl(int keycode) override;
		virtual bool IsMouseBtnPressedImpl(int button) override;
		virtual float GetMouseXImpl() override;
		virtual float GetMouseYImpl() override;
	};
}
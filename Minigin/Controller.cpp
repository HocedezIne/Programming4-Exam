#include "Controller.h"

#include <windows.h>
#include <Xinput.h>

namespace engine
{
	class Controller::Impl
	{
	public:
		Impl(const unsigned int index) : m_Index(index) {};
		~Impl() = default;

		Impl(const Impl& other) = default;
		Impl& operator= (const Impl& other) = delete;
		Impl(Impl&& other) = delete;
		Impl& operator= (Impl&& other) = delete;

		void PollInput()
		{
			CopyMemory(&m_PrevState, &m_State, sizeof(XINPUT_STATE));
			ZeroMemory(&m_State, sizeof(XINPUT_STATE));
			XInputGetState(m_Index, &m_State);

			uint8_t buttonChanges = static_cast<uint8_t>(m_State.Gamepad.wButtons ^ m_PrevState.Gamepad.wButtons);
			m_ButtonsPressedThisFrame = buttonChanges & m_State.Gamepad.wButtons;
			m_ButtonsReleasedThisFrame = buttonChanges & (~m_State.Gamepad.wButtons);
		};

		bool PressedThisFrame(Controller::Button button) const { return m_ButtonsPressedThisFrame & static_cast<int>(button); };
		bool ReleasedThisFrame(Controller::Button button) const { return m_ButtonsReleasedThisFrame & static_cast<int>(button); };
		bool HeldThisFrame(Controller::Button button) const { return m_State.Gamepad.wButtons & static_cast<int>(button); };

	private:
		const unsigned int m_Index;

		XINPUT_STATE m_State{};
		XINPUT_STATE m_PrevState{};

		uint8_t m_ButtonsPressedThisFrame{};
		uint8_t m_ButtonsReleasedThisFrame{};
	};

	Controller::Controller(const unsigned int index)
	{
		m_Impl = new Impl{ index };
	};

	Controller::~Controller()
	{
		delete m_Impl;
		m_Impl = nullptr;
	};

	void Controller::PollInput()
	{
		m_Impl->PollInput();
	}

	bool Controller::PressedThisFrame(Controller::Button button) const
	{
		return m_Impl->PressedThisFrame(button);
	}

	bool Controller::ReleasedThisFrame(Controller::Button button) const
	{
		return m_Impl->ReleasedThisFrame(button);
	}

	bool Controller::HeldThisFrame(Controller::Button button) const
	{
		return m_Impl->HeldThisFrame(button);
	}
}
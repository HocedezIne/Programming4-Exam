#include "Keyboard.h"

#include <algorithm>
#include <functional>

namespace engine
{
	Keyboard::Keyboard(const unsigned int index) : m_Index(index)
	{
		m_PrevState.reserve(SDL_NUM_SCANCODES);
		m_PrevState.assign(SDL_NUM_SCANCODES, 0);
	}

	void Keyboard::PollInput()
	{
		ClearState();

		auto keyboardState = SDL_GetKeyboardState(NULL);

		m_State.insert(m_State.end(), keyboardState, keyboardState + SDL_NUM_SCANCODES);

		std::vector<Uint8> changedKeys(SDL_NUM_SCANCODES);
		m_KeysPressedThisFrame.reserve(SDL_NUM_SCANCODES);
		m_KeysPressedThisFrame.assign(SDL_NUM_SCANCODES, 0);
		m_KeysReleasedThisFrame.reserve(SDL_NUM_SCANCODES);
		m_KeysReleasedThisFrame.assign(SDL_NUM_SCANCODES, 0);

		std::transform(m_State.begin(), m_State.end(), m_PrevState.begin(), changedKeys.begin(), std::bit_xor<Uint8>());
		std::transform(m_State.begin(), m_State.end(), changedKeys.begin(), m_KeysPressedThisFrame.begin(), std::bit_and<Uint8>());
		std::transform(m_State.begin(), m_State.end(), changedKeys.begin(), m_KeysReleasedThisFrame.begin(),
			[](Uint8 curr, Uint8 changed) { return static_cast<Uint8>(curr & ~changed); });

		std::copy(m_State.begin(), m_State.end(), m_PrevState.begin());
	}

	bool Keyboard::PressedThisFrame(SDL_Scancode key) const { return m_KeysPressedThisFrame[key] == 1; };
	bool Keyboard::ReleasedThisFrame(SDL_Scancode key) const { return m_KeysReleasedThisFrame[key] == 1; };
	bool Keyboard::HeldThisFrame(SDL_Scancode key) const { return m_State[key] == 1 && m_PrevState[key] == 1; };

	void Keyboard::ClearState()
	{
		m_State.clear();
		m_KeysPressedThisFrame.clear();
		m_KeysReleasedThisFrame.clear();
	}
}
#pragma once

#include <SDL.h>
#include <vector>

namespace engine
{
	class Keyboard
	{
	public:
		Keyboard(const unsigned int index);
		~Keyboard() = default;

		Keyboard(const Keyboard& other) = delete;
		Keyboard& operator=(const Keyboard& other) = delete;
		Keyboard(Keyboard&& other) = delete;
		Keyboard& operator=(Keyboard&& other) = delete;

		void PollInput();

		bool PressedThisFrame(SDL_Scancode key) const;
		bool ReleasedThisFrame(SDL_Scancode key) const;
		bool HeldThisFrame(SDL_Scancode key) const;

	private:
		void ClearState();

		const unsigned int m_Index;

		std::vector<Uint8> m_State{};
		std::vector<Uint8> m_PrevState{};

		std::vector<Uint8> m_KeysPressedThisFrame;
		std::vector<Uint8> m_KeysReleasedThisFrame;
	};
}
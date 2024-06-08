#pragma once

#include <map>

#include "Singleton.h"
#include "InputCommands.h"
#include "Controller.h"
#include "Keyboard.h"

namespace engine
{
	enum class KeyState
	{
		Pressed,
		Held,
		Released
	};

	class InputCommandLinker final : public Singleton<InputCommandLinker>
	{
	public:
		~InputCommandLinker() = default;

		bool ProcessInput();

		void AddKeyboardCommand(SDL_Scancode key, KeyState state, std::unique_ptr<InputCommand> command, unsigned int Id = 0);
		void AddControllerCommand(Controller::Button button, KeyState state, std::unique_ptr<InputCommand> command, unsigned int Id = 0);
		void RemoveKeyboardCommand(SDL_Scancode key, KeyState state, unsigned int Id = 0);
		void RemoveControllerCommand(Controller::Button button, KeyState state, unsigned int Id = 0);

		void AddKeyboard();
		void AddController();
		void RemoveKeyboard(unsigned int idx);
		void RemoveController(unsigned int idx);

		bool IsKeyDown(SDL_Scancode key);
		bool IsButtonDown(Controller::Button button);

	private:
		void ProcessKeyboardState();
		void ProcessControllerStates();

		std::vector<std::unique_ptr<Keyboard>> m_Keyboards;
		using KeyboardButtonState = std::pair< std::pair<unsigned int, SDL_Scancode>, KeyState>;
		using KeyboardCommands = std::map< KeyboardButtonState, std::unique_ptr<InputCommand> >;
		KeyboardCommands m_KeyboardCommands{};

		std::vector<std::unique_ptr<Controller>> m_Controllers;
		using ControllerButtonState = std::pair< std::pair<unsigned int, Controller::Button>, KeyState>;
		using ControllerCommands = std::map< ControllerButtonState, std::unique_ptr<InputCommand> >;
		ControllerCommands m_ControllerCommands{};
	};

}

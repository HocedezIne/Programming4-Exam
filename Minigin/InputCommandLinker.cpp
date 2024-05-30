#include <SDL.h>
#include "InputCommandLinker.h"

bool engine::InputCommandLinker::ProcessInput()
{
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			return false;
		}
	}

	ProcessKeyboardState();

	ProcessControllerStates();

	return true;
}

void engine::InputCommandLinker::AddKeyboardCommand(SDL_Scancode key, KeyState state, std::unique_ptr<InputCommand> command, unsigned int Id)
{
	KeyboardButtonState kbs = std::make_pair(std::make_pair(Id, key), state);
	m_KeyboardCommands.insert(std::make_pair(kbs, std::move(command)));
}

void engine::InputCommandLinker::AddControllerCommand(Controller::Button button, KeyState state, std::unique_ptr<InputCommand> command, unsigned int Id)
{
	ControllerButtonState cbs = std::make_pair(std::make_pair(Id, button), state);
	m_ControllerCommands.insert(std::make_pair(cbs, std::move(command)));
}

void engine::InputCommandLinker::RemoveKeyboardCommand(SDL_Scancode key, KeyState state, unsigned int Id)
{
	KeyboardButtonState kbs = std::make_pair(std::make_pair(Id, key), state);
	auto range = m_KeyboardCommands.equal_range(kbs);
	m_KeyboardCommands.erase(range.first, range.second);
}

void engine::InputCommandLinker::RemoveControllerCommand(Controller::Button button, KeyState state, unsigned int Id)
{
	ControllerButtonState cbs = std::make_pair(std::make_pair(Id, button), state);
	auto range = m_ControllerCommands.equal_range(cbs);
	m_ControllerCommands.erase(range.first, range.second);
}


void engine::InputCommandLinker::AddKeyboard()
{
	m_Keyboards.push_back(std::make_unique<Keyboard>(int(m_Keyboards.size())));
}

void engine::InputCommandLinker::AddController()
{
	m_Controllers.push_back(std::make_unique<Controller>(int(m_Controllers.size())));
}

bool engine::InputCommandLinker::IsKeyDown(SDL_Scancode key)
{
	for (int i{}; i < int(m_Keyboards.size()); ++i)
	{
		if (m_Keyboards[i]->PressedThisFrame(key))
			return true;
	}

	return false;
}

bool engine::InputCommandLinker::IsButtonDown(Controller::Button button)
{
	for (int i{}; i < int(m_Controllers.size()); ++i)
	{
		if (m_Controllers[i]->PressedThisFrame(button))
			return true;
	}

	return false;
}

void engine::InputCommandLinker::ProcessKeyboardState()
{
	for (int idx{}; idx < int(m_Keyboards.size()); ++idx)
	{
		m_Keyboards[idx]->PollInput();
	}

	for (const auto& pair : m_KeyboardCommands)
	{
		switch (pair.first.second)
		{
		case KeyState::Pressed:
			if (m_Keyboards[pair.first.first.first]->PressedThisFrame(pair.first.first.second))
			{
				pair.second->Execute();
			}
			break;
		case KeyState::Released:
			if (m_Keyboards[pair.first.first.first]->ReleasedThisFrame(pair.first.first.second))
			{
				pair.second->Execute();
			}
			break;
		case KeyState::Held:
			if (m_Keyboards[pair.first.first.first]->HeldThisFrame(pair.first.first.second))
			{
				pair.second->Execute();
			}
			break;
		}
	}
}

void engine::InputCommandLinker::ProcessControllerStates()
{
	for (int idx{}; idx < int(m_Controllers.size()); ++idx)
	{
		m_Controllers[idx]->PollInput();
	}

	for (const auto& pair : m_ControllerCommands)
	{
		switch (pair.first.second)
		{
		case KeyState::Pressed:
			if (m_Controllers[pair.first.first.first]->PressedThisFrame(pair.first.first.second))
			{
				pair.second->Execute();
			}
			break;
		case KeyState::Released:
			if (m_Controllers[pair.first.first.first]->ReleasedThisFrame(pair.first.first.second))
			{
				pair.second->Execute();
			}
			break;
		case KeyState::Held:
			if (m_Controllers[pair.first.first.first]->HeldThisFrame(pair.first.first.second))
			{
				pair.second->Execute();
			}
			break;
		}
	}
}

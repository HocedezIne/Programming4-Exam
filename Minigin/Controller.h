#pragma once
#include <memory>

namespace engine
{
	class Controller
	{
	public:
		enum class Button {
			DPadUp = 0x0001,
			DPadDown = 0x0002,
			DPadLeft = 0x0004,
			DPadRight = 0x0008,
			Start = 0x0010,
			Back = 0x0020,
			LeftThumb = 0x0040,
			RightThumb = 0x0080,
			LeftShoulder = 0x0100,
			RightShoulder = 0x0200,
			A = 0x1000,
			B = 0x2000,
			X = 0x4000,
			Y = 0x8000
		};

		Controller(const unsigned int index);
		~Controller();

		Controller(const Controller& other) = delete;
		Controller& operator=(const Controller& other) = delete;
		Controller(Controller&& other) = delete;
		Controller& operator=(Controller&& other) = delete;

		void PollInput();

		bool PressedThisFrame(Controller::Button button) const;
		bool ReleasedThisFrame(Controller::Button button) const;
		bool HeldThisFrame(Controller::Button button) const;

	private:
		class Impl;
		Impl* m_Impl;
	};
}
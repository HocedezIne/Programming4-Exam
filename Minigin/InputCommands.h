#pragma once
#include "GameObject.h"
#include "TimeUtil.h"
#include "ServiceLocator.h"

namespace engine
{
	class InputCommand
	{
	public:
		virtual ~InputCommand() = default;

		InputCommand(const InputCommand& other) = delete;
		InputCommand& operator=(const InputCommand& other) = delete;
		InputCommand(InputCommand&& other) = delete;
		InputCommand& operator=(InputCommand&& other) = delete;

		virtual void Execute() = 0;

	protected:
		InputCommand() = default;
	};

	class GameObjectInputCommand : public InputCommand
	{
	public:
		virtual ~GameObjectInputCommand() = default;

		GameObjectInputCommand(const GameObjectInputCommand& other) = delete;
		GameObjectInputCommand& operator=(const GameObjectInputCommand& other) = delete;
		GameObjectInputCommand(GameObjectInputCommand&& other) = delete;
		GameObjectInputCommand& operator=(GameObjectInputCommand&& other) = delete;

	protected:
		GameObjectInputCommand(GameObject* go) : InputCommand(), m_GO(go) {};

		GameObject* GetGameObject() const { return m_GO; };

	private:
		GameObject* m_GO;
	};

	class MoveInputCommand final : public GameObjectInputCommand
	{
	public:
		MoveInputCommand(GameObject* go, const glm::vec3 direction, const float speed) : GameObjectInputCommand(go),
			m_Direction(direction), m_Speed(speed) {};

		virtual ~MoveInputCommand() = default;

		MoveInputCommand(const MoveInputCommand& other) = delete;
		MoveInputCommand& operator=(const MoveInputCommand& other) = delete;
		MoveInputCommand(MoveInputCommand&& other) = delete;
		MoveInputCommand& operator=(MoveInputCommand&& other) = delete;

		virtual void Execute() override 
		{ 
			GetGameObject()->SetLocalPosition(GetGameObject()->GetLocalPosition() + (m_Direction * m_Speed * TimeUtil::GetDeltaTime())); 
		};
		
	private:
		const glm::vec3 m_Direction;
		const float m_Speed;
	};

	class SnapToInputCommand final : public GameObjectInputCommand
	{
	public:
		SnapToInputCommand(GameObject* go, const glm::vec3 pos) : GameObjectInputCommand(go),
			m_Position(pos) {};

		virtual ~SnapToInputCommand() = default;

		SnapToInputCommand(const SnapToInputCommand& other) = delete;
		SnapToInputCommand& operator=(const SnapToInputCommand& other) = delete;
		SnapToInputCommand(SnapToInputCommand&& other) = delete;
		SnapToInputCommand& operator=(SnapToInputCommand&& other) = delete;

		virtual void Execute() override
		{
			GetGameObject()->SetLocalPosition(m_Position);
		};

	private:
		const glm::vec3 m_Position;
	};

	class AlterVolumeCommand final : public InputCommand
	{
	public:
		AlterVolumeCommand(int amount) : InputCommand(), m_Amount(amount) {};
		virtual ~AlterVolumeCommand() = default;

		AlterVolumeCommand(const AlterVolumeCommand& other) = delete;
		AlterVolumeCommand& operator=(const AlterVolumeCommand& other) = delete;
		AlterVolumeCommand(AlterVolumeCommand&& other) = delete;
		AlterVolumeCommand& operator=(AlterVolumeCommand&& other) = delete;

		virtual void Execute() override
		{
			int currentVolume{ ServiceLocator::GetInstance().GetSoundSystem().GetVolume()};
			ServiceLocator::GetInstance().GetSoundSystem().SetVolume( currentVolume + m_Amount);
		}

	private:
		int m_Amount;
	};

	class MuteVolumeCommand final : public InputCommand
	{
	public:
		MuteVolumeCommand() : InputCommand() {};
		virtual ~MuteVolumeCommand() = default;

		MuteVolumeCommand(const MuteVolumeCommand& other) = delete;
		MuteVolumeCommand& operator=(const MuteVolumeCommand& other) = delete;
		MuteVolumeCommand(MuteVolumeCommand&& other) = delete;
		MuteVolumeCommand& operator=(MuteVolumeCommand&& other) = delete;

		virtual void Execute() override
		{
			ServiceLocator::GetInstance().GetSoundSystem().MuteVolume();
		}
	};
}
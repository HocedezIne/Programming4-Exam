#include "SoundSystemService.h"

#include "Singleton.h"
#include "ThreadSafeQueue.h"

#include <SDL.h>
#include <SDL_mixer.h>
#include <thread>
#include <map>
#include <algorithm>

namespace engine
{
	class SoundSystemService::Impl final : public Singleton<SoundSystemService::Impl>
	{
	public:
		Impl() {
			if (SDL_Init(SDL_INIT_AUDIO) < 0) std::cerr << "Error initializing SDL audio\n";
			if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) std::cerr << "Error initializing SDL_mixer\n";

			Mix_AllocateChannels(m_NumChannels);
			Mix_Volume(-1, 40);

			m_SoundThread = std::jthread(&Impl::SoundLoop, this);
		};

		~Impl() {
			m_SoundQueue.enqueue(SoundEvent{ SoundAction::Quit, "", false });
			m_SoundThread.join();

			for (auto& sound : m_SoundEffects)
			{
				Mix_FreeChunk(sound.second);
			}
			Mix_CloseAudio();
		};

		Impl(const Impl& other) = default;
		Impl& operator= (const Impl& other) = delete;
		Impl(Impl&& other) = delete;
		Impl& operator= (Impl&& other) = delete;

		virtual void PlaySound(const std::string& soundLabel, bool isMusic)
		{
			m_SoundQueue.enqueue(SoundEvent(SoundAction::Play, soundLabel, isMusic));
		};

		virtual void StopSound(const std::string& soundLabel) {
			m_SoundQueue.enqueue(SoundEvent{ SoundAction::Stop, soundLabel, false });
		};

		virtual void StopAllSound() { 
			m_SoundQueue.enqueue(SoundEvent{ SoundAction::Stop, "", false });
		};

		virtual int GetVolume() const
		{
			return Mix_Volume(-1, -1);
		}

		virtual void SetVolume(int value)
		{
			value = SDL_clamp(value, 0, MIX_MAX_VOLUME);
			Mix_Volume(-1, value);
		}

		virtual void MuteVolume()
		{
			Mix_Volume(-1, 0);
		}

	private:
		void SoundLoop()
		{
			bool active = true;
			while (active)
			{
				SoundEvent event = m_SoundQueue.dequeue();

				switch (event.action)
				{
				case SoundAction::Play:
					if (m_SoundEffects.find(event.label) == m_SoundEffects.end())
					{
						Mix_Chunk* sound = Mix_LoadWAV(event.label.c_str());
						if (!sound)
						{
							std::cerr << "Couldn't load sound: " << event.label << std::endl;
							return;
						}
						else m_SoundEffects[event.label] = sound;
					}

					Mix_PlayChannel(event.isMusic ? 0 : -1, m_SoundEffects[event.label], event.isMusic ? -1 : 0);
					break;
				case SoundAction::Stop:
					if (event.label == "")
					{
						Mix_HaltChannel(-1);
					}
					else 
					{
						bool found{ false };
						for (int i{}; i < m_NumChannels && !found; ++i)
						{
							auto sound = Mix_GetChunk(i);
							if (sound == m_SoundEffects[event.label])
							{
								Mix_HaltChannel(i);
								found = true;
							}
						}
					}
					break;
				case SoundAction::Quit:
					active = false;
					break;
				default:
					break;
				}
			}
		}

		const int m_NumChannels{ 16 };

		engine::ThreadSafeQueue<SoundEvent> m_SoundQueue;
		std::jthread m_SoundThread;

		std::map<std::string, Mix_Chunk*> m_SoundEffects;
	};

	SoundSystemService::SoundSystemService()
	{
		m_Impl = new Impl{};
	}

	SoundSystemService::~SoundSystemService()
	{
		delete m_Impl;
		m_Impl = nullptr;
	}

	void SoundSystemService::PlaySound(const std::string& soundLabel, bool isMusic)
	{
		m_Impl->PlaySound(soundLabel, isMusic);
	}

	void SoundSystemService::StopSound(const std::string& soundLabel)
	{
		m_Impl->StopSound(soundLabel);
	}

	void SoundSystemService::StopAllSound()
	{
		m_Impl->StopAllSound();
	}

	int SoundSystemService::GetVolume() const
	{
		return m_Impl->GetVolume();
	}

	void SoundSystemService::SetVolume(int value)
	{
		m_Impl->SetVolume(value);
	}

	void SoundSystemService::MuteVolume()
	{
		m_Impl->MuteVolume();
	}
}
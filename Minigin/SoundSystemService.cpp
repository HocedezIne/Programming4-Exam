#include "SoundSystemService.h"

#include "Singleton.h"

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

			m_AudioPos = NULL;
			m_AudioLen = 0;

			//m_SoundThread = std::jthread(&Impl::SoundLoop, this);
			//Mix_HookMusicFinished(Impl::AudioCallback, NULL);
		};
		~Impl() {
			//m_SoundThread.join();

			for (auto& sound : m_SoundEffects)
			{
				Mix_FreeChunk(sound.second);
			}
			Mix_CloseAudio();
			SDL_Quit();
		};

		Impl(const Impl& other) = default;
		Impl& operator= (const Impl& other) = delete;
		Impl(Impl&& other) = delete;
		Impl& operator= (Impl&& other) = delete;

		virtual void PlaySound(const std::string& soundLabel, bool isMusic)
		{
			if (m_SoundEffects.find(soundLabel) == m_SoundEffects.end())
			{
				Mix_Chunk* sound = Mix_LoadWAV(soundLabel.c_str());
				if (!sound)
				{
					std::cerr << "Couldn't load sound: " << soundLabel << std::endl;
					return;
				}
				else m_SoundEffects[soundLabel] = sound;
			}
			
			Mix_PlayChannel(isMusic ? 0 : -1, m_SoundEffects[soundLabel], 0);
		};

		virtual void StopSound(const std::string& /*soundLabel*/)
		{
		
		};

		virtual void StopAllSound()
		{
		
		};

	private:
		//void SoundLoop()
		//{
		//	while (!m_QuitSoundLoop)
		//	{
		//		while (SDL_PollEvent(&m_Event))
		//		{
		//			switch (m_Event.type)
		//			{
		//			case SDL_QUIT:
		//				m_QuitSoundLoop = true;
		//				break;
		//				case 
		//			default:
		//				break;
		//			}
		//		}
		//	}
		//}

		void AudioCallback(void* /*data*/, uint8_t* stream, int len)
		{
			if (m_AudioLen = 0) return; // no more audio to play

			len = std::min(len, m_AudioLen);

			SDL_MixAudio(stream, m_AudioPos, len, SDL_MIX_MAXVOLUME);

			m_AudioPos += len;
			m_AudioLen -= len;
		}

		//std::jthread m_SoundThread;
		std::map<std::string, Mix_Chunk*> m_SoundEffects;

		SDL_Event m_Event{};
		bool m_QuitSoundLoop{ false };

		uint8_t* m_AudioPos;
		int m_AudioLen;
	};

	SoundSystemService::SoundSystemService()
	{
		m_Impl = new Impl{};
	}

	SoundSystemService::~SoundSystemService()
	{
		m_Impl->StopAllSound();
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
}
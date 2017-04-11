#pragma once

namespace Necromancer{
	class SoundEngine{
	public:
		SoundEngine();
		virtual ~SoundEngine();

		void play_bgm(const char*);
	private:
	};
}


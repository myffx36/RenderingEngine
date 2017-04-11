#pragma once

namespace Necromancer{
	class PIL;
	class Configuration;
	class PILFactory{
	public:
		virtual ~PILFactory();

		virtual PIL* create_pil(Configuration* config);
	private:
	};
}


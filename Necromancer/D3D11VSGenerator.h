#pragma once

#include "VSGenerator.h"

namespace Necromancer{
	class D3D11VSGenerator:public VSGenerator{
	public:
		D3D11VSGenerator();

		virtual ~D3D11VSGenerator();
	private:
		virtual String generate_input_text(const String&);
		virtual String generate_output_text(const String&);
		virtual String generate_header_text(const String& input, const String& output);
		virtual String generate_tail_text(const String& input, const String& output);
		virtual String save_debug_file(const String&, const String&);

		void init_build_in_vs();
	};
}


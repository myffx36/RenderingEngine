#pragma once

namespace Necromancer{
	class DrawingRequest;
	class GraphicPipeline{
	public:
		virtual void process_drawing_request(DrawingRequest*) = 0;
		virtual ~GraphicPipeline();
	private:
	};
}


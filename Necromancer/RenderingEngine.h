#pragma once

#include "CommonUtility.h"
#include "Camera.h"
#include "SubScene.h"
#include "Container.hpp"

namespace Necromancer{
	class SubScene;
	class Configuration;
	class PIL;
	class Scene;
	class GraphicPipeline;
	class NodeObserverManager;
	class TwoDObject;
	class RenderingEngine{
		DEF_DATA_MEMBER_AND_GETTER(CameraPtr, camera);
		DEF_PTR_MEMBER_AND_GETTER(NodeObserverManager, node_observer_manager);
		DEF_PTR_MEMBER_AND_GETTER(Scene, scene);
	public:
		RenderingEngine(Configuration*, PIL*);
		~RenderingEngine();

		void attach_sub_scene(const SubScenePtr& );
		void detach_sub_scene(const SubScenePtr& );

		void attach_two_d_obejct(TwoDObject*);
		void detach_two_d_object(TwoDObject*);

		void draw_scene();
		virtual void begin();
		virtual void send_end_message();
		virtual void wait_for_end();

		virtual void thread_main();
	private:
		GraphicPipeline* m_graphic_pipeline;
		Vector<TwoDObject*>::Type m_two_d_object;
		bool m_quit;
		std::thread m_thread;
	};
}


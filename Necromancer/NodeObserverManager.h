#pragma once

#include "Container.hpp"

namespace Necromancer{
	class NodeObserver;
	class SceneNode;
	class NodeObserverManager{
	public:
		void attach_node_observer(NodeObserver*);
		virtual ~NodeObserverManager();

		void notify_update(SceneNode*);
	private:
		Vector<NodeObserver*>::Type m_node_observer;
	};
}


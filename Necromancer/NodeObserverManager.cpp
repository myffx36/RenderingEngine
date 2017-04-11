#include "stdafx.h"
#include "NodeObserverManager.h"

#include "NodeObserver.h"

namespace Necromancer{
	void NodeObserverManager::attach_node_observer(
		NodeObserver* node_observer)
	{
		m_node_observer.push_back(node_observer);
	}

	NodeObserverManager::~NodeObserverManager(){
		for(auto iter = m_node_observer.begin();
			iter != m_node_observer.end();++ iter)
		{
			if(*iter != nullptr){
				delete *iter;
			}
		}
	}

	void NodeObserverManager::notify_update(SceneNode* scene_node){
		for(auto iter = m_node_observer.begin();
			iter != m_node_observer.end();++ iter)
		{
			NodeObserver* observer = *iter;
			if(nullptr != observer){
				observer->notify_update(scene_node);
			}
		}
	}
}
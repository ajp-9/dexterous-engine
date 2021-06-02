#include "LayerManager.hpp"

#include <SDL2/SDL.h>

#include "../../Core/Engine.hpp"
#include "Event/KeyEvent.hpp"
#include "Event/MouseEvent.hpp"

namespace dex
{
	namespace Layer
	{
		Manager::Manager()
		{
		}

		void Manager::pushLayer(std::shared_ptr<Base> layer)
		{
			layer->m_Index = m_Layers.size();
			m_Layers.push_back(layer);
			layer->Attach();
		}

		void Manager::popLayer()
		{
			m_Layers.at(m_Layers.size() - 1)->Detach();
			m_Layers.pop_back();
		}

		void Manager::detachAllLayers()
		{
			for (auto& l : m_Layers)
				popLayer();
		}

		void Manager::updateLayers()
		{
			for (auto& l : m_Layers)
				l->update();
		}

		void Manager::renderLayers()
		{
			for (auto& l : m_Layers)
				l->render();
		}

		void Manager::sendEvents()
		{
			auto events = Event::Manager::getEventQueue();;

			for (auto& e : events)
			{
				for (std::vector<std::shared_ptr<Base>>::reverse_iterator it = m_Layers.rbegin(); it != m_Layers.rend(); ++it)
				{
					it->get()->event(e);

					if (e->m_Handled)
						return;
				}
			}
		}
	}
}

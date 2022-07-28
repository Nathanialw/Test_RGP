#pragma once
#include "entt.hpp"
#include "components.h"


namespace Unit_Status {
	
	void Update_Health(entt::registry& zone) {
		auto view2 = zone.view<Component::Health, Component::Struck, Component::Actions>();

		for (auto entity : view2) {
			auto& damage = view2.get<Component::Struck>(entity).struck;
			auto& health = view2.get<Component::Health>(entity).iHealth;
			auto& action = view2.get<Component::Actions>(entity).action;
			action = Component::struck;
			//std::cout << "health = " << health << std::endl;
			health -= damage;
			zone.remove<Component::Struck>(entity);

			//if the soldier is in the assignment vector it will be set as dead if it dies
			if (health == 0) {
				if (zone.any_of<Component::Assigned_To_Formation>(entity)) {
					auto& assignment = zone.get<Component::Assigned_To_Formation>(entity);
					auto& squad = zone.get<Component::Squad>(assignment.iUnit_Assigned_To);
					squad.bAlive.at(assignment.iIndex) = false;
				}
			}
		}
	}


	void Update_Unit_Status(entt::registry &zone) {
		Update_Health(zone);
	}

}
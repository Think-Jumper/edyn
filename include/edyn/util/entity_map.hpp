#ifndef EDYN_UTIL_ENTITY_MAP_HPP
#define EDYN_UTIL_ENTITY_MAP_HPP

#include <unordered_map>
#include <entt/fwd.hpp>
#include <entt/entity/entity.hpp>

namespace edyn {

class entity_map {
public:
    void insert(entt::entity remote_entity, entt::entity local_entity) {
        m_remloc[remote_entity] = local_entity;
        m_locrem[local_entity] = remote_entity;
    }

    bool has_rem(entt::entity remote_entity) const {
        return m_remloc.count(remote_entity);
    }

    bool has_loc(entt::entity local_entity) const {
        return m_locrem.count(local_entity);
    }

    entt::entity remloc(entt::entity remote_entity) const {
        if (has_rem(remote_entity)) {
            return m_remloc.at(remote_entity);
        } else {
            return entt::null;
        }
    }
    
    entt::entity locrem(entt::entity local_entity) const {
        if (has_loc(local_entity)) {
            return m_locrem.at(local_entity);
        } else {
            return entt::null;
        }
    }

private:
    std::unordered_map<entt::entity, entt::entity> m_remloc; // Maps remote to local entities.
    std::unordered_map<entt::entity, entt::entity> m_locrem; // Maps local to remote entities.
};

}

#endif // EDYN_UTIL_ENTITY_MAP_HPP
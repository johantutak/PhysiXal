#pragma once
#include <unordered_map>
#include <memory>
#include <typeindex>
#include <vector>
#include <type_traits>

namespace PhysiXal {

    using EntityID = unsigned int;

    // Base Component class
    class Component {
    public:
        virtual ~Component() = default;
    };

    // Entity class holding components
    class Entity {
    public:
        Entity(EntityID id) : m_ID(id) {}
        EntityID GetID() const { return m_ID; }

        template<typename T, typename... TArgs>
        T* AddComponent(TArgs&&... args) {
            static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");
            auto comp = std::make_shared<T>(std::forward<TArgs>(args)...);
            T* component = comp.get();
            m_Components[std::type_index(typeid(T))] = comp; // Implicit conversion from shared_ptr<T> to shared_ptr<Component> works.
            return component;
        }

        template<typename T>
        T* GetComponent() {
            static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");
            auto it = m_Components.find(std::type_index(typeid(T)));
            if (it != m_Components.end())
                return dynamic_cast<T*>(it->second.get());
            return nullptr;
        }

        template<typename T>
        bool HasComponent() {
            static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");
            return m_Components.find(std::type_index(typeid(T))) != m_Components.end();
        }

    private:
        EntityID m_ID;
        std::unordered_map<std::type_index, std::shared_ptr<Component>> m_Components;
    };

    // ECS Manager that tracks entities and the current selection
    class ECSManager {
    public:
        Entity* CreateEntity() {
            EntityID id = m_NextEntityID++;
            m_Entities.push_back(std::make_unique<Entity>(id));
            return m_Entities.back().get();
        }

        const std::vector<std::unique_ptr<Entity>>& GetEntities() const { return m_Entities; }

        Entity* GetSelectedEntity() const { return m_SelectedEntity; }
        void SetSelectedEntity(Entity* entity) { m_SelectedEntity = entity; }

    private:
        EntityID m_NextEntityID = 0;
        std::vector<std::unique_ptr<Entity>> m_Entities;
        Entity* m_SelectedEntity = nullptr;
    };

    extern ECSManager* g_ECSManager;
}
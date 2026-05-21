module;
#include <stdint.h>
#include <vector>
#include <unordered_map>
#include <log.hpp>
#include <windalive.hpp>
export module Game.ECS.ComponentArray;

import Game.ECS.Entity;

// TODO: rewrite all this shit plz
namespace Game {
  namespace ECS { 
    export template<
      typename T
    > class ComponentArray {
      private:
        std::vector<T> components;
        std::vector<Entity> entities;
        std::unordered_map<Entity, size_t> entityToIndex;

      public:
        ComponentArray() {
            components.reserve(100);
            entities.reserve(100);
        }

        fn add(Entity e, const T& component) -> void {
          if (entityToIndex.find(e) != entityToIndex.end()) {
            auto idx = entityToIndex[e];
            components[idx] = component;
            return;
          }

          entityToIndex[e] = components.size();

          components.push_back(component);
          entities.push_back(e);
        }

        fn remove(Entity e) -> void {
          auto it = entityToIndex.find(e);
          if (it == entityToIndex.end()) {
            return;
          }

          auto idx = it->second;
          auto lastIdx = components.size() - 1;

          if (idx != lastIdx) {
            components[idx] = std::move(components[lastIdx]);
            entities[idx] = entities[lastIdx];
            entityToIndex[entities[idx]] = idx;
          }

          components.pop_back();
          entities.pop_back();
          entityToIndex.erase(e);
        }

        fn get(Entity e) const noexcept -> const T& {
          auto it = entityToIndex.find(e);
          if constexpr (!NDEBUG_VAR) {
            if (it == entityToIndex.end()) {
                SPDLOG_ERROR(
                  "Entity {} Not exists in ComponentArray<{}>",
                  e, typeid(T).name()
                );
            }
          }

          return components[it->second];
        }

        fn get(Entity e) noexcept -> T& {
          auto it = entityToIndex.find(e);
          if constexpr (!NDEBUG_VAR) {
            if (it == entityToIndex.end()) {
                SPDLOG_ERROR(
                  "Entity {} Not exists in ComponentArray<{}>",
                  e, typeid(T).name()
                );
            }
          }

          return components[it->second];
        }

        fn getEntities() const noexcept -> const std::vector<Entity>& { 
          return entities; 
        }
        fn getComponents() noexcept -> std::vector<T>& { 
          return components; 
        }
    };
  }
}

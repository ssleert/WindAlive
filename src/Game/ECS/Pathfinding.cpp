module;
#include <algorithm>
#include <cmath>
#include <log.hpp>
#include <queue>
#include <unordered_map>
#include <vector>
#include <windalive.hpp>
export module Game.ECS.Pathfinding;

import Math.Vector;
import Game.World.State;
import Game.World.Field;

using namespace Math;

namespace Game {
namespace ECS {

// TODO: rewrite using more efficent heuristics (excepa Manhattan)
// maybe use Boost.Graph
export class Pathfinding
{
public:
  struct Node
  {
    int32_t x, y;
    float gCost, hCost, fCost;
    Node* parent = nullptr;

    fn operator>(const Node& other) const noexcept -> bool
    {
      return fCost > other.fCost;
    }
  };

  static fn findPath(const Game::World::State& world,
                     Vector2 startWorld,
                     Vector2 goalWorld) -> std::vector<Vector2>
  {
    const int32_t fieldSize = world.fieldSize;

    auto toGrid = [&](Vector2 w) -> Node {
      return { (int32_t)(w.x / fieldSize), (int32_t)(w.y / fieldSize) };
    };

    Node startNode = toGrid(startWorld);
    Node goalNode = toGrid(goalWorld);

    if (startNode.x < 0 || startNode.x >= world.width || startNode.y < 0 ||
        startNode.y >= world.height)
      return {};

    std::priority_queue<Node, std::vector<Node>, std::greater<>> openSet;
    std::unordered_map<int32_t, Node> allNodes; // key = x + y * width

    auto getKey = [&](int32_t x, int32_t y) { return x + y * world.width; };

    startNode.gCost = 0;
    startNode.hCost = heuristic(startNode, goalNode);
    startNode.fCost = startNode.hCost;
    openSet.push(startNode);
    allNodes[getKey(startNode.x, startNode.y)] = startNode;

    const int dx[8] = { -1, -1, -1, 0, 0, 1, 1, 1 };
    const int dy[8] = { -1, 0, 1, -1, 1, -1, 0, 1 };

    while (!openSet.empty()) {
      Node current = openSet.top();
      openSet.pop();

      if (current.x == goalNode.x && current.y == goalNode.y) {
        return reconstructPath(current, fieldSize);
      }

      for (int i = 0; i < 8; ++i) {
        int nx = current.x + dx[i];
        int ny = current.y + dy[i];

        if (nx < 0 || nx >= world.width || ny < 0 || ny >= world.height)
          continue;

        const auto& field = world.fields[ny * world.width + nx];
        if (field.isCollidable())
          continue;

        float tentativeGCost =
          current.gCost +
          (dx[i] != 0 && dy[i] != 0 ? 1.414f : 1.0f); // diagonal cost

        int key = getKey(nx, ny);
        auto it = allNodes.find(key);

        if (it == allNodes.end() || tentativeGCost < it->second.gCost) {
          Node neighbor = { nx,
                            ny,
                            tentativeGCost,
                            heuristic({ nx, ny }, goalNode),
                            tentativeGCost + heuristic({ nx, ny }, goalNode),
                            nullptr };
          neighbor.parent = &allNodes[getKey(current.x, current.y)];
          allNodes[key] = neighbor;
          openSet.push(neighbor);
        }
      }
    }

    SPDLOG_WARN("Cant find path for {} and {} vectors",
                startWorld.to_string(),
                goalWorld.to_string());
    return {};
  }

private:
  static fn heuristic(const Node& a, const Node& b) noexcept -> float
  {
    return std::abs(a.x - b.x) + std::abs(a.y - b.y);
  }

  static fn reconstructPath(Node node, int32_t fieldSize)
    -> std::vector<Vector2>
  {
    std::vector<Vector2> path;
    path.reserve(100); // TODO: add heuristic for list size
    Node* current = &node;

    while (current) {
      path.push_back({ current->x * (float)fieldSize + fieldSize * 0.5f,
                       current->y * (float)fieldSize + fieldSize * 0.5f });
      current = current->parent;
    }

    std::reverse(path.begin(), path.end());
    return path;
  }
};
}
}

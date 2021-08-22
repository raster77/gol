#ifndef SCENESIM_HPP_
#define SCENESIM_HPP_

#include <scene/Scene.hpp>
#include <utils/Fps.hpp>
#include "sparsegrid/SparseGrid.hpp"
#include "Rule.hpp"
#include "Gui.hpp"
#include <SFML/Graphics/Vertex.hpp>
#include <cmath>

class SceneSim final : public Scene
{
  public:
    SceneSim();
    SceneSim(sf::RenderWindow* window);
    ~SceneSim();

    void load();
    void update(const float dt);
    void draw();
    void handleEvent(sf::Event& event);

  private:
    struct DragMouse
    {
      sf::Vector2f startPos;
      sf::Vector2f endPos;
      bool dragging;

      DragMouse()
	: startPos(0.f, 0.f)
	, endPos(0.f, 0.f)
	, dragging(false)
      {}

      sf::Vector2f diff()
      {
	 //Pour arrondir m au multiple suivant de n, utilisez ((m + n-1) / n) * n
	const float x = std::floor(endPos.x - startPos.x);
	const float y = std::floor(endPos.y - startPos.y);
	return sf::Vector2f(x, y);
      }
    };

    SparseGrid gridA;
    SparseGrid gridB;
    std::vector<sf::Vertex> vertices;
    std::vector<sf::Vertex> gridVertices;
    std::size_t mGeneration;
    bool simRunning;
    const std::size_t cellSize;
    sf::View view;
    const float zoomFactor;
    float zoomLevel;
    DragMouse dragInfo;
    Rule rule;
    bool updateGrid;
    sf::Vector2f viewMove;
    Fps fps;
    Gui gui;
    sf::Clock clock;
    std::unordered_map<std::size_t, std::vector<std::pair<int64_t, int64_t>>> history;

    void addCell(const unsigned short int v);
    void updateVertices(SparseGrid* grid);
    void setGridVertices();
    sf::Vector2i getGridCoord();
    void goBack();

};

#endif /* SCENESIM_HPP_ */

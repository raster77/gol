#include "SceneSim.hpp"
#include <algorithm>
#include <memory>
#include <numeric>
#include "reader/PlainTextReader.hpp"
#include "reader/RleReader.hpp"

SceneSim::SceneSim()
  : Scene()
  , gridA()
  , gridB()
  , mGeneration(0)
  , simRunning(false)
  , cellSize(5)
  , zoomFactor(2.f)
  , zoomLevel(0.f)
  , updateGrid(false)
{
}

SceneSim::SceneSim(sf::RenderWindow* window)
  : Scene(window)
  , gridA()
  , gridB()
  , mGeneration(0)
  , simRunning(false)
  , cellSize(5)
  , zoomFactor(2.f)
  , zoomLevel(0.f)
  , updateGrid(false)
{
}

SceneSim::~SceneSim()
{
}

void SceneSim::load()
{
  Scene::load();
  view.setCenter(getWindowCenter());
  view.setSize(getWindowSize());
  rule.set({3}, {2, 3});
  setGridVertices();
  gui.init(window);
  gui.loadFont("res/DroidSerif-Regular.ttf", 14);
}

void SceneSim::update(const float dt)
{
  fps.update();
  SparseGrid* grid = mGeneration % 2 == 0 ? &gridA : &gridB;
  SparseGrid* grid2 = mGeneration % 2 == 0 ? &gridB : &gridA;
  if(gui.isRunning() && clock.getElapsedTime().asMilliseconds() >= gui.getStepTime()) {
    grid2->clear();
    std::vector<std::pair<int64_t, int64_t>> cells;
    auto it = grid->begin();
    while(it != grid->end())
    {
      const Cell* c = &it->second;
      if(c != nullptr)
      {
	cells.emplace_back(std::make_pair(c->row, c->col));
	std::array<Cell, 9> neighbours = grid->neighbours(c->row, c->col);
	for(auto& n : neighbours)
	{
	  const std::size_t c = grid->countNeighbours(n.row, n.col);
	  const bool alive = n.value == 1;
	  const unsigned short int v = rule.check(alive, c);
	  //const unsigned short int v2 = (!alive && c == 3) || (alive && (c >= 2 && c <= 3)) ? 1 : 0;
	  if(v == 1) {
	    grid2->set(n.row, n.col, Cell(n.row, n.col, 1, alive ? n.age + 1 : 1));
	  }
	}
      }
      ++it;
    }

    history[mGeneration] = cells;

    mGeneration++;
    clock.restart();
  }

  updateVertices(grid);

  if(updateGrid && gui.showGrid())
  {
    setGridVertices();
  }

  gui.setCoord(getGridCoord());
  gui.setGeneration(mGeneration);
  gui.setPopulation(grid->size());

  if(gui.doReset() || gui.loadFile())
  {
    gridA.clear();
    gridB.clear();
    mGeneration = 0;
    history.clear();
    gui.stopRunning();
  }

  if(gui.goBack())
  {
    goBack();
  }

  simRunning = gui.isRunning();
  if(gui.isStepMode())
  {
    simRunning = false;
    gui.stopRunning();
  }

  if(gui.loadFile())
  {
    loadDataFile();
  }

  gui.update();

  if(viewMove != sf::Vector2f(0, 0))
  {
    view.move(viewMove);
    viewMove = sf::Vector2f(0, 0);
  }
}

void SceneSim::goBack()
{
  if(mGeneration > 0 && history.find(mGeneration - 1) != history.end())
  {
    mGeneration--;
    gridA.clear();
    gridB.clear();
    std::vector<std::pair<int64_t, int64_t>> cells = history[mGeneration];
    SparseGrid* grid = mGeneration % 2 == 0 ? &gridA : &gridB;
    for(auto& c : cells)
    {
      grid->set(c.first,  c.second, Cell(c.first,  c.second, 1));
    }

  }
}

void SceneSim::draw()
{
  window->clear(gui.getBackgroundColor());
  window->setView(view);
  window->draw(vertices.data(), vertices.size(), sf::Quads);
  if(gui.showGrid() && zoomLevel > -1 && zoomLevel <= 4.f)
  {
    window->setView(window->getDefaultView());
    window->draw(gridVertices.data(), gridVertices.size(), sf::Lines);
  }
  gui.render();
}

void SceneSim::handleEvent(sf::Event& event)
{
  gui.handleEvent(event);
  if (event.type == sf::Event::KeyPressed)
  {
    if(event.key.code == sf::Keyboard::Escape)
    {
      mRunning = false;
      window->close();
    }

    if(event.key.code == sf::Keyboard::I)
    {
      std::cout << fps.getFPS() << std::endl;
    }

    if(event.key.code == sf::Keyboard::T)
    {
      boundingBox();
    }

    if(event.key.code == sf::Keyboard::Up)
    {
      viewMove.y = 50.f;
      updateGrid = true;
    }

    if(event.key.code == sf::Keyboard::Down)
    {
      viewMove.y = -50.f;
      updateGrid = true;
    }

    if(event.key.code == sf::Keyboard::Left)
    {
      viewMove.x = -50.f;
      updateGrid = true;
    }

    if(event.key.code == sf::Keyboard::Right)
    {
      viewMove.x = 50.f;
      updateGrid = true;
    }

  }

  if (event.type == sf::Event::MouseWheelMoved)
  {
    if(!gui.isFocused())
    {
      const float sign = event.mouseWheel.delta > 0.f ? 1.f : -1.f;
      if(zoomLevel + sign <= 4.f)
      {
	zoomLevel += sign;
	if(zoomLevel == 3.f)
	{
	  zoomLevel = sign >= 0.f ? 4.f : 2.f;
	}
	const float delta = event.mouseWheel.delta > 0.f ? 1.f / zoomFactor : zoomFactor;
	view.zoom(delta);
	view.setCenter(view.getCenter() * delta);
	updateGrid = true;
      }
    }
  }

  if (event.type == sf::Event::MouseButtonPressed)
  {
    if(gui.isFocused())
    {
      return;
    }

    if(sf::Mouse::isButtonPressed(sf::Mouse::Right))
    {
      dragInfo.dragging = true;
      dragInfo.startPos = getMousePosition();
    } else if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
      sf::Vector2i coord = getGridCoord();
      SparseGrid* grid = mGeneration % 2 == 0 ? &gridA : &gridB;

      if(grid->exists(coord.x, coord.y))
      {
    	grid->erase(coord.x, coord.y);
      } else {
    	grid->set(coord.x, coord.y, 1);
      }
    }
  }

  if (event.type == sf::Event::MouseMoved)
  {
    if(gui.isFocused())
    {
      return;
    }

    if(dragInfo.dragging)
    {
      dragInfo.endPos = getMousePosition();
      sf::Vector2i diff = static_cast<sf::Vector2i>(dragInfo.diff());
      const int round = 10;
      int sign = diff.x < 0.f ? -1 : 1;
      if(diff.x % round != 0)
      {
	diff.x = ((diff.x - diff.x % round) + round) * sign;
      }

      sign = diff.y < 0.f ? -1 : 1;
      if(diff.y % round != 0)
      {
	diff.y = ((diff.y - diff.y % round) + round) * sign;
      }
      view.setCenter(view.getCenter() + static_cast<sf::Vector2f>(diff));
      dragInfo.startPos = getMousePosition();
      updateGrid = true;
    }

    if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
      sf::Vector2i coord = getGridCoord();
      SparseGrid* grid = mGeneration % 2 == 0 ? &gridA : &gridB;
      unsigned short int v = grid->get(coord.x, coord.y).value;
      addCell(v);
    }
  }

  if (event.type == sf::Event::MouseButtonReleased)
  {
    if (event.mouseButton.button == sf::Mouse::Right)
    {
      dragInfo.dragging = false;
    }
  }
}

sf::Vector2i SceneSim::getGridCoord()
{
  sf::Vector2i pixelPos = sf::Mouse::getPosition(*window);
  sf::Vector2f worldPos = window->mapPixelToCoords(pixelPos, view);
  return sf::Vector2i(static_cast<int>(std::floor(worldPos.x / static_cast<float>(cellSize))),
		      static_cast<int>(std::floor(worldPos.y / static_cast<float>(cellSize))));
}

void SceneSim::addCell(const unsigned short int v)
{
  const sf::Vector2i coord = getGridCoord();
  SparseGrid* grid = mGeneration % 2 == 0 ? &gridA : &gridB;
  if(v == 1)
  {
    grid->erase(coord.x, coord.y);
  } else {
    grid->set(coord.x, coord.y, 1);
  }
}

void SceneSim::boundingBox()
{

  SparseGrid* grid = mGeneration % 2 == 0 ? &gridA : &gridB;
  Cell minCell(999999, 999999, 1);
  Cell maxCell(-999999, -999999, 1);
  std::vector<Cell> cells;
  auto it = grid->begin();
  while(it != grid->end())
  {
    cells.emplace_back(it->second);
    if(it->second.row < minCell.row)
    {
      minCell.row = it->second.row;
    }

    if(it->second.col < minCell.col)
    {
      minCell.col = it->second.col;
    }

    if(it->second.row > maxCell.row)
    {
      maxCell.row = it->second.row;
    }

    if(it->second.col > maxCell.col)
    {
      maxCell.col = it->second.col;
    }
    ++it;
  }

  std::sort(cells.begin(), cells.end());

  int width = (maxCell.row - minCell.row) + 1;
  int height = (maxCell.col - minCell.col) + 1;

  std::cout << "Min: " << minCell.row << " " << minCell.col << std::endl;
  std::cout << "Max: " << maxCell.row << " " << maxCell.col << std::endl;
  std::cout << "Width: " << width << " Height: " << height << std::endl;
  std::string pattern;
  for(int y = 0; y < height; ++y)
  {
    std::string tmp;
    for(int x = 0; x < width; ++x)
    {
      Cell c = minCell;
      c.row += x;
      c.col += y;
      int val = 0;
      if(std::find(cells.begin(), cells.end(), c) != cells.end())
      {
	val = 1;
      }
      tmp.append(std::to_string(val));
    }
    while(true)
    {
      if(!tmp.empty() && tmp[tmp.size()-1] == '0')
      {
	tmp.pop_back();
      } else {
	break;
      }
    }
    pattern.append(tmp);
    pattern.append("$");
  }

  char lastChar = pattern[0];
  std::size_t count = 1;
  std::string encoded = "";

  for(std::size_t i = 1; i < pattern.size(); ++i)
  {
    if(lastChar == pattern[i])
    {
      count++;
    } else {
      char val = lastChar == '0' ? 'b' : 'o';
      if(lastChar == '$')
	val = '$';
      if(pattern[i] != '$' || (pattern[i] == '$' && val != 'b'))
      {
	if(count > 1) {
	  encoded.append(std::to_string(count));
	}
	encoded += val;
      }
      count = 1;
      lastChar = pattern[i];
    }
  }
  encoded += '!';

  std::cout << pattern << std::endl;
  std::cout << encoded << std::endl;

}

void SceneSim::loadDataFile()
{
  const std::string file = gui.getFileName();
  std::string ext = file.substr(file.length() - 3, 3);
  std::transform(ext.begin(), ext.end(), ext.begin(), [] (unsigned char c) { return std::tolower(c); });
  std::unique_ptr<AbstractReader> reader;
  if(ext == "rle")
  {
    reader = std::make_unique<RleReader>();
  } else {
    reader = std::make_unique<PlainTextReader>();
  }

  reader.get()->readFile(file);
  std::vector<std::pair<int, int>> datas = reader.get()->getDatas();
  sf::Vector2i center = static_cast<sf::Vector2i>(view.getCenter()) / static_cast<int>(cellSize);
  for(auto& d : datas)
  {
    gridA.set(center.x + d.first, center.y + d.second, 1);
  }
}

void SceneSim::updateVertices(SparseGrid* grid)
{
  vertices.clear();
  vertices.reserve(grid->size() * 4);
  const float CS = static_cast<float>(cellSize);
  auto it = grid->begin();
  sf::Color c = gui.getCellColor();
  while(it != grid->end())
  {
    const float vx = static_cast<float>(it->second.row * CS);
    const float vy = static_cast<float>(it->second.col * CS);
    unsigned short int alpha = it->second.age;
    if(alpha > 255)
      alpha = 255;
    else if(alpha < 50)
      alpha = 50;

    //c.a = alpha;
    vertices.emplace_back(sf::Vector2f(vx, vy));
    vertices.back().color = c;
    vertices.emplace_back(sf::Vector2f(vx, vy + CS));
    vertices.back().color = c;
    vertices.emplace_back(sf::Vector2f(vx + CS, vy + CS));
    vertices.back().color = c;
    vertices.emplace_back(sf::Vector2f(vx + CS, vy));
    vertices.back().color = c;
    it++;
  }
}

void SceneSim::setGridVertices()
{
  updateGrid = false;
  if(zoomLevel > -1.f && zoomLevel <= 4.f)
  {
    const uint32_t z = zoomLevel == 0.f ? 1.f : zoomLevel * 2;
    const uint32_t CS = cellSize * z;
    const uint32_t row = std::floor(getWindowSize().x / CS);
    const uint32_t col = std::floor(getWindowSize().y / CS);
    const sf::Color c = gui.getGridColor();
    gridVertices.clear();
    gridVertices.reserve((row + col) * 2);

    sf::Vertex v;
    v.color = c;

    for(uint32_t x = 0; x < row; ++x)
    {
      const float cx = x * CS;
      v.position = sf::Vector2f(cx, 0);
      gridVertices.emplace_back(v);
      v.position = sf::Vector2f(cx, getWindowSize().y);
      gridVertices.emplace_back(v);
    }

    for(uint32_t y = 0; y < col; ++y)
    {
      const float cy = y * CS;
      v.position = sf::Vector2f(0, cy);
      gridVertices.emplace_back(v);
      v.position = sf::Vector2f(getWindowSize().x, cy);
      gridVertices.emplace_back(v);
    }
  }
}

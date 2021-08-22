#include "app/Application.hpp"
#include "SceneSim.hpp"

class MyApp : public Application
{
  public:
    MyApp()
      : Application()
    {
    }

    ~MyApp()
    {
    }

    void setup()
    {
      scene.setWindow(window());
      scene.load();
      currentScene = &scene;
    }

  private:
    SceneSim scene;
};

int main() {
  MyApp app;
  app.run(sf::VideoMode(1600, 900), "GOL");
  return 0;
}

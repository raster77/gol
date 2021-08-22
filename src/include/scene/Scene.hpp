#ifndef SCENE_HPP
#define SCENE_HPP

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <iostream>
#include <memory>
#include <string>

class Scene
{
  public:
    Scene()
      : mTitle("")
      , mousePos({0.f, 0.f})
      , window(nullptr)
      , enableMouseEvent(true)
      , mRunning(false)
    {}

    Scene(sf::RenderWindow* window)
      : mTitle("")
      , mousePos({0.f, 0.f})
      , window(window)
      , enableMouseEvent(true)
      , mRunning(false)
    {
    }

    virtual ~Scene()
    {
    }

    virtual void preUpdate(const float dt)
    {}
    virtual void update(const float dt) = 0;
    virtual void draw() = 0;
    virtual void preHandleEvent()
    {}
    virtual void handleEvent(sf::Event& event) = 0;
    virtual void postHandleEvent()
    {}
    virtual void load()
    {
	mRunning = true;
    }

    const bool isRunning()
    {
      return mRunning;
    }

    void setWindow(sf::RenderWindow* window)
    {
      this->window = window;
    }

    const std::string& getTitle() const
    {
      return mTitle;
    }

    void setTitle(const std::string& title)
    {
      mTitle = title;
    }

  protected:
    std::string mTitle;
    sf::Vector2f mousePos;
    sf::RenderWindow* window;
    bool enableMouseEvent;
    bool mRunning;

    const sf::Vector2f getWindowCenter() const
    {
      return static_cast<sf::Vector2f>(window->getSize()) * 0.5f;
    }

    const sf::Vector2f getWindowSize() const
    {
      return static_cast<sf::Vector2f>(window->getSize());
    }

    const float getWindowWidth() const
    {
      return static_cast<float>(window->getSize().x);
    }

    const float getWindowHeight() const
    {
      return static_cast<float>(window->getSize().y);
    }

    const float getWindowRatio() const
    {
      return getWindowWidth() / getWindowHeight();
    }

    const sf::Vector2f getMousePosition() const
    {
      return static_cast<sf::Vector2f>(sf::Mouse::getPosition(*window));
    }
};

#endif // SCENE_HPP

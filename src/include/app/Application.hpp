#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "scene/Scene.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/ContextSettings.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/WindowStyle.hpp>
#include <memory>
#include <string>
#include <stdexcept>

class Application
{
  public:
    Application(float framePerSecond = 60.f)
      : FPS_LIMIT(framePerSecond)
      , mWindowPtr(new sf::RenderWindow)
      , windowOpened(true)
      , currentScene(nullptr)
    {
    }

    Application(sf::ContextSettings ctx, float framePerSecond = 60.f)
      : FPS_LIMIT(framePerSecond)
      , mWindowPtr(new sf::RenderWindow)
      , windowOpened(true)
      , currentScene(nullptr)
    {
    }

    virtual ~Application()
    {}

    virtual void setup()
    {}

    void setPosition(const int x, const int y)
    {
      window()->setPosition(sf::Vector2i(x, y));
    }

    void run(const sf::VideoMode& videoMode = sf::VideoMode(1280, 720)
	     , const std::string& title = ""
	     , const unsigned int style = sf::Style::Close | sf::Style::Resize
	     , sf::ContextSettings* ctx = nullptr)
    {
      init(videoMode, title, style, ctx);
      setup();
      window()->setFramerateLimit(static_cast<unsigned int>(FPS_LIMIT));
      const float dt = 1.f / FPS_LIMIT;

      while(mWindowPtr.get()->isOpen()) {
	sf::Event event;

	currentScene->preHandleEvent();

	while(mWindowPtr.get()->pollEvent(event)) {
	  if (event.type == sf::Event::Closed)
	  {
	    windowOpened = false;
	    mWindowPtr.get()->close();
	    break;
	  }
	  if(windowOpened) {
	    currentScene->handleEvent(event);
	  }
	}

	if(!windowOpened) {
	  break;
	}

	currentScene->postHandleEvent();
	currentScene->preUpdate(dt);

	// update here
	if(!mWindowPtr.get()->isOpen()) {
	    break;
	}

	currentScene->update(dt);

	mWindowPtr.get()->clear();
	currentScene->draw();

	mWindowPtr.get()->display();
      }
      mWindowPtr.get()->close();
    }

    sf::RenderWindow* window()
    {
      return mWindowPtr.get();
    }

  protected:
    const float FPS_LIMIT;
    std::unique_ptr<sf::RenderWindow> mWindowPtr;
    bool windowOpened;
    Scene* currentScene;

  private:
    void init(const sf::VideoMode& videoMode
	      , const std::string& title
	      , const unsigned int style
	      , sf::ContextSettings* ctxPtr)
    {
      sf::ContextSettings ctx;
      if(ctxPtr != nullptr) {
	  ctx = *ctxPtr;
      }

      window()->create(videoMode, title, style, ctx);
    }
};

#endif // APPLICATION_HPP

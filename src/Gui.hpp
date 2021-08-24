#ifndef GUI_HPP_
#define GUI_HPP_

#include "imgui.h" // necessary for ImGui::*, imgui-SFML.h doesn't include imgui.h
#include "imfilebrowser.h"
#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <string>

class Gui final
{
  public:
    Gui();
    ~Gui();
    void init(sf::RenderWindow* window);
    void handleEvent(const sf::Event& event);
    void update();
    void render();
    bool isFocused();
    void loadFont(const std::string& font, const float size);

    void setCoord(sf::Vector2i coord)
    {
      mCoord = coord;
    }

    bool doReset()
    {
      bool res = mDoReset;
      if(res)
      {
	mDoReset = false;
      }
      return res;
    }

    void setGeneration(const std::size_t generation)
    {
      mGeneration = generation;
    }

    void setPopulation(const std::size_t population)
    {
      mPopulation = population;
    }

    const bool isStepMode() const
    {
      return mStepMode;
    }

    const bool isRunning() const
    {
      return mRunning;
    }

    const bool showGrid() const
    {
      return mShowGrid;
    }

    const bool goBack() const
    {
      return mDoGoBack;
    }

    const float getStepTime() const
    {
      return mStepTime;
    }

    void stopRunning()
    {
      mRunning = false;
    }

    sf::Color getCellColor()
    {
      return sf::Color(mCellColor[0] * 255, mCellColor[1] * 255, mCellColor[2] * 255);
    }

    sf::Color getGridColor()
    {
      return sf::Color(mGridColor[0] * 255, mGridColor[1] * 255, mGridColor[2] * 255);
    }

    sf::Color getBackgroundColor()
    {
      return sf::Color(mBgColor[0] * 255, mBgColor[1] * 255, mBgColor[2] * 255);
    }

    const bool isFileDialogOpened()
    {
      return fileDialog.isOpened();
    }

    bool loadFile()
    {
      return !mDataFile.empty();
    }

    std::string getFileName()
    {
      std::string s(mDataFile);
      mDataFile = "";
      return s;
    }

private:
    sf::RenderWindow* window;
    sf::Clock clock;
    sf::Vector2i mCoord;
    std::size_t mGeneration;
    std::size_t mPopulation;
    bool mStepMode;
    bool mRunning;
    bool mDoReset;
    bool mShowGrid;
    bool mDoGoBack;
    float mStepTime;
    float mCellColor[3];
    float mGridColor[3];
    float mBgColor[3];
    std::string mDataFile;
    ImFont* mFont;
    ImVec2 min;
    ImVec2 max;
    ImGui::FileBrowser fileDialog;

    void setGui();
};

#endif /* GUI_HPP_ */

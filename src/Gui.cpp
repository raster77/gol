#include "Gui.hpp"
#include "imgui-SFML.h" // for ImGui::SFML::* functions and SFML-specific overloads
#include "SFML/Window/Mouse.hpp"

Gui::Gui()
  : window(nullptr)
  , mCoord(0.f, 0.f)
  , mGeneration(0)
  , mPopulation(0)
  , mStepMode(false)
  , mRunning(false)
  , mDoReset(false)
  , mShowGrid(true)
  , mDoGoBack(false)
  , mStepTime(0.f)
  , mCellColor{0.f, 1.f, 0.f}
  , mGridColor{0.58f, 0.58f, 0.58f}
  , mBgColor{0.18f, 0.18f, 0.18f}
  , mDataFile("")
  , mFont(nullptr)
{
}

Gui::~Gui()
{
  ImGui::SFML::Shutdown();
}

void Gui::setGui()
{
  ImGui::Begin(" ");
  if(mFont != nullptr)
  {
    ImGui::PushFont(mFont);
  }

  std::string lbl = "XY: ";
  lbl.append(std::to_string(mCoord.x)).append(" ").append(std::to_string(mCoord.y));
  lbl.append("\n").append("Generation: ").append(std::to_string(mGeneration));
  lbl.append(" ").append("Population: ").append(std::to_string(mPopulation));
  lbl.append("\n\n");
  ImGui::Text(lbl.c_str());

  ImGui::Checkbox("Step mode: ", &mStepMode);
  ImGui::Checkbox("Show grid: ", &mShowGrid);

  ImGui::SliderFloat("", &mStepTime, 0.f, 500.f, "Step time = %.0f");

  ImGui::Text("\n");

  std::string lblRun = mRunning ? "Pause" : " Run ";
  if(ImGui::Button(lblRun.c_str())) {
    mRunning = !mRunning;
  }
  ImGui::SameLine();
  if (ImGui::Button("Back")) {
    mDoGoBack = true;
  }
  ImGui::SameLine();
  if (ImGui::Button("Reset")) {
    mDoReset = true;
  }

  ImGui::Text("\n");
  if (ImGui::CollapsingHeader("Colors"))
  {
    ImGui::ColorPicker3("Cell", mCellColor, 0);
    ImGui::ColorPicker3("Grid", mGridColor, 0);
    ImGui::ColorPicker3("Background", mBgColor, 0);
  }

  if(ImGui::Button("Load pattern"))
  {
    fileDialog.open();
  }

  min.x = ImGui::GetWindowPos().x - 10;
  min.y = ImGui::GetWindowPos().y - 10;
  max.x = ImGui::GetWindowSize().x + ImGui::GetWindowPos().x + 10;
  max.y = ImGui::GetWindowSize().y + ImGui::GetWindowPos().y + 10;

  if(mFont != nullptr)
  {
    ImGui::PopFont();
  }
  ImGui::End();

  fileDialog.display();

  if(fileDialog.hasSelected())
  {
    mDataFile = fileDialog.getSelected().string();
    fileDialog.clearSelected();
  }
}

bool Gui::isFocused()
{
  const sf::Vector2f pos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(*window));
  return (pos.x >= min.x && pos.x <= max.x && pos.y >= min.y && pos.y <= max.y) || fileDialog.isOpened();
}

void Gui::init(sf::RenderWindow* renderWindow)
{
  window = renderWindow;
  ImGui::SFML::Init(*window);
  fileDialog.setTitle("title");
  fileDialog.setTypeFilters({ ".cells", ".rle" });
}

void Gui::loadFont(const std::string& font, const float size)
{
  ImGuiIO& io = ImGui::GetIO();
  mFont = io.Fonts->AddFontFromFileTTF(font.c_str(), size);
  ImGui::SFML::UpdateFontTexture();

  fileDialog.setFont(mFont);
}

void Gui::handleEvent(const sf::Event& event)
{
  ImGui::SFML::ProcessEvent(event);
}

void Gui::update()
{
  ImGui::SFML::Update(*window, clock.restart());
  setGui();
}

void Gui::render()
{
  ImGui::SFML::Render(*window);
}

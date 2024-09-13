#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

class App {
 public:
  App();

 private:
  sf::RenderWindow window_;
  std::vector<sf::VertexArray> shape_;
  sf::Transform transform_;

  static const int window_size = 1024, shape_size = 512, max_steps = 10,
                   translate_step = 1, max_scale = 40, back_scale = 30;
  constexpr static const float scale_step = 0.1;

  int step_, zoom_;

  void Run();
  void Loop();

  void HandleEvents();
  void HandleKeyPress(sf::Event::KeyEvent);

  void Render();

  void Calculate();
};
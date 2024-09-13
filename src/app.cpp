#include "app.hpp"

#include <cmath>
#include <iostream>

App::App()
    : window_(sf::VideoMode(window_size, window_size), "Koch", sf::Style::None),
      shape_(max_steps, sf::VertexArray(sf::LineStrip)),
      step_(0),
      zoom_(0) {
  Calculate();
  transform_.translate((window_size - shape_size) / 2,
                       (window_size - shape_size) / 2);
  Render();
  Run();
}

void App::Calculate() {
  shape_[0].resize(4);
  shape_[0][0] = shape_[0][3] = sf::Vector2f(0, shape_size * sqrt(3) / 2);
  shape_[0][1] = sf::Vector2f(shape_size / 2, 0);
  shape_[0][2] = sf::Vector2f(shape_size, shape_size * sqrt(3) / 2);
  for (int i = 1; i < max_steps; ++i) {
    shape_[i].resize((shape_[i - 1].getVertexCount() - 1) * 3 +
                     shape_[i - 1].getVertexCount());
    shape_[i][0] = shape_[i - 1][0];
    for (int j = 1; 4 * j < shape_[i].getVertexCount(); ++j) {
      shape_[i][j * 4] = shape_[i - 1][j];
      sf::Vector2f temporary =
          (shape_[i][j * 4].position - shape_[i][(j - 1) * 4].position) / 3.0f;
      shape_[i][j * 4 - 3].position =
          shape_[i][(j - 1) * 4].position + temporary;
      shape_[i][j * 4 - 2].position.x = shape_[i][j * 4 - 3].position.x +
                                        temporary.x * 0.5 +
                                        temporary.y * sqrt(3) / 2;
      shape_[i][j * 4 - 2].position.y = shape_[i][j * 4 - 3].position.y +
                                        temporary.y * 0.5 -
                                        temporary.x * sqrt(3) / 2;
      shape_[i][j * 4 - 1].position =
          shape_[i][(j - 1) * 4].position + temporary * 2.0f;
    }
  }
}

void App::Run() {
  while (window_.isOpen()) {
    Loop();
  };
}

void App::Loop() { HandleEvents(); }

void App::HandleEvents() {
  sf::Event event;
  while (window_.pollEvent(event)) {
    if (event.type == sf::Event::Closed) {
      window_.close();
    } else if (event.type == sf::Event::KeyPressed) {
      HandleKeyPress(event.key);
    }
  }
}

void App::HandleKeyPress(sf::Event::KeyEvent key) {
  if (key.code == sf::Keyboard::Up) {
    if (zoom_ >= max_scale) {
      if (step_ == max_steps - 1) {
        zoom_ = back_scale;
        transform_ = sf::Transform::Identity;
        transform_.translate((window_size - shape_size) / 2,
                             (window_size - shape_size) / 2);
        for (int i = 0; i < back_scale; ++i) {
          transform_.translate(-shape_size * scale_step / 2, 0)
              .scale(1 + scale_step, 1 + scale_step);
        }
        Render();
      }
    } else {
      ++zoom_;
      transform_.translate(-shape_size * scale_step / 2, 0)
          .scale(1 + scale_step, 1 + scale_step);
      Render();
    }
  } else if (key.code == sf::Keyboard::Left) {
    step_ = std::max(step_ - 1, 0);
    Render();
  } else if (key.code == sf::Keyboard::Down) {
    if (zoom_ > 0) {
      --zoom_;
      transform_ = sf::Transform::Identity;
      transform_.translate((window_size - shape_size) / 2,
                           (window_size - shape_size) / 2);
      for (int i = 0; i < zoom_; ++i) {
        transform_.translate(-shape_size * scale_step / 2, 0)
            .scale(1 + scale_step, 1 + scale_step);
      }
      Render();
    }
  } else if (key.code == sf::Keyboard::Right) {
    step_ = std::min(step_ + 1, max_steps - 1);
    Render();
  } else if (key.code == sf::Keyboard::Escape) {
    window_.close();
  }
}

void App::Render() {
  window_.clear();
  window_.draw(shape_[step_], transform_);
  window_.display();
}

#pragma once

#include "maze_game.h"

class OrdinaryRoom : public Room {
 public:
  virtual ~OrdinaryRoom() override = default;

  void Connect(const Room* that) override {}
};

class OrdinaryMazeGameCreator : public MazeGameCreator {
 public:
  virtual ~OrdinaryMazeGameCreator() override = default;

  Room* MakeRoom() override { return new OrdinaryRoom(); }
};

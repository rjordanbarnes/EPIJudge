
#pragma once

#include "maze_game.h"

class MagicRoom : public Room {
 public:
  virtual ~MagicRoom() override = default;

  void Connect(const Room* that) override {}
};

class MagicMazeGameCreator : public MazeGameCreator {
 public:
  virtual ~MagicMazeGameCreator() override = default;

  Room* MakeRoom() override { return new MagicRoom(); }
};

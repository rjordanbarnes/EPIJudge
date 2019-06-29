from maze_game import MazeGame, Room


class MagicMazeGame(MazeGame):
    def make_room(self):
        return MagicRoom()


class MagicRoom(Room):
    def connect(self, room2):
        print('Connecting magic room')


x = MagicMazeGame()
assert x is not None
assert x.make_room() is not None

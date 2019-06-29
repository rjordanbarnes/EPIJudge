
package epi;

public class OrdinaryMazeGame extends MazeGame {
  // The default constructor will call superclass constructor.
  @Override
  protected Room makeRoom() {
    return new OrdinaryRoom();
  }
}

class OrdinaryRoom implements Room {
  public void connect(Room that) {}
}

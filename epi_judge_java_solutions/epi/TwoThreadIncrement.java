
package epi;

public class TwoThreadIncrement {
  //@include
  public static int counter = 0;

  public static class IncrementThread implements Runnable {
    private int n;

    IncrementThread(int n) { this.n = n; }

    public void run() {
      for (int i = 0; i < this.n; i++) {
        TwoThreadIncrement.counter++;
      }
    }
  }

  public static void TwoThreadIncrementDriver(int n) throws Exception {
    Thread t1 = new Thread(new IncrementThread(n));
    Thread t2 = new Thread(new IncrementThread(n));

    t1.start();
    t2.start();
    t1.join();
    t2.join();

    System.out.println(counter);
  }
  //@exclude

  public static void main(String[] args) throws Exception {
    int n = (args.length > 0) ? Integer.valueOf(args[0]) : 1000000000;
    TwoThreadIncrementDriver(n);
  }
}

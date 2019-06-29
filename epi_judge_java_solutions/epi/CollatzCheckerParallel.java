

package epi;

import java.math.BigInteger;
import java.util.concurrent.TimeUnit;
import java.util.Date;
import java.util.List;
import java.util.ArrayList;
import java.util.Set;
import java.util.HashSet;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class CollatzCheckerParallel {
  //@include
  // Performs basic unit of work, i.e., checking CH for an interval
  public static class MyRunnable implements Runnable {
    public int lower;
    public int upper;

    MyRunnable(int lower, int upper) {
      this.lower = lower;
      this.upper = upper;
    }

    @Override
    public void run() {
      for (int i = lower; i <= upper; ++i) {
        CollatzCheckerParallel.CollatzCheck(i, new HashSet<BigInteger>());
      }
    }
  }

  // Checks an individual number.
  public static boolean CollatzCheck(BigInteger aNum, Set<BigInteger> visited) {
    if (aNum.equals(BigInteger.ONE)) {
      return true;
    } else if (visited.contains(aNum)) {
      return false;
    }
    visited.add(aNum);
    if (aNum.getLowestSetBit() == 1) { // Odd number.
      return CollatzCheck(
          new BigInteger("3").multiply(aNum).add(BigInteger.ONE), visited);
    }
    // Divide by 2 for even number.
    return CollatzCheck(aNum.shiftRight(1), visited);
  }

  public static boolean CollatzCheck(int aNum, Set<BigInteger> visited) {
    BigInteger b = new BigInteger(Integer.toString(aNum));
    return CollatzCheck(b, visited);
  }

  static int N = 10000000;
  static int RANGE_SIZE = 1000000;
  static int NUM_THREADS = 4;

  static void parseArgs(String[] args) {
    if (args.length >= 1) {
      N = Integer.parseInt(args[0]);
    }
    if (args.length >= 2) {
      RANGE_SIZE = Integer.parseInt(args[1]);
    }
    if (args.length >= 3) {
      NUM_THREADS = Integer.parseInt(args[2]);
    }
  }

  public static void maintest(String[] args) {
    System.out.println(
        "CollatzCheck(1): " +
        CollatzCheck(new BigInteger("1"), new HashSet<BigInteger>()));
    System.out.println(
        "CollatzCheck(3): " +
        CollatzCheck(new BigInteger("3"), new HashSet<BigInteger>()));
    System.out.println(
        "CollatzCheck(8): " +
        CollatzCheck(new BigInteger("8"), new HashSet<BigInteger>()));
    parseArgs(args);
  }

  //@include
  public static ExecutorService execute() {
    // Uses the Executor framework for task assignment and load balancing
    List<Thread> threads = new ArrayList<Thread>();
    ExecutorService executor = Executors.newFixedThreadPool(NUM_THREADS);
    for (int i = 0; i < (N / RANGE_SIZE); ++i) {
      Runnable worker =
          new MyRunnable(i * RANGE_SIZE + 1, (i + 1) * RANGE_SIZE);
      executor.execute(worker);
    }
    executor.shutdown();
    return executor;
  }
  //@exclude

  public static void main(String[] args) {
    long lDateTime = new Date().getTime();
    parseArgs(args);

    ExecutorService executor = execute();

    // while (!executor.isTerminated() ) {
    // }
    try {
      while (!executor.awaitTermination(1, TimeUnit.SECONDS)) {
      }
    } catch (Exception e) {
      e.printStackTrace();
    }

    System.out.println("Finished all threads");
    long fDateTime = new Date().getTime();
    System.out.println("time in milliseconds for checking to " + N + " is " +
                       (fDateTime - lDateTime) + " (" +
                       N / (fDateTime - lDateTime) + " per ms)");
  }
}

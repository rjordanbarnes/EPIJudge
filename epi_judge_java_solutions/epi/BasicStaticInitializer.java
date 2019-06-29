

package epi;

import java.util.HashMap;
import java.util.Map;

public class BasicStaticInitializer {

  private static double[] probabilities = new double[6];

  // Static initializer block. Executed once, when class is first acccessed.
  static {
    double sum = 0.0;
    for (int i = 0; i < 6; i++) {
      probabilities[i] = Math.random(); // Returns a number in [0,1).
      sum += probabilities[i];
    }
    // Ensure that the sum of the probabilities is 1.0.
    for (int i = 0; i < 6; i++) {
      probabilities[i] /= sum;
    }
  }
}

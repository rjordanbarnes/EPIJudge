

package epi;

import java.util.List;

public class StringConcatenation {

  public static String concat(List<String> strings) {
    String result = "";
    for (String s : strings) {
      result += s;
    }
    return result;
  }
}

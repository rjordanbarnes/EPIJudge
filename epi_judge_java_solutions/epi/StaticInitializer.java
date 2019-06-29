

package epi;

import java.util.Calendar;
import java.util.Date;
import java.util.TimeZone;

public class StaticInitializer {

  private static final Date UNIX_START;
  private static final Date UNIX_END;

  static {
    // Reuse the gmtCal object to create Date objects for
    // UNIX_START and UNIX_END.
    Calendar gmtCal = Calendar.getInstance(TimeZone.getTimeZone("GMT"));

    gmtCal.set(1970, Calendar.JANUARY, 1, 0, 0, 0);
    UNIX_START = gmtCal.getTime();

    gmtCal.set(2011, Calendar.OCTOBER, 12, 0, 0, 0);
    UNIX_END = gmtCal.getTime();
  }
}

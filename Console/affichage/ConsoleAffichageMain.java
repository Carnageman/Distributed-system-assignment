package affichage;
import common.Avion;

public class ConsoleAffichageMain {
  public static void main(String[] args) {
    if (args.length == 0) {
      System.out.println("Usage : ConsoleAffichageMain <hostname>");
    }
    else {
      (new UpdateVectorAvionThread(args[0])).run();
    }
  }
}

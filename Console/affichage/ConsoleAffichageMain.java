package affichage;
import common.Avion;

public class ConsoleAffichageMain {
  public static void main(String[] args) {
    (new UpdateVectorAvionThread(args[0])).run();
  }
}

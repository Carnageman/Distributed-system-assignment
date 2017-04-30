package affichage;
import common.Avion;

public class ConsoleAffichageMain {
  public static void main(String[] args) {
    if (args.length == 0) {
      System.out.println("Usage : ConsoleAffichageMain <hostname>");
    }
    else {
      try {
        (new UpdateVectorAvionThread(java.net.InetAddress.getByName(args[0]))).run();
      }
      catch (java.net.UnknownHostException excepHost){
        System.out.println("Unknown host !");
      }
    }
  }
}

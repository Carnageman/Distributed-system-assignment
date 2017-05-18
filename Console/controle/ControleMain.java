package controle;

public class ControleMain {
  public static void main(String[] args) {
    ConsoleSwing console;
    if (args.length != 1) { //Si il n'y a pas l'argument du hostname
      System.out.println("Usage : ControleMain <hostname>");
    }
    else {
      try {
        console = (new ConsoleSwing(java.net.InetAddress.getByName(args[0]),5842));
      }
      catch (java.net.UnknownHostException excp) { //Si le nom d'hôte n'est pas valide
        System.err.println("Le nom d'hôte est invalide !");
      }
    }
  }
}

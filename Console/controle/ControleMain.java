package controle;

public class ControleMain {
  public static void main(String[] args) {
    int i;
    String numero_vol;
    int alt = -1;
    int cap = -1;
    int vitesse = -1;
    boolean valide;
    ConsoleSwing console;
    java.util.Scanner in = new java.util.Scanner(System.in);
    CommunicationSGCA comm;
    if (args.length != 1) {
      System.out.println("Usage : ControleMain <hostname>");
    }
    else {
      try {
      console = (new ConsoleSwing(java.net.InetAddress.getByName(args[0]),5842));
      //try {
        //comm = new CommunicationSGCA(java.net.InetAddress.getByName(args[0]),5842);
        /*Ping pour voir si le SGCA est connecté*/
        /*try {
          comm.envoiOrdre(new common.OrdreDeplacement("Pingu",0,0,0)); //NOOT NOOT
        }
        catch(controle.AvionNotFoundException e) {
        }
        catch(controle.NoFreeSpaceOrderException e) {
        }
        catch(common.SGCATimeOutException e) {
          System.out.println("Le SGCA ne répond pas !");
          System.exit(-1);
        } 
        do {
          System.out.println("Entrez l'identifiant de l'appareil : ");
          numero_vol = in.nextLine();
          if (numero_vol.length() != 5) {
            System.out.println("L'identifiant doit faire 5 caractères !");
          }
        }
        while (numero_vol.length() != 5);
        do {
          valide = true;
          System.out.println("Entrez l'altitude : ");
          try {
            alt = in.nextInt();
          }
          catch (java.util.InputMismatchException e) {
            System.out.println("L'altitude doit être un entier !");
            valide = false;
          }
          in.nextLine();
        } while (!valide);
        do {
          valide = true;
          System.out.println("Entrez le cap : ");
          try {
            cap = in.nextInt();
          }
          catch (java.util.InputMismatchException e) {
            System.out.println("Le cap doit être un entier !");
            valide = false;
          }
          in.nextLine();
        } while (!valide);
        do {
          valide = true;
          System.out.println("Entrez la vitesse : ");
          try {
            vitesse = in.nextInt();
          }
          catch (java.util.InputMismatchException e) {
            System.out.println("La vitesse doit être un entier !");
            valide = false;
          }
          in.nextLine();
        } while (!valide);
        common.OrdreDeplacement dep = new common.OrdreDeplacement(numero_vol,alt,cap,vitesse);
        try {
          comm.envoiOrdre(dep);
          System.out.println("Ordre envoyé et pris en compte !");
        }

        catch(controle.AvionNotFoundException e) {
          System.out.println("L'avion n'existe pas !");
        }
        catch(controle.NoFreeSpaceOrderException e) {
          System.out.println("Le SGCA est surchargé !");
        }
        catch(common.SGCATimeOutException e) {
          System.out.println("Le SGCA ne répond pas !");
        }*/
      }
      catch (java.net.UnknownHostException excp) {
        System.out.println("Le nom d'hôte est invalide !");
      }
    }
  }
}

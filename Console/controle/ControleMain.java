package controle;

public class ControleMain {
  public static void main(String[] args) {
    int i;
    String numero_vol;
    int alt,cap,vitesse;
    java.util.Scanner in = new java.util.Scanner(System.in);
    CommunicationSGCA comm;
    try {
      do {
      System.out.println("Entrez l'identifiant de l'appareil : ");
      numero_vol = in.nextLine();
      if (numero_vol.length() != 5) {
        System.out.println("L'identifiant doit faire 5 caractères !");
      }
      }
      while (numero_vol.length() != 5);
      System.out.println("Entrez l'altitude : ");
      alt = in.nextInt();
      in.nextLine();
      System.out.println("Entrez le cap : ");
      cap = in.nextInt();
      in.nextLine();
      System.out.println("Entrez la vitesse : ");
      vitesse = in.nextInt();
      in.nextLine();
      comm = new CommunicationSGCA(java.net.InetAddress.getByName(args[0]),5842);
      common.OrdreDeplacement dep = new common.OrdreDeplacement(numero_vol,alt,cap,vitesse);
      try {comm.envoiOrdre(dep);}
      catch(controle.AvionNotFoundException e) {
        System.out.println("L'avion n'existe pas !");
      }
      catch(controle.NoFreeSpaceOrderException e) {
        System.out.println("Le SGCA est surchargé !");
      }
      catch(common.SGCATimeOutException e) {
        System.out.println("Le SGCA ne répond pas !");
      }
    }
    catch (java.net.UnknownHostException excp) {

    }
    //while(true) {
    //Interaction pour obtenir les informations de l'ordre
    //Appel d'une méthode d'une classe pour envoyer l'ordre (lève des exceptions si erreur)
    //Réaction aux différentes exceptions ou au cas où il n'y a pas d'exceptions
   // }
  }
}

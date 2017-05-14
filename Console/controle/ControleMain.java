package controle;

public class ControleMain {
  public static void main(String[] args) {
    int i;
    CommunicationSGCA comm;
    try {
    comm = new CommunicationSGCA(java.net.InetAddress.getByName(args[0]),5842);
    common.OrdreDeplacement dep = new common.OrdreDeplacement("Hibou",500,360,5);
    comm.envoiOrdre(dep);
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

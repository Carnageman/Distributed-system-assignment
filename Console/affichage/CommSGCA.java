package affichage;
import java.util.Vector;
import java.net.DatagramPacket;
import common.Avion;

public class CommSGCA {
  private java.net.InetAddress adr; //Adresse du serveur SGCA
  private int port; //Port du serveur SGCA
  private int nbAvion; //Garde en mémoire la dernière valeur de nbAvion reçue du serveur
  private java.net.DatagramSocket socket;
  public CommSGCA(java.net.InetAddress addr,int portServer) {
    adr = addr;
    port = portServer;
    nbAvion = 0;
    try {
      socket = new java.net.DatagramSocket();
      socket.setSoTimeout(250);
    }
    catch (java.net.SocketException excp) {
      System.err.println("Création de socket impossible !");
      System.err.println("Erreur : " + excp.getMessage());
    }
  }
  public Vector<Avion> getAvions() throws common.SGCATimeOutException {
    int nbTry = 0;
    Vector<Avion> vectAvion = new Vector<Avion>();
    DatagramPacket packetReception = new DatagramPacket(new byte[50],50);
    DatagramPacket packetDemandeAvions = PacketsAffichage.makePacketDemandeAvions(adr,port,nbAvion);
    while(true) { //Sortie lors du return
      try {
        socket.send(packetDemandeAvions);
      }
      catch (java.io.IOException excp) {
        System.err.println("La socket UDP a été fermée !");
        System.err.println("Erreur : " + excp.getMessage());
      }
      try {
        while(true) { //Sortie quand il y a timeOut du receive
          try {
          socket.receive(packetReception);
          nbTry = 0; //On a bien reçu un paquet, on remet les essais à 0
          if (packetReception.getLength() == 8) {
            nbAvion = java.nio.ByteBuffer.wrap(packetReception.getData(),4,4).getInt();
            if (nbAvion == 0) {
              vectAvion.clear();
              return vectAvion;
            }
            packetDemandeAvions = PacketsAffichage.makePacketDemandeAvions(adr,port,nbAvion);
            socket.send(packetDemandeAvions);
          } 
          if (packetReception.getLength() == 32) {
            vectAvion.add(Avion.fromBytes(packetReception.getData()));
            if (vectAvion.size() == nbAvion) {
              return vectAvion;
            }
          }
        }
        catch (java.net.SocketTimeoutException e) {
          nbTry++;
          if (nbTry > 10) {
            throw new common.SGCATimeOutException();
          }
          vectAvion.clear();
          break; //Break du second tant que pour renvoyer un message
        }
      }
      }
      catch (java.io.IOException excp) {
        System.err.println("La socket a été fermée !");
        System.err.println("Erreur : " + excp.getMessage());
      }
    }
  }
}

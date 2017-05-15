package controle;

public class CommunicationSGCA {
  private java.net.InetAddress adr;
  private int port;
  private java.net.DatagramSocket socket;
  public CommunicationSGCA(java.net.InetAddress addr,int portServer) {
    adr = addr;
    port = portServer;
    try {
      socket = new java.net.DatagramSocket();
      socket.setSoTimeout(250);
    }
    catch (java.net.SocketException excp) {
      System.err.println("Création de socket impossible !");
      System.err.println("Erreur : " + excp.getMessage());
    } 
  }

  public void envoiOrdre(common.OrdreDeplacement dep) throws AvionNotFoundException, NoFreeSpaceOrderException, common.SGCATimeOutException {
    int nbTry = 0;
    int reponse;
    java.net.DatagramPacket packetReception = new java.net.DatagramPacket(new byte[50],50);
    try {
    while(true) {
        socket.send(dep.makePacketOrdre(adr,port));
        try {
          java.nio.ByteBuffer buffer;
          socket.receive(packetReception);
          buffer = java.nio.ByteBuffer.wrap(packetReception.getData());
          if ((packetReception.getLength() == 8) && (buffer.getInt(0) == 56841)) { 
            reponse = buffer.getInt(4);
            System.out.println(reponse);
            if (reponse == 0) {
              System.out.println("Bien reçu !");
              break;
            }
            if (reponse == 1) { //Si l'avion n'existe pas dans la base
              throw new AvionNotFoundException();
            }
            if (reponse == 2) { //Si il n'y a plus de place dans la base des ordres
              throw new NoFreeSpaceOrderException();
            }
          }
        }
        catch (java.net.SocketTimeoutException excpSock) {
          nbTry++;
          if (nbTry > 10) {
            throw new common.SGCATimeOutException();
          }
        }
      }
    }
    catch(java.io.IOException excp) {
      System.err.println("La socket a été fermée !");
      System.err.println("Erreur : " + excp.getMessage());
    }
  }     
}

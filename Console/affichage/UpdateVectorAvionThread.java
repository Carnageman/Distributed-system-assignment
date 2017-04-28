package affichage;
import common.Avion;
import java.util.Vector;
import java.net.DatagramSocket;
import java.net.DatagramPacket;
import java.net.SocketException;
import java.io.IOException;

public class UpdateVectorAvionThread extends Thread {
  String hostName;
  private Vector<Avion> vecteurAvion;
  private DatagramSocket socket;
  private java.net.InetAddress adr;
  public UpdateVectorAvionThread(String host) {
    hostName = host;
    try {
      socket = new DatagramSocket();
    }
    catch (SocketException excp) {
      System.err.println("Création de socket impossible !");
      System.err.println("Erreur : " + excp.getMessage());
    }
    vecteurAvion = new Vector<Avion>();
  }
  public UpdateVectorAvionThread(Vector<Avion> vecteur) {
    vecteurAvion = vecteur;
    try {
      socket = new DatagramSocket();
    }
    catch (SocketException excp) {
      System.err.println("Création de socket impossible !");
      System.err.println("Erreur : " + excp.getMessage());
    }
  }
  @Override
  public void run() {
    Avion testAvion;
    DatagramPacket packet = new DatagramPacket(new byte[28],28);
    DatagramPacket packetDemande = new DatagramPacket(new byte[4],4);
    java.net.InetAddress adr;
    byte[] magicnbr = java.nio.ByteBuffer.allocate(4).putInt(56841).array();
    java.nio.ByteBuffer demande = java.nio.ByteBuffer.allocate(8).putInt(0,56841);
    int nbAvion;
    try {
      adr = java.net.InetAddress.getByName(hostName);
      while (true) {
        try {
          socket.setSoTimeout(250);
          packetDemande.setAddress(adr);
          packetDemande.setPort(5842);
          packetDemande.setData(magicnbr);
          System.err.println("Envoi d'une demande : " + packetDemande.getLength());
          socket.send(packetDemande);
        }
        catch (IOException excp) {
          System.err.println("La socket a été fermée !");
          System.err.println("Erreur : " + excp.getMessage());
        }
        try {
          //Penser à tester si nbAvion == 0
          while(true) {
            socket.receive(packet);
            if (packet.getLength() ==  8) {
         	    nbAvion = java.nio.ByteBuffer.wrap(packet.getData(),4,4).getInt();
              System.out.println(nbAvion);
              packetDemande.setData(demande.putInt(4,nbAvion).array());
              socket.send(packetDemande);
            }
            if (packet.getLength() == 32) {
              testAvion = Avion.fromBytes(java.nio.ByteBuffer.wrap(packet.getData(),4,28).array());
              vecteurAvion.add(testAvion);
            }
          }
        }
        catch (java.net.SocketTimeoutException excpSock) {
          System.err.println("Timeout !");
        }
        catch (IOException excp) {
          System.err.println("La socket a été fermée !");
          System.err.println("Erreur : " + excp.getMessage());
        }
      }
    }
    catch (java.net.UnknownHostException excpHost) {
      System.err.println("Adresse introuvable !");
    }
  }
}

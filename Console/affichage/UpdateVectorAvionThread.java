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
  public UpdateVectorAvionThread(String host,Vector<Avion> vecteur) {
    hostName = host;
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
    DatagramPacket packet = new DatagramPacket(new byte[50],50);
    DatagramPacket packetDemande = new DatagramPacket(new byte[4],4);
    java.net.InetAddress adr;
    byte[] magicnbr = java.nio.ByteBuffer.allocate(4).putInt(56841).array();
    java.nio.ByteBuffer demande = java.nio.ByteBuffer.allocate(8).putInt(0,56841);
    int nbAvion;
    boolean fini;
    try {
      adr = java.net.InetAddress.getByName(hostName);
      while (true) {
        nbAvion = 0;
        fini = true;
        try {
          socket.setSoTimeout(250);
          packetDemande.setAddress(adr);
          packetDemande.setPort(5842);
          packetDemande.setData(magicnbr);
          socket.send(packetDemande);
          vecteurAvion.clear();
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
              if (nbAvion == 0) break;
              packetDemande.setData(demande.putInt(4,nbAvion).array());
              socket.send(packetDemande);
            }
            if (packet.getLength() == 32) {
              testAvion = Avion.fromBytes(java.nio.ByteBuffer.wrap(packet.getData(),4,28).array());
              testAvion.afficher_donnees();
              vecteurAvion.add(testAvion);
            }
          }
        }
        catch (java.net.SocketTimeoutException excpSock) {
          fini = (nbAvion == vecteurAvion.size());
        }
        catch (IOException excp) {
          System.err.println("La socket a été fermée !");
          System.err.println("Erreur : " + excp.getMessage());
        }
        try {
          if (fini) {
            Thread.sleep(5000);
          }
        }
        catch(InterruptedException e) {
        }
      }
    }
    catch (java.net.UnknownHostException excpHost) {
      System.err.println("Adresse introuvable !");
    }
  }
}

package affichage;
import common.Avion;
import java.util.Vector;
import java.net.DatagramSocket;
import java.net.DatagramPacket;
import java.net.SocketException;
import java.io.IOException;
import affichage.CommSGCA;

public class UpdateVectorAvionThread extends Thread {
  private CommSGCA comSGCA;
  public UpdateVectorAvionThread(java.net.InetAddress addr) {
    comSGCA = new CommSGCA(addr,5842); 
  } 
  private void clearScreen() {
    final String ANSI_CLS = "\u001b[2J";
    final String ANSI_HOME = "\u001b[H";
    System.out.print(ANSI_CLS + ANSI_HOME);
    System.out.flush();
  }
  @Override
  public void run() {
    Vector<Avion> vectAvion;
    int i;
    try {
    while (true) {
      vectAvion = comSGCA.getAvions();
      clearScreen();
      for (i=0;i<vectAvion.size();i++) {
        vectAvion.get(i).afficher_donnees();
      }
      try {
        Thread.sleep(5000);
      }
      catch (InterruptedException excp) {
      }
    }  
    }
    catch (common.SGCATimeOutException e) {
      System.out.println("Connexion avec le serveur perdue !");
    }
  }
}

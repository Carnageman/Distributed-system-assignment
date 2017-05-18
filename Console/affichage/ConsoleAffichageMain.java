package affichage;
import java.util.Vector;
import common.Avion;
import javax.swing.JFrame;
import javax.swing.text.TableView;

public class ConsoleAffichageMain {
  public static void main(String[] args) {
    java.net.InetAddress addr;
    java.util.Vector<Avion> vectAvion;
    affichage.CommSGCA sgca;
    if (args.length != 1) {
      System.out.println("Usage : ConsoleAffichageMain <hostname>");
    }
    else {
      try {
        addr = java.net.InetAddress.getByName(args[0]);
        javax.swing.JFrame frame = new JFrame("Console d'affichage");
        javax.swing.JTable table = new javax.swing.JTable();
        javax.swing.JScrollPane scrollpane = new javax.swing.JScrollPane(table);
        frame.getContentPane().add(scrollpane);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.pack();
        frame.setVisible(true);
        sgca = new affichage.CommSGCA(addr,5842);
        //(new UpdateVectorAvionThread(addr)).run();
        while(true) {
            vectAvion = sgca.getAvions();
            table.setModel(new TableModelAvion(vectAvion));
          
          try {
            Thread.sleep(2000);
          }
          catch (InterruptedException excp) {
          }
        }
      }
      catch (java.net.UnknownHostException excepHost){
        System.out.println("Le nom d'hôte est invalide !");
      }
      catch (common.SGCATimeOutException e) {
        System.exit(1);
      }
    }
  }
}

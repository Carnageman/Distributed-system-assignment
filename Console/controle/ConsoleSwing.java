package controle;
import java.awt.*;
import java.awt.event.ActionEvent;
import javax.swing.*;



public class ConsoleSwing extends JFrame {
     
    private JButton ordre = new JButton("Envoyer ordre");
    private JTextField alt = new JTextField();
    private JTextField vit = new JTextField();
    private JTextField cap = new JTextField();
    private JTextField nvol = new JTextField();
    private JLabel res = new JLabel();
    private java.net.InetAddress adresse;
    private int port;
    public ConsoleSwing(java.net.InetAddress addr, int p) { 
        super("Console de contrôle");
        adresse = addr;
        port = p;
        setSize(new Dimension(300,130));          
        
        JButton aide = new JButton("Aide");
        
        getRootPane().setDefaultButton(ordre);
        ordre.addActionListener(new ActionEnvoyer(nvol,cap,vit,alt,res,adresse,port));
        aide.addActionListener(null);

        JPanel pan=new JPanel(); 
        BoxLayout bl=new BoxLayout(pan,BoxLayout.Y_AXIS);   
        pan.setLayout(bl); 
        
        JPanel corps= new JPanel(new GridLayout(4,2));        
        corps.add(new JLabel("Numéro de vol : "));
        corps.add(nvol);
        corps.add(new JLabel("Cap : "));
        corps.add(cap);
        corps.add(new JLabel("Vitesse : "));
        corps.add(vit);
        corps.add(new JLabel("Altitude : "));
        corps.add(alt);       
        pan.add(corps);
        
        JPanel bottom = new JPanel();
        bottom.add(ordre);     
        pan.add(bottom);
        
        JPanel resultat = new JPanel();
        resultat.add(res);
        pan.add(resultat);

        setContentPane(pan);        
        setVisible(true);           
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE); 
    }
    
    public void actionPerformed(ActionEvent e) {
        if(e.getSource() == ordre) {
            
        }
        else {
            
        }
    }
    
}

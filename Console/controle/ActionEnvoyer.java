package controle;
import javax.swing.JTextField;
import javax.swing.JLabel;
import java.awt.*;
import java.awt.event.ActionEvent;
import common.OrdreDeplacement;


public class ActionEnvoyer implements java.awt.event.ActionListener {    
  CommunicationSGCA comm;
  JTextField nvol;
  JTextField ca;
  JTextField vi;
  JTextField al;
  JLabel res;
    
  public ActionEnvoyer(JTextField nvol,JTextField ca,JTextField vi, JTextField al, JLabel res,java.net.InetAddress addr, int port) { 
    comm = new CommunicationSGCA(addr,port);
    try { //Ping pour voir si le SGCA est bien lancé à l'adresse donnée
      comm.envoiOrdre(new common.OrdreDeplacement("Pingu",0,0,0)); //NOOT NOOT
    }
    catch(controle.AvionNotFoundException e) {
    }
    catch(controle.NoFreeSpaceOrderException e) {
    }
    catch(common.SGCATimeOutException e) { //Si le SGCA n'a pas répondu, on en déduit qu'il n'est pas lancé, rien ne sert de continuer
      System.err.println("Le SGCA ne répond pas !");
      System.exit(-1);
    }
    this.nvol = nvol;
    this.ca = ca;
    this.vi = vi;
    this.al = al;
    this.res = res;
  }
    
  public void actionPerformed(ActionEvent e) {     
	  boolean lancer = true;
	  int cap = -1;
	  int vitesse = -1;
	  int altitude = -1;
	  if (nvol.getText().length() != 5) {
		  res.setText("Le numéro vol doit faire 5 caractères !");
		  lancer = false;
	  }
	  String numero_vol = nvol.getText();
	  if(ca.getText().equals("")) {
		  cap = -1;
	  }
	  else {
		  try {
			  cap = Integer.parseInt(ca.getText());
		  }
		  catch (java.lang.NumberFormatException excp) {
			  res.setText("Le cap n'est pas un entier !");
			  lancer = false;
		  }
	  }
	  if(vi.getText().equals("")) {
		  vitesse = -1;
	  }
	  else {
		  try {
			  vitesse = Integer.parseInt(vi.getText());
		  }
		  catch (java.lang.NumberFormatException excp) {
			  res.setText("La vitesse n'est pas un entier !");
			  lancer = false;
		  }
	  }
	  if(al.getText().equals("")) {
		  altitude = -1;
	  }
	  else {
		  try {
			  altitude = Integer.parseInt(al.getText());
		  }
		  catch (java.lang.NumberFormatException excp) {
			  res.setText("L'altitude n'est pas un entier !");
			  lancer = false;
		  }
	  }
	  OrdreDeplacement ordre = new OrdreDeplacement(numero_vol,altitude,cap,vitesse);
	  if (lancer) {
		  try {
			  comm.envoiOrdre(ordre);
        nvol.setText("");
        ca.setText("");
        vi.setText("");
        al.setText("");
			  res.setText("Ordre envoyé et pris en compte");
		  }
		  catch(controle.AvionNotFoundException ex) {
			  res.setText("L'avion n'existe pas !");
		  }
		  catch(controle.NoFreeSpaceOrderException ex) {
			  res.setText("Le SGCA est surchargé !");
		  }
		  catch(common.SGCATimeOutException ex) {
			  System.err.println("Le SGCA ne réponds pas !");
        System.exit(-1);
		  }
	  }      
  }
}

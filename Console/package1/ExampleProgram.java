package package1;
import package2.ExampleIO;

public class ExampleProgram {
  public static void main(String[] args){
    ExampleIO jeanmi = new ExampleIO();
    java.util.Vector<String> vecteur = new java.util.Vector<String>();
    vecteur.add("Bonjour");
    //ExamplePasIO michel = new ExamplePasIO();
    System.out.println("I'm a Simple Program");
    System.out.println(vecteur.get(0));
    jeanmi.afficher();
  }
}

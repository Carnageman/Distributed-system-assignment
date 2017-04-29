package common;

public class Avion {
  private String nomAvion;
  private int posx;
  private int posy;
  private int altitude;
  private int cap;
  private int vitesse;

  public Avion(String nom, int x, int y, int alt, int cp, int vites) {
    nomAvion = nom;
    posx = x;
    posy = y;
    altitude = alt;
    cap = cp;
    vitesse = vites;
  }

  public String getNomAvion() {
    return nomAvion;
  }
  public int getPosX() {
    return posx;
  }
  public int getPosY() {
    return posy;
  }
  public int getAltitude() {
    return altitude;
  }
  public int getCap() {
    return cap;
  }
  public int getVitesse() {
    return vitesse;
  }

  public static Avion fromBytes(byte[] array) {
    Avion a;
    java.nio.ByteBuffer buffer = java.nio.ByteBuffer.wrap(array);

    a = new Avion(new String(array,4,8),buffer.getInt(12),buffer.getInt(16),buffer.getInt(20),buffer.getInt(24),buffer.getInt(28));
    return a;
  }

  public void afficher_donnees() {
    System.out.println("Avion : " + getNomAvion());
    System.out.println("Position (x,y) : (" + getPosX() + "," + getPosY() + ")");
    System.out.println("Altitude : " + getAltitude());
    System.out.println("Cap : " + getCap());
    System.out.println("Vitesse : " + getVitesse());
  }
}

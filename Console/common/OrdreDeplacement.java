package common;

public class OrdreDeplacement {
  private String numero_vol;
  private int altitude;
  private int cap;
  private int vitesse;

  public OrdreDeplacement(String num,int alt,int c,int vit) {
    numero_vol = num;
    altitude = alt;
    cap = c;
    vitesse = vit;
  }

  public String getNumero_vol() {
    return numero_vol;
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

  public java.net.DatagramPacket makePacketOrdre(java.net.InetAddress addr,int portServer) {
    byte[] numArray;
    java.net.DatagramPacket packet = new java.net.DatagramPacket(new byte[24],24);
    java.nio.ByteBuffer buffer = java.nio.ByteBuffer.wrap(packet.getData());
    try {
      numArray = getNumero_vol().getBytes("US-ASCII");
      packet = new java.net.DatagramPacket(new byte[24],24);
      buffer = java.nio.ByteBuffer.wrap(packet.getData());
      buffer.putInt(56841);
      buffer.put(numArray,0,5);
      buffer.put((byte) 0); //On doit rajouter 3 bytes pour car il y a un offset de 3 dans la structure C
      buffer.put((byte) 0);
      buffer.put((byte) 0);
      buffer.putInt(getCap());
      buffer.putInt(getVitesse());
      buffer.putInt(getAltitude());
      packet.setAddress(addr);
      packet.setPort(portServer);
    }
    catch (java.io.UnsupportedEncodingException excp) {
      System.err.println("Encodage introuvable !");
      packet = null;
    }
    return packet;
  }
}

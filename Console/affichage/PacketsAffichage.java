package affichage;
import java.net.DatagramPacket;
public class PacketsAffichage {
  public static DatagramPacket makePacketDemandeAvions(java.net.InetAddress adr,int port,int nbAvion) {
    DatagramPacket packet = new DatagramPacket(new byte[8],8);
    java.nio.ByteBuffer.wrap(packet.getData()).putInt(0,56841);
    java.nio.ByteBuffer.wrap(packet.getData()).putInt(4,nbAvion);
    packet.setAddress(adr);
    packet.setPort(port);
    return packet;
  }
}

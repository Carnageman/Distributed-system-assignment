package affichage;
import java.util.Vector;
import common.Avion;
import javax.swing.table.AbstractTableModel;
public class TableModelAvion extends AbstractTableModel {
  private Vector<Avion> vector;
  private String[] columnNames = {"Numéro vol","X","Y","Altitude","Vitesse","Cap"};
  public TableModelAvion(Vector<Avion> vect) {
    vector = vect;
  }

  public void setVector(Vector<Avion> vect) {
    vector = vect;
  }
  public String getColumnName(int columnIndex) {
    return columnNames[columnIndex];
  }
 
  public int getRowCount() {
    return vector.size();
  }

  public int getColumnCount() {
    return columnNames.length;
  }

  public Object getValueAt(int rowIndex, int columnIndex) {
    Avion a = vector.get(rowIndex);
    switch (columnIndex) {
      case 0:
        return a.getNomAvion();
      case 1:
        return a.getPosX();
      case 2:
        return a.getPosY();
      case 3:
        return a.getAltitude();
      case 4:
        return a.getVitesse();
      case 5:
        return a.getCap();
    }
    return null;
  }

  public Class<?> getColumnClass(int columnIndex) {
    switch (columnIndex) {
      case 0:
        return String.class;
      case 1:
        return Integer.class;
      case 2:
        return Integer.class;
      case 3:
        return Integer.class;
      case 4:
        return Integer.class;
      case 5:
        return Integer.class;
    }
    return null;
  }
}

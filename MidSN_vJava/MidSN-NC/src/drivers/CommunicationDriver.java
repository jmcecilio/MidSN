/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package drivers;

import ioadapter.MidSN_packet;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.logging.Level;
import java.util.logging.Logger;



/**
 *
 * @author cecilio
 */
public class CommunicationDriver implements Runnable{
    private boolean msgWaiting;

    public CommunicationDriver(int port) {
        this.port = port;
    }

    public CommunicationDriver() {
        this.port = 9000;
    }
    
    
    
    public int new_message_arrives = 0;
    Socket s;
    private DataOutputStream out;
    private DataInputStream in;

    private MidSN_packet packet = new MidSN_packet();
    
    private int port = 4444, maxConnections = 0;
    

    public void open_connection(String ip, int serverPort) {
        try {
            s = new Socket(ip, serverPort);
            createInputOutputStrems();
        } catch (IOException e) {
            System.out.println("Socket:" + e.getMessage());
        }
    }

    public void close_connection() {
        if (s != null) {
            try {
                s.close();
            } catch (IOException e) {/*close
                 failed*/
            }
        }
    }


    public void send_to(String address, byte[] msg, int size) {
        try {
            out.writeInt(size);
            if (size > 0) {
                out.write(msg);
            }
        } catch (IOException ex) {
            System.out.println("DRIVER (send_to):" + ex.getMessage());
        }
    }

    @Override
    public void run() {
        int i = 0;
        try {
            ServerSocket listener = new ServerSocket(port);
            Socket server;
            while ((i++ < maxConnections) || (maxConnections == 0)) {
                server = listener.accept();
                doComms conn_c = new doComms(server, this);
                Thread t = new Thread(conn_c);
                t.start();
            }
        } catch (IOException ioe) {
            System.out.println("IOException on socket listen: " + ioe);
        }
    }


    public byte[] midsn_get_received_packet() {
        return getPacket().getBytes();
    }

    private void createInputOutputStrems() {
        try {
            out = new DataOutputStream(s.getOutputStream());
            in = new DataInputStream(s.getInputStream());
        } catch (IOException ex) {
            Logger.getLogger(CommunicationDriver.class.getName()).log(Level.SEVERE, null, ex);
        }
    }

    public void setMSGWaiting(boolean b) {
        msgWaiting = b;
    }
    
    public boolean getMSGWaiting() {
        return msgWaiting;
    }

    /**
     * @return the packet
     */
    public MidSN_packet getPacket() {
        return packet;
    }

    /**
     * @param packet the packet to set
     */
    public void setPacket(MidSN_packet packet) {
        this.packet = packet;
    }
    
}
class doComms implements Runnable {
    private Socket server;
    CommunicationDriver commDriver;
    doComms(Socket server, CommunicationDriver commDriver) {
        this.server = server;
        this.commDriver = commDriver;
    }
    @Override
    public void run() {
//        System.out.println("Listener started...");

        try {
            // Get input from the client
            DataInputStream in = new DataInputStream(server.getInputStream());

            byte[] data = null;
            try {
                int len = in.readInt();
                data = new byte[len];
                if (len > 0) {
                    in.readFully(data);
                    commDriver.setMSGWaiting(true);
                }
            } catch (IOException ex) {
                System.out.println("DRIVER (run):" + ex.getMessage());
            }

            
            MidSN_packet packet = new MidSN_packet();
            packet.createPacket(data);

            commDriver.setPacket(packet);



            server.close();
        } catch (IOException ioe) {
            System.out.println("IOException on socket listen: " + ioe);
        }
    }
}
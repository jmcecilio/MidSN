/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package tests;

import ioadapter.MidSN_packet;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;

/**
 *
 * @author cecilio
 */
public class MidSNMainTester {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        // TODO code application logic here

        MidSNMainTester node = new MidSNMainTester();
        node.run();
    }
    
    private static int port = 4444, maxConnections = 0;

    public void run() {

        int i = 0;

        try {
            ServerSocket listener = new ServerSocket(port);
            Socket server;

            while ((i++ < maxConnections) || (maxConnections == 0)) {

                server = listener.accept();
                doComms conn_c = new doComms(server);
                Thread t = new Thread(conn_c);
                t.start();
            }
        } catch (IOException ioe) {
            System.out.println("IOException on socket listen: " + ioe);
        }
    }
}




class doComms implements Runnable {

    private Socket server;
    private String line, input;

    doComms(Socket server) {
        this.server = server;
    }

    @Override
    public void run() {
        
        System.out.println("Listener started...");

        input = "";

        try {
            // Get input from the client
            DataInputStream in = new DataInputStream(server.getInputStream());
            DataOutputStream out = new DataOutputStream(server.getOutputStream());
            
            byte[] data = null;
            try {
                int len = in.readInt();
                data = new byte[len];
                if (len > 0) {
                    in.readFully(data);
                }
            } catch (IOException ex) {
                System.out.println("DRIVER (run):" + ex.getMessage());
            }

            MidSN_packet packet = new MidSN_packet();
            packet.createPacket(data);

            packet.printPacket();


            server.close();
        } catch (IOException ioe) {
            System.out.println("IOException on socket listen: " + ioe);
        }
    }
}

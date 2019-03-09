/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package tests;

import drivers.CommunicationDriver;
import ioadapter.MidSN_packet;
import java.util.logging.Level;
import java.util.logging.Logger;
import midsn.nc.Definitions;





/**
 *
 * @author cecilio
 */
public class MidSNMainTester4 {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        // TODO code application logic here

        MidSNMainTester4 node = new MidSNMainTester4();
        node.run();
    }
    
    private static int port = 4444, maxConnections = 0;

    
    
   
    
    public MidSN_packet dataPacketTest(){
        String myIp = "192.168.123.10";
        String TargetIp = "192.168.123.10";
        

        
        MidSN_packet packet = new MidSN_packet();
        packet.createPacket();

        packet.addAddressToPacket(myIp);
        packet.addAddressToPacket(TargetIp);

        packet.addIntToPacket(Definitions.MSG_DATA);
        packet.addIntToPacket(0x0000);
        packet.addLongToPacket(System.currentTimeMillis());
        packet.addIntToPacket(100);
        
        packet.addIntToPacket(1 + 2);
        
//        packet.addIntToPacket(1);
//        
//        packet.addIntToPacket(5000);
//        
//        packet.addIntToPacket(6380);
        
        
        return packet;
    }
    
    
    
    public void run() {
        
        CommunicationDriver comm = new CommunicationDriver(9001);
        comm.open_connection("localhost", 9000);
        MidSN_packet pkt = dataPacketTest();

        
        byte [] msg = pkt.getBytes();
        comm.send_to("localhost", msg, pkt.getPosition());
        comm.close_connection();
        
        
        }

   
}

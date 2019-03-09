/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package tests;

import drivers.CommunicationDriver;
import ioadapter.MidSN_packet;
import java.util.ArrayList;
import midsn.nc.Definitions;
import nc_ginapp_aa.Field;
import nc_ginapp_aa.Stream;
import nc_ginapp_aa.Where;





/**
 *
 * @author cecilio
 */
public class MidSNMainTester2 {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        // TODO code application logic here

        MidSNMainTester2 node = new MidSNMainTester2();
        node.run();
    }
    
    private static int port = 4444, maxConnections = 0;

    
    
    public Stream createStream() {

        Stream stream = new Stream();
        stream.setStreamID(1);
        stream.setIsIp(true);
        stream.setIpTo("localhost");
        stream.setIpPort(9000);
//        stream.setIpPort(4444);
        stream.setIsAlarm(false);
        stream.setRimeTo(0x0001);
        stream.setSend_rate(5000);
        stream.setSense_rate(1000);
        stream.setWindow_size(1);
        stream.setRleOn(false);
        stream.setRleInterval(100);
        stream.setRleIndex(Definitions.MAXINT);
        stream.setLast_sendTS(System.currentTimeMillis());
        stream.setRleAliveTime(10000);
        stream.setTimeStartRle(System.currentTimeMillis());
        stream.setRleMaxSize(25);
        stream.setIsIntervalRel(true);
        stream.setnSeq(0);

        stream.setIs_one_time_query(false);
        
        ArrayList <Field> select = new ArrayList<Field>();
        select.add(new Field(Definitions.FFIELD_sensor_LASTSENDINTERVAL, Definitions.NONE));
        select.add(new Field(Definitions.SENSOR_TEMPERATURE, Definitions.FFIELD_VALUE));
        select.add(new Field(Definitions.FFIELD_sensor_END, Definitions.FFIELD_VALUE));
        stream.setSelect(select);

        ArrayList <Where> where = new ArrayList<Where>();
        where.add(
                new Where(
                new Field(Definitions.FFIELD_sensor_END, Definitions.NONE),
                Definitions.NONE,
                new Field(Definitions.FFIELD_sensor_END, Definitions.NONE)));
        stream.setWhere(where);

        
        stream.setDataValues( new ArrayList());


        return stream;
    }
    
    public MidSN_packet dataPacketTest(){
        String myIp = "192.168.123.10";
        String TargetIp = "192.168.123.10";
        int IPport = 9000;

        
        MidSN_packet packet = new MidSN_packet();
        packet.createPacket();

        packet.addAddressToPacket(myIp);
        packet.addAddressToPacket(TargetIp);
        packet.addIntToPacket(IPport);

        packet.addIntToPacket(Definitions.MSG_DATA);
        packet.addIntToPacket(0x0000);
        packet.addLongToPacket(System.currentTimeMillis());
        packet.addIntToPacket(100);
        
        packet.addIntToPacket(1 + 2);
        
        packet.addIntToPacket(1);
        
        packet.addIntToPacket(5000);
        
        packet.addIntToPacket(6380);
        
        
        return packet;
    }
    
    
    
    public void run() {
        
        CommunicationDriver comm = new CommunicationDriver(9001);
        comm.open_connection("localhost", 9000);
//        MidSN_packet pkt = dataPacketTest();
        
        Stream stream = createStream();
        MidSN_packet pktConfig = createPacketFromStreamConfiguration(stream);
        
        
        
        byte [] msg = pktConfig.getBytes();
        comm.send_to("localhost", msg, pktConfig.getPosition());
        comm.close_connection();
        
        
        }

    private MidSN_packet createPacketFromStreamConfiguration(Stream stream) {
        
        
        MidSN_packet packet = new MidSN_packet();
        packet.createPacket();

        packet.addAddressToPacket(stream.getIpTo());
        packet.addAddressToPacket(stream.getIpTo());

        packet.addIntToPacket(stream.getIpPort());

        packet.addIntToPacket(Definitions.MSG_AGENT);
        packet.addIntToPacket(Definitions.CTRL_GP_NEW_STREAM);
        packet.addLongToPacket(System.currentTimeMillis());

        packet.addIntToPacket(321);
        
      
        

        packet.addIntToPacket(78 + 1); // SIZE

        packet.addIntToPacket(stream.getStreamID());
        
        if (stream.getIsIp()) {
            packet.addIntToPacket(1);
            packet.addAddressToPacket(stream.getIpTo());
            packet.addIntToPacket(stream.getIpPort());
        }
        else {
            packet.addIntToPacket(0);
            packet.addIntToPacket(stream.getRimeTo());
        }
        
        
        packet.addIntToPacket(stream.getSend_rate());
        packet.addIntToPacket(stream.getSense_rate());
        packet.addIntToPacket(stream.getWindow_size());
        

        if (stream.getIs_one_time_query()) {
            packet.addIntToPacket(1);
        }
        else {
            packet.addIntToPacket(0);
        }

        
        ArrayList <Field> select = stream.getSelect();
        ArrayList <Where> where = stream.getWhere();
        
        packet.addIntToPacket(select.size());
        packet.addIntToPacket(where.size());
        
        for (int i=0; i<select.size(); i++){
            packet.addIntToPacket(select.get(i).getSensor());
            packet.addIntToPacket(select.get(i).getMetric());
        }
        
        for (int i=0; i<where.size(); i++){
            packet.addIntToPacket(where.get(i).getField1().getSensor());
            packet.addIntToPacket(where.get(i).getField1().getMetric());
            packet.addIntToPacket(where.get(i).getOperator());
            packet.addIntToPacket(where.get(i).getField2().getSensor());
            packet.addIntToPacket(where.get(i).getField2().getMetric());
        }
        


        return packet;
        
    }
}

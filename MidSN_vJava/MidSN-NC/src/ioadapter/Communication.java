/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package ioadapter;

import java.util.ArrayList;
import midsn.nc.Definitions;
import midsn.nc.MidSNNC;
import nc_ginapp_aa.Stream;
import nc_ginapp_cm.ConfigManager;
import nc_ginapp_dc.DataCollector;
import nc_ginapp_gp.StreamProcessors;
import nc_kernel_am.AgentManager;

/**
 *
 * @author cecilio
 */
public class Communication {

    MidSNNC midsn;

    public Communication(MidSNNC midsn) {
        this.midsn = midsn;
    }

    public Communication() {
        this.midsn = new MidSNNC();
    }

    public void comm_send_values(Stream streamp, ArrayList valuesToSendArrayStructPointer) {
        
        MidSN_packet packet = new MidSN_packet();
        packet.createPacket();

        packet.addAddressToPacket(streamp.getIpTo());
        packet.addAddressToPacket(streamp.getIpTo());
        packet.addIntToPacket(streamp.getIpPort());
        packet.addIntToPacket(Definitions.MSG_DATA);
        packet.addIntToPacket(0x0000);
        packet.addLongToPacket(System.currentTimeMillis());
        packet.addIntToPacket(streamp.getnSeq());
        packet.addIntToPacket(valuesToSendArrayStructPointer.size()*2 + 8);
        packet.addIntToPacket(streamp.getStreamID());
        packet.addIntToPacket(streamp.getnSeq());
        packet.addIntToPacket(0);
        packet.addIntToPacket(0);

        for (int i = 0; i < valuesToSendArrayStructPointer.size(); i++) {
            packet.addIntToPacket(Integer.parseInt(valuesToSendArrayStructPointer.get(i).toString()));
        }

        packet.parsePacket();
        packet.printPacketDecimal();

        packet.send_pkt(streamp);


        streamp.setnSeq(streamp.getnSeq() + 1);
        
        midsn.addStreamToDataCollector(streamp.getStreamID(), streamp);

    }

    public void comm_receive_handler(MidSN_packet pkt) {

        if ((pkt.getMsgType()) == Definitions.MSG_CONTROL) {
            System.out.println("MSG_CONTROL RECEIVED!");
            AgentManager manager = new AgentManager();
            manager.cm_node(pkt);
        }

        if (pkt.getMsgType() == Definitions.MSG_AGENT) {
            System.out.println("MSG_AGENT RECEIVED!");
            ConfigManager config = new ConfigManager(midsn);
            config.cm_ginapp(pkt);
        }

        if (pkt.getMsgType() == Definitions.MSG_OTHER) {
            System.out.println("MSG OTHER RECEIVED!");
        }

        if (pkt.getMsgType() == Definitions.MSG_DATA) {
            //System.out.println("MSG DATA RECEIVED!");
            
            
            Stream stream = midsn.getStreamFromoDataCollector(pkt.getDataAtPosition(0));
            
            if (stream == null) return;
            
            ArrayList tuples = stream.getDataValues();
            tuples.add(pkt);
            
            midsn.addStreamToDataCollector(stream.getStreamID(), stream);
            
            
            
            
//            pkt.printPacketDecimal();
        }

    }
}

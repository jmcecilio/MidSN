/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package ioadapter;

import drivers.CommunicationDriver;
import midsn.nc.Definitions;
import nc_ginapp_aa.Stream;

/**
 *
 * @author cecilio
 */
public class MidSN_packet {

    byte[] pkt;
    private int position = 0;
    private int SRC_LENGTH_OFFSET = 0;
    private int SRC_ADDRESS_OFFSET = 2;
    private byte[] srcAddr;
    private byte[] destAddr;
    private int[] data;
    private int nSeq;
    private long timestamp;
    private int msgCtrl;
    private int msgType;
    private int dataSize;
    private int dataOffset;
    private int msgPort;

    public void createPacket() {
        pkt = new byte[256];
    }

    public void createPacket(byte[] data) {
        pkt = data;
    }

    public void addIntToPacket(int value) {
        Utils.addInt(value, pkt, getPosition());
        setPosition(getPosition() + 2);

    }

    public void addLongToPacket(long value) {
        Utils.addLong(value, pkt, getPosition());
        setPosition(getPosition() + 8);
    }
    

    public void addBytesToPacket(byte[] value) {
        System.arraycopy(value, 0, pkt, getPosition(), value.length);
        setPosition(getPosition() + value.length);
    }
    
    
    

    public void printPacket() {
        printPacketHex();
        printPacketDecimal();
    }

    public void printPacketHex() {
        System.out.print("packet print hex: ");
        for (int j = 0; j < pkt.length; j++) {
            System.out.print(pkt[j] + " ");
        }
        System.out.println("");
    }
    
    
    public void parsePacket(){
        int srcLength = Utils.getInt(pkt, SRC_LENGTH_OFFSET);
        srcAddr = new byte[srcLength];
        System.arraycopy(pkt, SRC_ADDRESS_OFFSET, srcAddr, 0, srcLength);
        
        int destLength = Utils.getInt(pkt, srcLength + SRC_ADDRESS_OFFSET);
        destAddr = new byte[destLength];
        System.arraycopy(pkt, srcLength + SRC_ADDRESS_OFFSET + 2, destAddr, 0, destLength);
        
        int offset = srcLength + SRC_ADDRESS_OFFSET + destLength + 2;
        
        msgPort =  Utils.getInt(pkt, offset + 0);
        
        offset = offset + 2;
        
        msgType =  Utils.getInt(pkt, offset + 0);
        msgCtrl = Utils.getInt(pkt, offset + 2);
        timestamp = Utils.getLong(pkt, offset + 4);
        offset = offset + 12;
        nSeq = Utils.getInt(pkt, offset);
        dataSize = Utils.getInt(pkt, offset + 2)/2;
        dataOffset = offset + 4;
        data = new int [dataSize];        
        if (msgType == Definitions.MSG_DATA) {

            for (int i = 0; i < Utils.getInt(pkt, offset + 2)/2; i++) {
                data[i] = Utils.getInt(pkt, offset + 4 + i * 2);
            }
        }
    }

    public void printPacketDecimal() {  
        parsePacket();
        System.out.println("SRC: " + new String(this.srcAddr));     
        System.out.println("DEST: " + new String(this.destAddr));
        System.out.println("DEST PORT: " + this.msgPort);
        System.out.println("MSG_TYPE: " + this.msgType);
        System.out.println("MSG_CTRL: " + this.msgCtrl);
        System.out.println("TIMESTAMP: " + this.timestamp);
        System.out.println("nSEQ: " + this.nSeq);
        System.out.print("DATA: ");

        for (int i = 0; i < data.length; i++) {
            System.out.print(data[i] + ", ");
        }
        System.out.println("");
    }

    public int getMsgType() {
        return msgType;
    }

    public int getMsgCtrl() {
        return msgCtrl;
    }

    public long getMsgTimestamp() {
        return timestamp;
    }

    public int getMsgNSeq() {
        return nSeq;
    }

    public int getMsgDataSize() {
        return dataSize;
    }
    
    public int [] getMsgData() {
        return data;
    }
    
    public int getDataAtPosition(int pos) {
        return data[pos];
    }

    public void send_pkt(Stream streamp) {
        CommunicationDriver comm = new CommunicationDriver(streamp.getIpPort());
        comm.open_connection(streamp.getIpTo(), streamp.getIpPort());
        comm.send_to(streamp.getIpTo(), pkt, getPosition());
        comm.close_connection();
    }

    public byte[] getBytes() {
        
        byte[] pktShort = new byte[position];
        System.arraycopy(pkt, 0, pktShort, 0, position);

//        return pktShort;
        return pkt;
    }


    public void addAddressToPacket(String ip) {
        byte[] sBytes = ip.getBytes();
        addIntToPacket(sBytes.length);
        addBytesToPacket(sBytes);
    }

    /**
     * @return the position
     */
    public int getPosition() {
        return position;
    }

    /**
     * @param position the position to set
     */
    public void setPosition(int position) {
        this.position = position;
    }

    /**
     * @return the dataOffset
     */
    public int getDataOffset() {
        return dataOffset;
    }



    /**
     * @return the msgPort
     */
    public int getMsgPort() {
        return msgPort;
    }
    
    public String getMsgDestination() {
        return new String(this.destAddr);
    }
    
    public String getSrcAddr(){
        return new String(this.srcAddr);
    }
        
}

/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package tests;

import java.util.ArrayList;
import java.util.logging.Level;
import java.util.logging.Logger;
import midsn.nc.Definitions;
import nc_ginapp_aa.Field;
import nc_ginapp_aa.Stream;
import nc_ginapp_aa.Where;
import nc_ginapp_dc.DataCollector;

/**
 *
 * @author cecilio
 */
public class MidSNMainTesterWriteFile {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        // TODO code application logic here

        MidSNMainTesterWriteFile node = new MidSNMainTesterWriteFile();
        node.run();
    }

    public void run() {


        Stream stream = createStream();
        DataCollector data = new DataCollector();
        data.addStreamToFile(stream.getStreamID(), stream);



        for (int i = 0; i < 1001; i++) {
            

            String tuple = "<tuple> ";
            for (int j = 0; j < 50; j++) {
                tuple += i + "; ";
            }
            tuple += "</tuple>\n";

            data.addTupleToStreaminFile(stream.getStreamID(), tuple);
            

        }
        
        ArrayList res = null;
        
        for (int i = 0; i < 1000; i++) {

            long initInstante = System.nanoTime();
            res = data.readTuplesFromStreaminFile(stream.getStreamID(), 1000);
            long fim = System.nanoTime();

            System.out.println("Time to read 1 tuple from stream: " + (fim - initInstante) * 1e-6);

        }
        
        
        
//        for (int i = 0; i < res.size(); i++) {
//            System.out.println("tuple: " + res.get(i));
//        }




//            data.removeStreamFromFile(stream.getStreamID());
//            try {
//                Thread.sleep(100);
//            } catch (InterruptedException ex) {
//                Logger.getLogger(MidSNMainTesterWriteFile.class.getName()).log(Level.SEVERE, null, ex);
//            }


    }

    public Stream createStream() {

        Stream stream = new Stream();
        stream.setStreamID(1);
        stream.setIsIp(false);
        stream.setIpTo("localhost");
        stream.setIpPort(9000);
//        stream.setIpPort(4444);
        stream.setIsAlarm(false);
        stream.setRimeTo(0x0001);
        stream.setSend_rate(100);
        stream.setSense_rate(10);
        stream.setWindow_size(1000);
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

        ArrayList<Field> select = new ArrayList<Field>();

//                select.add(new Field(Definitions.SENSOR_TEMPERATURE, Definitions.FFIELD_VALUE));
        select.add(new Field(Definitions.SENSOR_TEMPERATURE, Definitions.FFIELD_AVG));
//                select.add(new Field(Definitions.SENSOR_TEMPERATURE, Definitions.FFIELD_MAX));
//                select.add(new Field(Definitions.SENSOR_TEMPERATURE, Definitions.FFIELD_MIN));
//                select.add(new Field(Definitions.SENSOR_TEMPERATURE, Definitions.FFIELD_COUNT));
//                select.add(new Field(Definitions.SENSOR_TEMPERATURE, Definitions.FFIELD_SUM));
//                select.add(new Field(Definitions.SENSOR_TEMPERATURE, Definitions.FFIELD_VARIANCE));
//                select.add(new Field(Definitions.SENSOR_TEMPERATURE, 90));

//        select.add(new Field(Definitions.FFIELD_sensor_LASTSENDINTERVAL, Definitions.NONE));
//        select.add(new Field(Definitions.SENSOR_TEMPERATURE, Definitions.FFIELD_VALUE));
//        select.add(new Field(Definitions.FFIELD_sensor_END, Definitions.FFIELD_VALUE));
        stream.setSelect(select);

        ArrayList<Where> where = new ArrayList<Where>();
        where.add(
                new Where(
                new Field(Definitions.FFIELD_sensor_END, Definitions.NONE),
                Definitions.NONE,
                new Field(Definitions.FFIELD_sensor_END, Definitions.NONE)));
        stream.setWhere(where);


        stream.setDataValues(new ArrayList());


        return stream;
    }
}

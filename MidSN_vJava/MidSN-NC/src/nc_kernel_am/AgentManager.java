/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package nc_kernel_am;

import ioadapter.MidSN_packet;
import java.io.File;
import java.net.MalformedURLException;
import java.net.URL;
import java.net.URLClassLoader;
import midsn.nc.Definitions;

/**
 *
 * @author cecilio
 */
public class AgentManager {


    public void cm_node(MidSN_packet pkt) {

        if (pkt.getMsgType() == Definitions.MSG_CONTROL) {

            switch (pkt.getMsgCtrl()) {
                case Definitions.CTRL_NODE_START_AGENT:

                    break;

                case Definitions.CTRL_NODE_STOP_AGENT:

                    break;

                case Definitions.CTRL_NODE_LOAD_AGENT:

                    break;

                case Definitions.CTRL_NODE_UNLOAD_AGENT:

                    break;

                case Definitions.CTRL_NODE_DELETE_AGENT:

                    break;

                case Definitions.CTRL_NODE_NEW_AGENT:

                    // Create a File object on the root of the directory containing the class file
                    File file = new File("c:\\myclasses\\");

                    try {
                        // Convert File to a URL
                        URL url = file.toURL();          // file:/c:/myclasses/
                        URL[] urls = new URL[]{url};

                        // Create a new class loader with the directory
                        ClassLoader cl = new URLClassLoader(urls);

                        // Load in the class; MyClass.class should be located in
                        // the directory file:/c:/myclasses/com/mycompany
                        Class cls = cl.loadClass("com.mycompany.MyClass");
                    } catch (MalformedURLException e) {
                    } catch (ClassNotFoundException e) {
                    }



                    break;

                case Definitions.CTRL_NODE_PING:

                    break;

                case Definitions.CTRL_NODE_REBOOT:

                    break;

            }
        } 
    }
}

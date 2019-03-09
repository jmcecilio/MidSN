/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package drivers;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.logging.Level;
import java.util.logging.Logger;
import midsn.nc.MidSNNC;

/**
 *
 * @author cecilio
 */
public class Timer implements Runnable{

    String timerType;
    int timerID;
    long timeInterval;
    private List _listeners = new ArrayList();
    private boolean run = true;
    
    public Timer(String timerType, int timerID, TimerListener listener, long period) {
        this.timerType = timerType;
        this.timerID = timerID;
        this.timeInterval = period;
        _listeners.add(listener);
    } 

    public Timer() {
        this.timerType = "test";
        this.timerID = 1;
        this.timeInterval = 10000;
        _listeners.add(new TimerListener() {

            @Override
            public void handleMyTimerEvent(TimerEvent event, String timerType, int timerID) {
                throw new UnsupportedOperationException("Not supported yet.");
            }
        }
                );
    }
    
    

  
  public synchronized void addEventListener(TimerListener listener)	{
    _listeners.add(listener);
  }
  public synchronized void removeEventListener(TimerListener listener)	{
    _listeners.remove(listener);
  }

  // call this method whenever you want to notify
  //the event listeners of the particular event
  public synchronized void fireEvent()	{
    TimerEvent event = new TimerEvent(this);
    Iterator i = _listeners.iterator();
    while(i.hasNext())	{
      ((TimerListener) i.next()).handleMyTimerEvent(event, timerType, timerID);
    }
  }

    @Override
    public void run() {
        while (run) {
            try {
                Thread.sleep(timeInterval);
                fireEvent();

            } catch (InterruptedException ex) {
                Logger.getLogger(MidSNNC.class.getName()).log(Level.SEVERE, null, ex);
            }
        }
    }

    /**
     * @param run the run to set
     */
    public void setRun(boolean run) {
        this.run = run;
    }

}


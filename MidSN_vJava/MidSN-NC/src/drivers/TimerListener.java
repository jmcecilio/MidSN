/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package drivers;

/**
 *
 * @author cecilio
 */
public interface TimerListener {

    public void handleMyTimerEvent(TimerEvent event, String timerType, int timerID);
}

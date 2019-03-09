/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package tests;

/**
 *
 * @author cecilio
 */
public class GetMemoryUsage {

    private long init;
    private long end;

    public static void main(String[] args) throws Exception {

        long heap1 = memoryUsed();
        long array[] = new long[1];

        for (int i = 0; i < array.length; i++) {
            array[i] = i;
        }

        long heap2 = memoryUsed();
        System.out.println("'before' heap: " + heap1
                + ", 'after' heap: " + heap2);
        System.out.println("heap delta: " + (heap2 - heap1));

        array[array.length - 1] = -1;

    }
    
    public void printUsageMemory(){
//        System.out.println("'before' heap: " + init
//                + ", 'after' heap: " + end);
        System.out.println("delta heap: " + (end - init));

    }

    public void setInitMark() {
        init = memoryUsed();
    }

    public void setEndMark() {
        end = memoryUsed();
    }

    static long memoryUsed() {
        return runtime.totalMemory() - runtime.freeMemory();
    }
    static final Runtime runtime = Runtime.getRuntime();
}
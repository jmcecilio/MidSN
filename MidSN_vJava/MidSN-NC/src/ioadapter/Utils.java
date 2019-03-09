/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package ioadapter;

/**
 *
 * @author cecilio
 */
/*
 * Copyright Aduna (http://www.aduna-software.com/) (c) 1997-2006.
 *
 * Licensed under the Aduna BSD-style license.
 */


public class Utils {

    
  public static void addInt(int value, byte[] array, int offset) {
//    array[offset]   = (byte)(0xff & (value >> 24));
//    array[offset+1] = (byte)(0xff & (value >> 16));
    array[offset] = (byte)(0xff & (value >> 8));
    array[offset+1] = (byte)(0xff & value);
  }

  public static int getInt(byte[] array, int offset) {
    return
//      ((array[offset]   & 0xff) << 24) |
//      ((array[offset+1] & 0xff) << 16) |
      ((array[offset] & 0xff) << 8) |
       (array[offset+1] & 0xff);
  }

  public static void addLong(long value, byte[] array, int offset) {
    array[offset]   = (byte)(0xff & (value >> 56));
    array[offset+1] = (byte)(0xff & (value >> 48));
    array[offset+2] = (byte)(0xff & (value >> 40));
    array[offset+3] = (byte)(0xff & (value >> 32));
    array[offset+4] = (byte)(0xff & (value >> 24));
    array[offset+5] = (byte)(0xff & (value >> 16));
    array[offset+6] = (byte)(0xff & (value >> 8));
    array[offset+7] = (byte)(0xff & value);
  }
  

  public static long getLong(byte[] array, int offset) {
    return
      ((long)(array[offset]   & 0xff) << 56) |
      ((long)(array[offset+1] & 0xff) << 48) |
      ((long)(array[offset+2] & 0xff) << 40) |
      ((long)(array[offset+3] & 0xff) << 32) |
      ((long)(array[offset+4] & 0xff) << 24) |
      ((long)(array[offset+5] & 0xff) << 16) |
      ((long)(array[offset+6] & 0xff) << 8) |
      ((long)(array[offset+7] & 0xff));
  }
  
  public static String getString(byte[] array, int offset, int size){

        byte [] addr = new byte[size];
        System.arraycopy(array, offset, addr, 0, size);
      
      return new String(addr);
  }


}

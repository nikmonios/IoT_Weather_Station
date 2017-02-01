// TCPClient.java
// A client program implementing TCP socket
import java.net.*; 
import java.io.*; 
import java.io.IOException;

public class TCPClient
 {
 
	public static void main (String args[]) 
	{
    // arguments supply message and hostname of destination  
		Socket s = null; 
		try
    { 
            int serverPort = 3300;     //select port
            String ip = "192.168.1.109"; //select IP

            s = new Socket(ip, serverPort); 
            DataInputStream input = new DataInputStream( s.getInputStream()); 

            //Step 1 read length
            int nb = input.readInt();
            byte[] digit = new byte[nb];
            //Step 2 read byte
            for(int i = 0; i < nb; i++)
              digit[i] = input.readByte();

            String st = new String(digit);
              System.out.println("Received: "+ st); 
		}
		catch (UnknownHostException e)
    { 
			  System.out.println("Sock:"+e.getMessage());
    }
		catch (EOFException e)
    {
			  System.out.println("EOF:"+e.getMessage()); 
    }
		catch (IOException e)
    {
			  System.out.println("IO:"+e.getMessage());
    } 
		finally 
    {
			 if(s!=null) 
			 try 
       {
           s.close();
			 } 
			 catch (IOException e) 
       {/*close failed*/}
    } //end main
  } //end class

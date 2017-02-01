// TCPClient.java
// A client program implementing TCP socket
import java.net.*; 
import java.io.*; 
import java.io.IOException;
//import java.io.BufferedReader;
//import java.io.FileReader;

public class TCPClient
 {
     
	 public static String readFile() 
     {
		BufferedReader br = null;
		String data="";

		try {
			String sCurrentLine;
			br = new BufferedReader(new FileReader("measurements.txt"));

			while ((sCurrentLine = br.readLine()) != null) {
				//System.out.println(sCurrentLine);
				data += sCurrentLine+"\n";
			}

		} catch (IOException e) {
			e.printStackTrace();
		} finally {
			try {
				if (br != null)br.close();
			} catch (IOException ex) {
				ex.printStackTrace();
			}
		}
		return data;

	}

/*	public static String cleanData(String str){
		String data="";
		String[] tokens = str.split("SPLIT");
		for(String t: tokens){
			if(t.contains("MEASUREMENTS")){
				t=t.replace("MEASUREMENTS","\n\n");
				System.out.println(t);
				data+=t;
			}
		}
		//System.out.println(data);
		return data; 
	}
*/
	public static void main (String args[]) 
	{// arguments supply message and hostname of destination  
		Socket s = null; 
		try
        { 
            int serverPort = 3300;     //select port
			String ip = "192.168.1.109"; //select IP
			String data = "Hello, How are you?";
            //System.out.println("pro-socket");
			s = new Socket(ip, serverPort); 
			DataInputStream input = new DataInputStream( s.getInputStream()); 
			DataOutputStream output = new DataOutputStream( s.getOutputStream()); 
            //System.out.println("after-data");
			//Step 1 send length
			System.out.println("Length"+ data.length());
			output.writeInt(data.length());
			//Step 2 send length
			System.out.println("Writing.......");
			output.writeBytes(data); // UTF is a string encoding
			  
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
        }
    }
  
}



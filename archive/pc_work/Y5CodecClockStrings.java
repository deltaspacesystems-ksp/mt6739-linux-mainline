import ghidra.app.script.GhidraScript;
import java.io.*;
public class Y5CodecClockStrings extends GhidraScript {
 public void run() throws Exception {
  try(PrintWriter out=new PrintWriter(new FileWriter(getScriptArgs()[0]))) {
   for(String a:new String[]{"ffffff8008e1adcc","ffffff8008e1ae0c","ffffff8008e1ae4c","ffffff8008e1ae94"}) {
    StringBuilder s=new StringBuilder();
    for(int i=0;i<80;i++){int c=currentProgram.getMemory().getByte(toAddr(a).add(i))&255;if(c==0)break;if(c<32||c>126)throw new IOException("Non ASCII");s.append((char)c);}
    out.println(a+" "+s);
   }
  }
 }
}

import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import java.io.PrintWriter;
public class Y5ScanoutGpuTables extends GhidraScript {
 private String cstring(long value) throws Exception {
  StringBuilder s=new StringBuilder();Address a=toAddr(value);
  for(int i=0;i<96;i++){int c=currentProgram.getMemory().getByte(a.add(i))&255;if(c==0)break;if(c<32||c>126)return "<non-ascii>";s.append((char)c);}return s.toString();
 }
 public void run() throws Exception {
  try(PrintWriter out=new PrintWriter(getScriptArgs()[0])){
   Address map=toAddr("ffffff8008c00e80");
   for(int i=0;i<3;i++)out.printf("DISPLAY_PORT_MAP index=%d module=%d port=%d%n",i,getInt(map.add(i*8)),getInt(map.add(i*8+4)));
   Address ports=toAddr("ffffff800944a230");
   for(int i=0;i<3;i++){Address a=ports.add(i*48);long ptr=getLong(a);out.printf("PORT_TABLE index=%d name=%s packed=%02x larb_port=%d%n",i,cstring(ptr),getByte(a.add(8))&255,getByte(a.add(9))&255);}
   out.println("GPU_FIRMWARE_FALLBACK="+cstring(0xffffff8008e69b38L));
  }
 }
}

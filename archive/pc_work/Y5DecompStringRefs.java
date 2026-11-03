import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.symbol.Reference;
import java.nio.file.*;
import java.nio.charset.StandardCharsets;
import java.util.*;
public class Y5DecompStringRefs extends GhidraScript {
 public void run() throws Exception {
  String[] args=getScriptArgs();StringBuilder out=new StringBuilder();Set<Address> seen=new HashSet<>();
  DecompInterface di=new DecompInterface();di.openProgram(currentProgram);
  try {
   for(String text:Files.readAllLines(Paths.get(args[1]),StandardCharsets.UTF_8)){
    if(text.isEmpty())continue;byte[] bytes=text.getBytes(StandardCharsets.US_ASCII);Address start=currentProgram.getMinAddress();int matches=0;
    while(start!=null&&matches++<10){Address at=currentProgram.getMemory().findBytes(start,bytes,null,true,monitor);if(at==null)break;
     out.append("// STRING ").append(text).append(" @").append(at).append("\n");
     for(Reference ref:currentProgram.getReferenceManager().getReferencesTo(at)){
      Function f=getFunctionContaining(ref.getFromAddress());if(f==null||!seen.add(f.getEntryPoint()))continue;
      out.append("// FUNCTION ").append(f.getName()).append(" @").append(f.getEntryPoint()).append("\n");
      DecompileResults result=di.decompileFunction(f,30,monitor);
      if(result.decompileCompleted())out.append(result.getDecompiledFunction().getC()).append("\n");else out.append("// DECOMPILE FAILED\n");
     }
     start=at.next();
    }
   }
  } finally {di.dispose();Files.write(Paths.get(args[0]),out.toString().getBytes(StandardCharsets.UTF_8));}
 }
}

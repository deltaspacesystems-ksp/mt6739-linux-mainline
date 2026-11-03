import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.*;
import ghidra.program.model.symbol.Reference;
import java.nio.file.*;
import java.nio.charset.StandardCharsets;
import java.util.*;
public class Y5DecompCallers extends GhidraScript {
 public void run() throws Exception {
  String[] args=getScriptArgs();Set<String> names=new HashSet<>(Files.readAllLines(Paths.get(args[1]),StandardCharsets.UTF_8));
  Set<Address> callers=new LinkedHashSet<>();StringBuilder out=new StringBuilder();
  FunctionIterator it=currentProgram.getFunctionManager().getFunctions(true);
  while(it.hasNext()){Function target=it.next();if(!names.contains(target.getName()))continue;out.append("// TARGET ").append(target.getName()).append(" @").append(target.getEntryPoint()).append("\n");
   for(Reference ref:currentProgram.getReferenceManager().getReferencesTo(target.getEntryPoint())){Function f=getFunctionContaining(ref.getFromAddress());if(f!=null)callers.add(f.getEntryPoint());}
  }
  DecompInterface di=new DecompInterface();di.openProgram(currentProgram);
  try{for(Address at:callers){if(monitor.isCancelled())break;Function f=getFunctionAt(at);out.append("// CALLER ").append(f.getName()).append(" @").append(at).append("\n");DecompileResults result=di.decompileFunction(f,45,monitor);if(result.decompileCompleted())out.append(result.getDecompiledFunction().getC()).append("\n");}}
  finally{di.dispose();Files.write(Paths.get(args[0]),out.toString().getBytes(StandardCharsets.UTF_8));}
 }
}

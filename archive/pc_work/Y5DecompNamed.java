import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.listing.*;
import java.io.*;
import java.util.*;
public class Y5DecompNamed extends GhidraScript {
 public void run() throws Exception {
  String[] a=getScriptArgs();
  Map<String,Function> functions=new HashMap<>();
  FunctionIterator it=currentProgram.getFunctionManager().getFunctions(true);
  while(it.hasNext()){Function f=it.next();functions.put(f.getName(),f);}
  DecompInterface di=new DecompInterface();di.openProgram(currentProgram);
  try(PrintWriter out=new PrintWriter(new FileWriter(a[0]));BufferedReader targets=new BufferedReader(new FileReader(a[1]))){
   String line;
   while((line=targets.readLine())!=null){
    String name=line.trim();if(name.isEmpty())continue;
    Function f=functions.get(name);out.println("//// "+name+(f==null?" NOT FOUND":" @"+f.getEntryPoint()));
    if(f!=null){DecompileResults result=di.decompileFunction(f,90,monitor);if(result.getDecompiledFunction()!=null)out.println(result.getDecompiledFunction().getC());else out.println("// failed: "+result.getErrorMessage());}
   }
  }finally{di.dispose();}
 }
}

import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.address.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.symbol.*;
import java.io.*;
import java.util.*;

public class Y5Decomp extends GhidraScript {
	public void run() throws Exception {
		String[] a = getScriptArgs();   // ksyms, out, targets(comma)
		AddressFactory af = currentProgram.getAddressFactory();
		SymbolTable st = currentProgram.getSymbolTable();
		Map<String, Address> sym = new LinkedHashMap<>();
		List<Address> funcs = new ArrayList<>();
		try (BufferedReader r = new BufferedReader(new FileReader(a[0]))) {
			String l;
			while ((l = r.readLine()) != null) {
				String[] p = l.split("\\s+");
				if (p.length < 3) continue;
				Address ad = af.getDefaultAddressSpace().getAddress(Long.parseUnsignedLong(p[0], 16));
				if (!currentProgram.getMemory().contains(ad)) continue;
				if (!sym.containsKey(p[2])) sym.put(p[2], ad);
				try { st.createLabel(ad, p[2], SourceType.USER_DEFINED); } catch (Exception e) {}
				if (p[1].equals("t") || p[1].equals("T")) funcs.add(ad);
			}
		}
		println("symbols " + sym.size() + " funcs " + funcs.size());
		int n = 0;
		FunctionManager fm = currentProgram.getFunctionManager();
		for (Address ad : funcs) {
			if (fm.getFunctionAt(ad) != null) continue;
			try { disassemble(ad); createFunction(ad, null); n++; } catch (Exception e) {}
		}
		println("created " + n);
		DecompInterface di = new DecompInterface();
		di.openProgram(currentProgram);
		try (PrintWriter w = new PrintWriter(new FileWriter(a[1]))) {
			List<String> tl = new ArrayList<>();
			if (a[2].endsWith(".txt")) { try (BufferedReader tr = new BufferedReader(new FileReader(a[2]))) { String x; while ((x = tr.readLine()) != null) if (!x.trim().isEmpty()) tl.add(x.trim()); } } else tl.addAll(Arrays.asList(a[2].split(",")));
			for (String t : tl) {
				Address ad = sym.get(t);
				if (ad == null) { w.println("//// " + t + ": brak symbolu"); continue; }
				Function f = fm.getFunctionAt(ad);
				if (f == null) { w.println("//// " + t + ": brak funkcji @" + ad); continue; }
				DecompileResults res = di.decompileFunction(f, 120, monitor);
				w.println("//// " + t + " @" + ad);
				if (res.getDecompiledFunction() != null) w.println(res.getDecompiledFunction().getC());
				else w.println("// decompile failed: " + res.getErrorMessage());
			}
		}
	}
}

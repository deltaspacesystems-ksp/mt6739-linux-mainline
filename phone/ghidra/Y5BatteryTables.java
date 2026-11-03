import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import java.io.PrintWriter;

/* Fixed stock-kernel tables only; no device partitions or runtime secrets. */
public class Y5BatteryTables extends GhidraScript {
    public void run() throws Exception {
        try (PrintWriter out = new PrintWriter(getScriptArgs()[0])) {
            Address table = toAddr("ffffff80094b57f8");
            out.println("# stock_Image thermistor: temperature_C resistance_ohm");
            for (int i = 0; i < 21; i++) {
                out.printf("%d %d%n", getInt(table.add(i * 8)), getInt(table.add(i * 8 + 4)));
            }
            out.println("# ADC descriptor: index bits ratio channel u8 req_id ready_id out_id");
            Address adc = toAddr("ffffff8009451b18");
            for (int i = 0; i < 13; i++) {
                Address a = adc.add(i * 16);
                out.printf("%d %d %d %d %d 0x%x 0x%x 0x%x%n", i,
                    getByte(a) & 255, getByte(a.add(1)) & 255,
                    getByte(a.add(2)) & 255, getByte(a.add(3)) & 255,
                    getInt(a.add(4)), getInt(a.add(8)), getInt(a.add(12)));
            }
            out.println("# PMIC field descriptor: id reg mask shift");
            Address fields = toAddr("ffffff8008bd8390");
            for (int id : new int[]{0x6aa,0x6ad,0x63a,0x639,0x4c1}) {
                Address a = fields.add(id * 12);
                out.printf("0x%x 0x%x 0x%x %d%n", getInt(a),
                    getShort(a.add(4)) & 65535, getShort(a.add(6)) & 65535,
                    getByte(a.add(8)) & 255);
            }
            out.println("# BIF weak implementation instructions");
            Address bif = toAddr("ffffff8008512258");
            disassemble(bif);
            for (int off = 0; off < 44; off += 4) {
                out.println(currentProgram.getListing().getInstructionAt(bif.add(off)));
            }
            out.println("# charger model names selected by stock detect (ID41, ID51 subvariants)");
            for (String addr : new String[]{"ffffff80094b6558", "ffffff80094b6590", "ffffff80094b65c8",
                    "ffffff8008dfa463", "ffffff8008dfb889", "ffffff8008dfb89e"}) {
                byte[] bytes = getBytes(toAddr(addr), 48);
                int length = 0;
                while (length < bytes.length && bytes[length] != 0) length++;
                out.println(addr + " " + new String(bytes, 0, length, java.nio.charset.StandardCharsets.UTF_8));
            }
        }
    }
}

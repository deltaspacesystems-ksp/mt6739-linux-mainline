from pathlib import Path
import json
base = Path('D:/y5_linux_work')
handles = json.loads((base / 'out/v59-dt-handles.json').read_text())
extra = '''
/ {
 mfw-gpu-id-probe@10006000 { status = "disabled"; };
 gpu@13000000 {
  compatible = "microfw,mt6739-rogue-ddk19";
  reg = <0 0x13000000 0 0x80000>, <0 0x10006000 0 0x1000>,
        <0 0x10001000 0 0x1000>, <0 0x1020e000 0 0x1000>;
  reg-names = "gpu", "spm", "infra-ao", "infra";
  interrupt-parent = <1>;
  interrupts = <0 226 8>;
  interrupt-names = "RGX";
  clocks = <CLOCK>;
  vcore-supply = <VCORE>;
  microfw,pmic-provider = <PMIC>;
 };
};
'''.replace('CLOCK', str(handles['clock'])).replace('VCORE', str(handles['vcore'])).replace('PMIC', str(handles['pmic']))
(base / 'out/v19.dts').write_text((base / 'out/v18.dts').read_text() + extra, newline='\n')
(base / 'microfw-gpu/gpu-native-v60.dtsi').write_text('// Stock GPU reg/IRQ provenance: phone/hardware/gpu-stock-evidence.json\n' + extra, newline='\n')
print('DT19 source prepared: native driver default-disabled, old ID probe disabled, stock SPI226 low')

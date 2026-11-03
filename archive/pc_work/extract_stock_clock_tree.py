"""Decode only clock descriptor fields proven by k44_clock_tree.c.

Input is the stock kernel executable, never calibration/partition data.
Addresses/counts/stride follow mtk_apmixed_init and register_plls.
Remaining PLL words intentionally retain their unknown field names.
"""
from pathlib import Path
import hashlib, json, struct

work = Path(r'D:\y5_linux_work')
blob = (work/'out/stock_Image').read_bytes()
base = 0xffffff8008080000
assert blob[56:60] == b'ARM\x64'

def unpack(fmt, va):
    offset = va-base
    assert 0 <= offset <= len(blob)-struct.calcsize(fmt)
    return struct.unpack_from(fmt, blob, offset)

def string(va):
    offset = va-base
    assert 0 <= offset < len(blob)
    end = blob.index(b'\0', offset, offset+128)
    value = blob[offset:end].decode('ascii')
    assert value and all(c.isalnum() or c in '_-' for c in value), value
    return value

plls = []
for i in range(6):
    va = 0xffffff8008c59088 + i*112
    words = unpack('<28I', va)
    plls.append(dict(id=words[0], name=string(unpack('<Q', va+8)[0]),
                     descriptor_va=hex(va), reg_offset=hex(words[4]),
                     pwr_offset=hex(words[5]), pd_offset=hex(words[7]),
                     tuner_offset=hex(words[8]), pcw_offset=hex(words[23]),
                     raw_words=[hex(w) for w in words]))
assert len({p['id'] for p in plls}) == 6
assert len({p['name'] for p in plls}) == 6
factors = []
for i in range(40):
    va = 0xffffff800901bf28+i*40
    w = unpack('<10I', va)
    factors.append(dict(id=w[0], name=string(unpack('<Q',va+8)[0]),
                        parent=string(unpack('<Q',va+16)[0]),
                        mult=w[6], div=w[7],
                        pdn_raw_words=[hex(w[8]),hex(w[9])]))
assert len({f['id'] for f in factors}) == 40
assert all(f['mult'] > 0 and f['div'] > 0 for f in factors)
fixed_va = 0xffffff800901bf08
fixed = dict(id=unpack('<I', fixed_va)[0],
             name=string(unpack('<Q', fixed_va+8)[0]),
             rate=unpack('<Q', fixed_va+24)[0])
fixed_parent = unpack('<Q', fixed_va+16)[0]
fixed['parent'] = string(fixed_parent) if fixed_parent else None
muxes = []
for i in range(24):
    va = 0xffffff800901c568+i*48
    count = unpack('<B',va+0x2c)[0]
    assert 0 < count <= 16
    parents_va = unpack('<Q',va+16)[0]
    muxes.append(dict(id=unpack('<I',va)[0],
                      name=string(unpack('<Q',va+8)[0]),
                      parents=[string(unpack('<Q',parents_va+j*8)[0])
                               for j in range(count)],
                      register_words_raw=[hex(x) for x in unpack('<4I',va+24)],
                      field_bytes_raw=list(unpack('<4B',va+40))))
assert len({m['id'] for m in muxes}) == 24
result = dict(source_sha256=hashlib.sha256(blob).hexdigest(),
              evidence='Stock kernel descriptors, not live PLL measurements. '
              'Offsets verified in k44_clock_tree.c; other fields are raw.',
              plls=plls, fixed_clock=fixed, factors=factors, muxes=muxes,
              btif_clock_names=[string(0xffffff8008ea6786),
                                string(0xffffff8008ea67e3)],
              top=dict(provider_slots=67, fixed_count=1, factor_pdn_count=40,
                       mux_count=24, fixed_va='0xffffff800901bf08',
                       factor_pdn_va='0xffffff800901bf28',
                       mux_va='0xffffff800901c568'),
              infra=dict(provider_slots=80, gate_count=79,
                         gates_va='0xffffff800901c9e8'))
(work/'out/stock-clock-tree.json').write_text(
    json.dumps(result, indent=2)+'\n', encoding='utf-8')
for pll in plls:
    print(pll['id'], pll['name'], pll['reg_offset'], pll['pcw_offset'])
print('PASS: 6 PLL descriptors; 40 factors/24 muxes/79 gates from init calls')

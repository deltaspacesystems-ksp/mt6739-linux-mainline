"""Translate the existing project's absolute M/L/Q/Z SVG paths to Cairo C.

Fail on unknown SVG features rather than silently changing icon geometry.
Generated code belongs in build/, originals remain the source of truth.
"""
from pathlib import Path
import re
import sys
import xml.etree.ElementTree as ET

root = Path(__file__).resolve().parents[1]
names = ('wifi', 'cellular', 'gear', 'clock', 'camera', 'sun', 'torch')
out = ['/* Generated from the existing MICROFW SVG assets; do not edit. */',
       '#include "ui.h"', '#include <math.h>',
       'static void quad(cairo_t *c,double x,double y,double ex,double ey) {',
       ' double sx,sy; cairo_get_current_point(c,&sx,&sy);',
       ' cairo_curve_to(c,sx+(x-sx)*2/3,sy+(y-sy)*2/3,',
       ' ex+(x-ex)*2/3,ey+(y-ey)*2/3,ex,ey); }']
for name in names:
    svg = ET.parse(root / 'assets/icons' / (name + '.svg')).getroot()
    w, h = (float(svg.attrib[x].removesuffix('px')) for x in ('width', 'height'))
    group = next(x for x in svg if x.tag.endswith('g'))
    expected = f'scale(1,-1) translate(0,-{svg.attrib["height"].removesuffix("px")})'
    assert group.attrib['transform'] == expected, name
    out += [f'static void icon_{name}(cairo_t *c,double x,double y,double w,double h) {{',
            f' double s=fmin(w/{w},h/{h}); cairo_save(c);',
            f' cairo_translate(c,x+(w-{w}*s)/2,y+(h-{h}*s)/2);',
            f' cairo_scale(c,s,-s); cairo_translate(c,0,-{h}); cairo_new_path(c);']
    for path in group:
        assert path.tag.endswith('path') and path.attrib.get('stroke-width') == '1.0'
        tokens = re.findall(r'[A-Za-z]|-?(?:\d+(?:\.\d*)?|\.\d+)', path.attrib['d'])
        i = 0
        while i < len(tokens):
            cmd = tokens[i]; i += 1
            counts = {'M': 2, 'L': 2, 'Q': 4, 'Z': 0}
            assert cmd in counts, (name, cmd)
            args = tokens[i:i+counts[cmd]]; i += counts[cmd]
            fn = {'M':'cairo_move_to', 'L':'cairo_line_to', 'Q':'quad', 'Z':'cairo_close_path'}[cmd]
            out.append(f' {fn}(c' + (','+','.join(args) if args else '') + ');')
        out += [' cairo_set_line_width(c,1); cairo_fill_preserve(c); cairo_stroke(c);']
    out += [' cairo_restore(c); }']
out += ['void mfw_icon(cairo_t *c,enum mfw_icon icon,double x,double y,double w,double h) {',
        ' switch(icon) {']
for name in names:
    out += [f' case ICON_{name.upper()}: icon_{name}(c,x,y,w,h); break;']
out += [' } }']
Path(sys.argv[1]).write_text('\n'.join(out)+'\n', encoding='utf-8')

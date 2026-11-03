from pathlib import Path
r=Path('X:/y5linux/phone/ui');p=r/'src/motion.c';s=p.read_text();s=s.replace(' cairo_save(c);cairo_rectangle(c,shift,0,a->w,a->h);cairo_clip(c);',''' double radius=28*sin(3.141592653589793*p);
 cairo_save(c);cairo_new_sub_path(c);
 cairo_arc(c,shift+a->w-radius,radius,radius,-1.570796326794897,0);
 cairo_arc(c,shift+a->w-radius,a->h-radius,radius,0,1.570796326794897);
 cairo_arc(c,shift+radius,a->h-radius,radius,1.570796326794897,3.141592653589793);
 cairo_arc(c,shift+radius,radius,radius,3.141592653589793,4.71238898038469);
 cairo_close_path(c);cairo_clip(c);''');p.write_text(s,encoding='utf-8',newline='\n')
p=r/'Makefile';s=p.read_text(encoding='utf-8').replace('$(SRC) src/ui.h src/gestures.h','$(SRC) src/ui.h src/motion.h src/gestures.h');s+='''\n.PHONY: test-motion
build/test-motion: src/motion.c src/motion.h src/ui.h tools/test_motion.c | build
\t$(CC) $(CPPFLAGS) $(CFLAGS) -fsanitize=address,undefined src/motion.c tools/test_motion.c -o $@ $(LDLIBS)
test-motion: build/test-motion
\t./build/test-motion
''';p.write_text(s,encoding='utf-8',newline='\n')
for n in ['src/motion.c','Makefile']:Path('D:/y5_linux_work/microfw-ui',n).write_bytes((r/n).read_bytes())

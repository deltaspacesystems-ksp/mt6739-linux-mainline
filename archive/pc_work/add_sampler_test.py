from pathlib import Path
w=Path(r'D:\y5_linux_work\microfw-ui')
p=w/'Makefile';s=p.read_text();s+='''
.PHONY: test-sampler
build/test-sampler: tools/test_sampler.c src/sampler.c src/sampler.h src/ui.h | build
	$(CC) $(CPPFLAGS) $(CFLAGS) -fsanitize=undefined tools/test_sampler.c src/sampler.c -o $@ $(LDLIBS)
test-sampler: build/test-sampler
	./build/test-sampler
''';p.write_text(s,newline='\n')

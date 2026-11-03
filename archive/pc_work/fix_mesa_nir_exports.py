from pathlib import Path
p=Path('/home/y5/mesa-native/src/compiler/nir/nir_fixup_is_exported.c');s=p.read_text();old='if (_mesa_set_search(shadowed, func->name)) {';assert old in s;s=s.replace(old,'/* Newer Clang names the inner function __clang_ocl_kern_imp_*,\n       * so entrypoint wrappers need not have a shadowed name. */\n      if (func->is_entrypoint || _mesa_set_search(shadowed, func->name)) {');p.write_text(s)
fn=s[s.index('void\nnir_fixup_is_exported'):]
prefix='''#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
typedef struct {const char *name;bool is_entrypoint,is_exported;} nir_function;
typedef struct {nir_function funcs[8];unsigned count;} nir_shader;
#define nir_foreach_function(f,n) for(nir_function *f=(n)->funcs;f<(n)->funcs+(n)->count;f++)
struct set {const char *keys[16];unsigned count;};
static unsigned _mesa_hash_string(const void *s){(void)s;return 0;}
static bool _mesa_key_string_equal(const void *a,const void *b){return !strcmp(a,b);}
static struct set *_mesa_set_create(void *ctx,unsigned(*h)(const void*),bool(*e)(const void*,const void*)){(void)ctx;(void)h;(void)e;return calloc(1,sizeof(struct set));}
static void *_mesa_set_search(struct set *s,const char *key){for(unsigned i=0;i<s->count;i++)if(!strcmp(s->keys[i],key))return (void*)key;return NULL;}
static void _mesa_set_add(struct set *s,const char *key){assert(s->count<16);s->keys[s->count++]=key;}
static void _mesa_set_destroy(struct set *s,void *f){(void)f;free(s);}
'''
suffix='''
int main(void){
 nir_shader old={.funcs={{"kernel",true,false},{"kernel",false,true}},.count=2};nir_fixup_is_exported(&old);assert(old.funcs[0].is_exported&&!old.funcs[1].is_exported);
 nir_shader modern={.funcs={{"kernel",true,false},{"__clang_ocl_kern_imp_kernel",false,true}},.count=2};nir_fixup_is_exported(&modern);assert(modern.funcs[0].is_exported&&!modern.funcs[1].is_exported&&!modern.funcs[1].is_entrypoint);
 nir_shader helper={.funcs={{"helper",false,true}},.count=1};nir_fixup_is_exported(&helper);assert(helper.funcs[0].is_exported);
 nir_shader hidden={.funcs={{"_hidden",true,true}},.count=1};nir_fixup_is_exported(&hidden);assert(!hidden.funcs[0].is_exported&&!hidden.funcs[0].is_entrypoint);
 nir_shader duplicate={.funcs={{"helper",false,true},{"helper",false,true}},.count=2};nir_fixup_is_exported(&duplicate);assert(!duplicate.funcs[0].is_exported&&!duplicate.funcs[1].is_exported);
 nir_shader visible={.funcs={{"visible",true,true}},.count=1};nir_fixup_is_exported(&visible);assert(visible.funcs[0].is_exported&&visible.funcs[0].is_entrypoint);
 puts("PASS six exact NIR export-pass cases: old/new Clang wrappers, helper, private, duplicate, existing export");
}
'''
Path('/mnt/d/y5_linux_work/test_mesa_nir_exports.c').write_text(prefix+fn+suffix)

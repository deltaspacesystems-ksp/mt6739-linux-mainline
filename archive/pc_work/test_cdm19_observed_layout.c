#include <stdint.h>
#include <stddef.h>
/* Observed stock envelope layout, NOT a complete firmware ABI or dispatch packet.
 * Names deliberately avoid unproven register semantics; never submit this type. */
struct mfw_cdm19_observed_layout {
 uint64_t runtime_words[6];
 uint8_t unresolved_30[8];
 uint64_t context_words[7];
 uint32_t flags,deadline;
};
_Static_assert(sizeof(struct mfw_cdm19_observed_layout)==120,"observed packet extent");
_Static_assert(offsetof(struct mfw_cdm19_observed_layout,unresolved_30)==48,"unresolved gap");
_Static_assert(offsetof(struct mfw_cdm19_observed_layout,context_words)==56,"context copy");
_Static_assert(offsetof(struct mfw_cdm19_observed_layout,flags)==112,"flags");
_Static_assert(offsetof(struct mfw_cdm19_observed_layout,deadline)==116,"deadline");
int main(void){return 0;}

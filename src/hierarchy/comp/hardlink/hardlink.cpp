#include "hardlink.h"

#include "../../hierarchy.h"
#include "../comp.h"

void hardlink::nullify_link(const ctx_t<std::unique_ptr<comp>>& link_comp, bool update_reference) {
    hardlink* link_ptr = comp::get_ptr<hardlink>(link_comp.val.get());

    if (update_reference) { // remove the reference from the target node
        auto refs = hierarchy::read_refs(hierarchy::stat_refs_id(link_ptr->target));
        refs.val->hardlinks.erase({link_comp.hier, link_ptr->id});
        refs.hier->driver->write_refs(refs.val.get()); // write back to driver
    }

    // nullify the link ctx pointer
    link_ptr->target = null_ctx<node_id_t>(node_id_null);

    // write the link to driver
    link_comp.hier->driver->write_comp(link_comp.val.get());

}

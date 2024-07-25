#include "hierarchy.h"

#include <iostream>

ctx_t<node_id_t> hierarchy::mk_direct_node(ctx_t<node_id_t> ctx, const path_comp &p,
                                           const comp_data &comp_val,
                                           const content_data &content_val) noexcept {
    if (is_ctx_null(ctx) || !ctx.hier->driver->has_node(ctx.val)) return null_ctx<node_id_t>();

    // preevaluate current context
    ctx = evaluate_ctx(ctx, true, true, true);
    if (is_ctx_null(ctx)) return null_ctx<node_id_t>();

    auto node_comp = ctx_t(ctx.hier, ctx.hier->driver->read_comp(ctx.hier->driver->read_node(ctx.val)->comp_id));
    // auto node_comp = read_comp(ctx);
    switch (comp::get_type(*node_comp.val)) {
        case comp_type::dir: {
            ctx_t<node_id_t> child_ctx = dir::search_path_comp(node_comp, p);
            if (is_ctx_null(child_ctx)) {
                // need to allocte the node and attach it to the directory

                // allocate a new node
                child_ctx = {ctx.hier, ctx.hier->driver->allocte_node()};
                node child(child_ctx.val);
                child.name = p;

                // allocte comp
                comp_id_t comp_id = ctx.hier->driver->allocate_comp();
                comp _comp(comp_val);
                std::visit([&](auto &obj) { obj.id = comp_id; }, _comp);

                // allocate its content
                content_id_t content_id = ctx.hier->driver->allocate_content();
                content _content(content_val);
                std::visit([&](auto &obj) { obj.id = content_id; }, _content);

                // allocte empty refs
                refs_id_t refs_id = ctx.hier->driver->allocte_refs();
                refs refs(refs_id);

                // write the new node and all its data
                child.comp_id = comp_id;
                child.refs_id = refs_id;
                child.content_id = content_id;
                ctx.hier->driver->write_node(&child);
                ctx.hier->driver->write_comp(&_comp);
                ctx.hier->driver->write_refs(&refs);
                ctx.hier->driver->write_content(&_content);

                // add the new node under the current directory
                dir::add_child_to_dir(node_comp, child_ctx, true);
            }

            return {ctx.hier, child_ctx.val};
        }
        default: return null_ctx<node_id_t>();
    }
}

ctx_t<node_id_t> hierarchy::mk_path(ctx_t<node_id_t> ctx, const path &p,
                                          const std::vector<comp_data> &comp_vals,
                                          const std::vector<content_data> &content_vals) noexcept {
    if (p.empty()) return ctx;

    for (int i = 0; i < p.size() && !is_ctx_null(ctx); i++)
        ctx = mk_direct_node(ctx, p[i], comp_vals[i], content_vals[i]);

    return ctx;
}

ctx_t<node_id_t>
hierarchy::evaluate_ctx(ctx_t<node_id_t> ctx, bool follow_curr_softlink, bool follow_curr_hardlink, bool follow_curr_mount) noexcept {
    while (!is_ctx_null(ctx) && ctx.hier->driver->has_node(ctx.val)) {
        comp_id_t comp_id = ctx.hier->driver->read_node(ctx.val)->comp_id;
        comp_type comp_type = ctx.hier->driver->read_comp_type(comp_id);

        // follow softlink
        switch (comp_type) {
            case comp_type::softlink: {
                if (!follow_curr_softlink) goto eval_end;;
                auto comp = ctx.hier->driver->read_comp(comp_id);
                auto link_ptr = comp::get_ptr<softlink>(comp.get());
                ctx = search_node(link_ptr->target.hier->get_root_ctx(), link_ptr->target.val,
                                  true, true);
                break;
            }
            case comp_type::hardlink: {
                if (!follow_curr_hardlink) goto eval_end;
                auto comp = ctx.hier->driver->read_comp(comp_id);
                auto link_ptr = comp::get_ptr<hardlink>(comp.get());
                ctx = link_ptr->target;
                break;
            }
            case comp_type::mount: {
                if (!follow_curr_mount) goto eval_end;
                auto comp = ctx.hier->driver->read_comp(comp_id);
                ctx = comp::get_ptr<mount>(comp.get())->target_hier->get_root_ctx();
                break;
            }
            default: goto eval_end;
        }
    }
eval_end: {
        if (!is_ctx_null(ctx) && ctx.hier->driver->has_node(ctx.val)) return ctx;
        else return null_ctx<node_id_t>();
    }
}

ctx_t<node_id_t> hierarchy::mk_node(ctx_t<node_id_t> ctx, const path &p, const comp_data &comp_val,
                                          const content_data &content_val, bool return_on_success) noexcept {
    if (p.empty()) return return_on_success ? ctx_t<node_id_t>{} : ctx;

    for (int i = 0; i < p.size() - 1 && !is_ctx_null(ctx); i++)
        ctx = mk_direct_node(ctx, p[i], comp(dir()), content(null_content())); // make empty directories
    if (is_ctx_null(ctx))
        return null_ctx<node_id_t>();
    if (ctx_t<node_id_t> child = search_node(ctx, path{p.back()}, true, true); !is_ctx_null(child)) // node already exists
        return return_on_success ? ctx_t<node_id_t>{} : child;

    return mk_direct_node(ctx, p.back(), comp_val, content_val);
}

void hierarchy::rm_node(ctx_t<node_id_t> ctx, bool recursive) noexcept {
    auto node = ctx.hier->driver->read_node(ctx.val);
    auto comp = ctx_t(ctx.hier, ctx.hier->driver->read_comp(node->comp_id));
    auto refs = ctx.hier->driver->read_refs(node->refs_id);

    // update dirs which point to this node
    for (const ctx_t<comp_id_t>& dir_ref: refs->dirs) {
        auto dir_comp = read_comp(dir_ref);
        dir::remove_child_from_dir(dir_comp, ctx, false);
    }

    // nullify hardlinks which point to this node
    for (const ctx_t<comp_id_t>& hardlink_ref: refs->hardlinks) {
        auto hardlink_comp = read_comp(hardlink_ref);
        hardlink::nullify_link(hardlink_comp, false);
    }

    switch (comp::get_type(*comp.val)) {
        default:
        case comp_type::null: return;
        case comp_type::softlink:
        case comp_type::mount: {


            ctx.hier->driver->deallocate_content(node->content_id); // deallocate the content
            ctx.hier->driver->deallocate_comp(node->comp_id); // deallocate the content
            ctx.hier->driver->deallocate_refs(node->refs_id); // deallocate the refs
            ctx.hier->driver->deallocate_node(ctx.val); // deallocate the node
            return;
        }
        case comp_type::dir: {
            if (!recursive) return;

            // recursivly remove the child nodes
            for (const ctx_t<node_id_t>& child: dir::get_all_children(comp))
                rm_node(child, recursive);

            ctx.hier->driver->deallocate_comp(node->comp_id); // deallocate the comp
            ctx.hier->driver->deallocate_refs(node->refs_id); // deallocate the refs
            ctx.hier->driver->deallocate_content(node->content_id); // deallocate the content // todo hard links?
            ctx.hier->driver->deallocate_node(ctx.val); // deallocate the node
            return;
        }
    }
}

ctx_t<node_id_t> hierarchy::search_node(ctx_t<node_id_t> ctx, const path &p, bool follow_softlink_at_end,bool follow_hardlink_at_end,
                                              bool follow_mount_at_end) noexcept {
    if (is_ctx_null(ctx) || !ctx.hier->driver->has_node(ctx.val)) return null_ctx<node_id_t>();

    for (int i = 0; i < p.size() && !is_ctx_null(ctx); i++) {
        // preevaluate current context
        ctx = evaluate_ctx(ctx, (i != p.size() - 1) ? true : follow_softlink_at_end, (i != p.size() - 1 ? true : follow_hardlink_at_end), (i != p.size() - 1) ? true : follow_mount_at_end);

        auto node_comp = ctx_t(ctx.hier, ctx.hier->driver->read_comp(ctx.hier->driver->read_node(ctx.val)->comp_id));
        switch (comp::get_type(*node_comp.val)) {
            case comp_type::dir: {
                ctx = dir::search_path_comp(node_comp, p[i]);
                break;
            }
            // if this ctx is either softlink or mount, it means we didn't evalute it in the preevaluation therefore this is the end of the path
            case comp_type::softlink:
            case comp_type::mount:
                return ctx;
            default: {
                ctx = null_ctx<node_id_t>();
                break;
            }
        }
    }

    return ctx;
}

ctx_t<std::unique_ptr<node> > hierarchy::stat_node(ctx_t<node_id_t> ctx) noexcept {
    return {ctx.hier, std::move(ctx.hier->driver->read_node(ctx.val))};
}

ctx_t<comp_id_t> hierarchy::stat_comp_id(ctx_t<node_id_t> ctx) noexcept {
    return {ctx.hier, ctx.hier->driver->read_node(ctx.val)->comp_id};
}

ctx_t<refs_id_t> hierarchy::stat_refs_id(ctx_t<node_id_t> ctx) noexcept {
    return {ctx.hier, ctx.hier->driver->read_node(ctx.val)->refs_id};
}

ctx_t<content_id_t> hierarchy::stat_content_id(ctx_t<node_id_t> ctx) noexcept {
    return {ctx.hier, ctx.hier->driver->read_node(ctx.val)->content_id};
}

comp_type hierarchy::stat_comp_type(ctx_t<node_id_t> ctx) noexcept {
    return ctx.hier->driver->read_comp_type(ctx.hier->driver->read_node(ctx.val)->comp_id);
}

content_type hierarchy::stat_content_type(ctx_t<node_id_t> ctx) noexcept {
    return ctx.hier->driver->read_content_type(ctx.hier->driver->read_node(ctx.val)->content_id);
}

ctx_t<std::unique_ptr<comp> > hierarchy::read_comp(ctx_t<comp_id_t> ctx) noexcept {
    return {ctx.hier, std::move(ctx.hier->driver->read_comp(ctx.val))};
}

ctx_t<std::unique_ptr<refs>> hierarchy::read_refs(ctx_t<refs_id_t> ctx) noexcept {
    return {ctx.hier, std::move(ctx.hier->driver->read_refs(ctx.val))};
}

ctx_t<std::unique_ptr<content> > hierarchy::read_content(ctx_t<content_id_t> ctx) noexcept {
    return {ctx.hier, std::move(ctx.hier->driver->read_content(ctx.val))};
}

ctx_t<std::unique_ptr<dir> > hierarchy::stat_dir(ctx_t<node_id_t> ctx) noexcept {
    return read_comp<dir>(stat_comp_id(ctx));
}

ctx_t<node_id_t> hierarchy::mk_dir(ctx_t<node_id_t> ctx, const path &p) noexcept {
    return mk_node(ctx, p, dir()/* empty dir at the end */, content(null_content()), true);
}

ctx_t<std::unique_ptr<softlink> > hierarchy::stat_softlink(ctx_t<node_id_t> ctx) noexcept {
    return read_comp<softlink>(stat_comp_id(ctx));
}

ctx_t<node_id_t> hierarchy::mk_softlink(ctx_t<node_id_t> ctx, const path &p, const softlink &link) noexcept {
    return mk_node(ctx, p, link, content(null_content()), true);
}

ctx_t<std::unique_ptr<hardlink>> hierarchy::stat_hardlink(ctx_t<node_id_t> ctx) noexcept {
    return read_comp<hardlink>(stat_comp_id(ctx));
}

ctx_t<node_id_t> hierarchy::mk_hardlink(ctx_t<node_id_t> ctx, const path &p, const hardlink &link) noexcept {
    return mk_node(ctx, p, link, content(null_content()), true);
}

ctx_t<std::unique_ptr<mount>> hierarchy::stat_mount(ctx_t<node_id_t> ctx) noexcept {
    return read_comp<mount>(stat_comp_id(ctx));
}

ctx_t<node_id_t> hierarchy::mk_mount(ctx_t<node_id_t> ctx, const path &p, const mount &mnt) noexcept {
    return mk_node(ctx, p, comp_data(mnt), content_data(), true);
}

void hierarchy::rm_mount(ctx_t<node_id_t> ctx, const path &p) noexcept {
    rm_node(search_node(ctx, p, true, false));
}

ctx_t<node_id_t> hierarchy::mv_mount(ctx_t<node_id_t> ctx, const path &src, const path &dest) noexcept {
    ctx_t<node_id_t> src_ctx = search_node(ctx, src, true, false);
    if (is_ctx_null(src_ctx)) return null_ctx<node_id_t>();

    auto src_node = src_ctx.hier->driver->read_node(src_ctx.val);
    if (src_ctx.hier->driver->read_comp_type(src_node->comp_id) != comp_type::mount) // source is not mounted
        return null_ctx<node_id_t>();


    ctx_t<node_id_t> dest_ctx = mk_node(ctx, dest, comp(dir()), content(null_content()), false);
    // make dest node (in case not exist)
    if (is_ctx_null(dest_ctx)) return null_ctx<node_id_t>();

    auto dest_node = dest_ctx.hier->driver->read_node(dest_ctx.val);
    if (dest_ctx.hier->driver->read_comp_type(dest_node->comp_id) != comp_type::null)
        // node was already exist and has nonnull comp
        return null_ctx<node_id_t>();


    // move the content id
    dest_node->comp_id = src_node->comp_id;
    src_node->comp_id = comp_id_null;

    // write the nodes
    dest_ctx.hier->driver->write_node(src_node.get());
    src_ctx.hier->driver->write_node(dest_node.get());

    return dest_ctx;
}

ctx_t<node_id_t> hierarchy::cp_mount(ctx_t<node_id_t> ctx, const path &src, const path &dest) noexcept {
    ctx_t<node_id_t> src_ctx = search_node(ctx, src, true, false);
    if (is_ctx_null(src_ctx)) return null_ctx<node_id_t>();

    auto src_node = src_ctx.hier->driver->read_node(src_ctx.val);
    if (src_ctx.hier->driver->read_comp_type(src_node->comp_id) != comp_type::mount) // source is not mounted
        return null_ctx<node_id_t>();


    ctx_t<node_id_t> dest_ctx = mk_node(ctx, dest, comp(dir()), content(null_content()), false);
    // make dest node (in case not exist)
    if (is_ctx_null(dest_ctx)) return null_ctx<node_id_t>();

    auto dest_node = dest_ctx.hier->driver->read_node(dest_ctx.val);
    if (dest_ctx.hier->driver->read_comp_type(dest_node->comp_id) != comp_type::null)
        // node was already exist and has nonnull comp
        return null_ctx<node_id_t>();


    // make copy of the mount comp
    comp_id_t comp_copy_id = dest_ctx.hier->driver->allocate_comp();
    auto mnt = dest_ctx.hier->driver->read_comp(src_node->comp_id);
    mount mnt_copy(*comp::get_ptr<mount>(mnt.get()));
    mnt_copy.id = comp_copy_id;

    // write the comp
    comp comp_copy(mnt_copy);
    dest_ctx.hier->driver->write_comp(&comp_copy);

    // assign the comp to the dest node
    dest_node->comp_id = comp_copy_id;

    // write the dest node
    dest_ctx.hier->driver->write_node(src_node.get());

    return dest_ctx;
}

ctx_t<node_id_t> hierarchy::mk_textfile(ctx_t<node_id_t> ctx, const path &p) noexcept {
    return mk_node(ctx, p, comp(null_comp()), content(textfile()), true);
}

ctx_t<std::unique_ptr<textfile> > hierarchy::open_textfile(ctx_t<node_id_t> ctx) noexcept {
    return read_content<textfile>(stat_content_id(ctx));
}

void hierarchy::close_textfile(hierarchy *fs, textfile *file) noexcept {
    content temp{content_data{*file}};
    fs->driver->write_content(&temp);
}

ctx_t<std::unique_ptr<ctl_dev_pt>> hierarchy::stat_ctl_dev_pt(ctx_t<node_id_t> ctx) noexcept {
    return read_content<ctl_dev_pt>(stat_content_id(ctx));
}

ctx_t<node_id_t> hierarchy::mk_ctl_dev_pt(ctx_t<node_id_t> ctx, const path &p, const ctl_dev_pt &pt) noexcept {
    return mk_node(ctx, p, comp_data(null_comp()), content_data(pt), true);
}

ctx_t<std::unique_ptr<stream_dev_pt>> hierarchy::stat_stream_dev_pt(ctx_t<node_id_t> ctx) noexcept {
    return read_content<stream_dev_pt>(stat_content_id(ctx));
}

ctx_t<node_id_t> hierarchy::mk_stream_dev_pt(ctx_t<node_id_t> ctx, const path &p, const stream_dev_pt &pt) noexcept {
    return mk_node(ctx, p, comp_data(null_comp()), content_data(pt), true);
}
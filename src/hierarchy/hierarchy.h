#ifndef HIERARCHY_H
#define HIERARCHY_H

#include <memory>

#include "node/node.h"
#include "node/node_id.h"
#include "path/path.h"
#include "content/content.h"

#include "basic_driver.h"
#include "ctx_t.h"
#include "hier_id.h"

struct hierarchy {
    friend struct dir;
    friend struct softlink;

protected:
    basic_driver *driver;
    node_id_t root_id;

public:
    hier_id id;

    explicit hierarchy(hier_id id, basic_driver *driver)
        : id(id), driver(driver) {
        // initialize the filesystem with a root node of type directory

        // allocte a root node
        root_id = driver->allocte_node();
        node root_node(root_id);

        // allocte a directory as the root comp
        comp_id_t comp_id = driver->allocate_comp();
        struct dir dir{};
        dir.id = comp_id;
        comp root_dir{dir};

        // allocte empty refs
        refs_id_t refs_id = driver->allocte_refs();
        refs refs(refs_id);

        // write the new node and all its data
        root_node.comp_id = comp_id;
        root_node.refs_id = refs_id;
        driver->write_node(&root_node);
        driver->write_comp(&root_dir);
        driver->write_refs(&refs);
    }

    virtual ~hierarchy() {
    }


    /*** fundamental internal functions ***/
protected:
    /** make nodes */
    // create a node in the path comp directly down from the given root node
    static ctx_t<node_id_t> mk_direct_node(ctx_t<node_id_t> ctx, const path_comp &p,
                                           const comp_data &comp_val) noexcept;

    // search the path starting from the given root node, create any intermediate nodes if needed
    static ctx_t<node_id_t> mk_path(ctx_t<node_id_t> ctx, const path &p,
                                    const std::vector<comp_data> &comp_vals) noexcept;

    // search the path going down from the given ctx, create any intermediate directory nodes if needed, and create the end node as given.
    static ctx_t<node_id_t> mk_node(ctx_t<node_id_t> ctx, const path &p, const comp_data &comp_val,
                                    bool return_on_success) noexcept;

    /** evalute ctx (jump softlinks and mounts) */
    static ctx_t<node_id_t> evaluate_ctx(ctx_t<node_id_t> ctx, bool follow_curr_softlink,
                                         bool follow_curr_mount) noexcept;

    /** read node */
    static ctx_t<comp_id_t> stat_comp_id(ctx_t<node_id_t> ctx) noexcept;

    // static comp_type stat_comp_type(ctx_t<node_id_t> ctx) noexcept;

    static ctx_t<content_id_t> stat_content_id(ctx_t<node_id_t> ctx) noexcept;

    // static content_type stat_content_type(ctx_t<node_id_t> ctx) noexcept;

    static ctx_t<refs_id_t> stat_refs_id(ctx_t<node_id_t> ctx) noexcept;


    /** read component */
    static ctx_t<std::unique_ptr<comp> > stat_comp(ctx_t<comp_id_t> ctx) noexcept;

    template<typename T>
    static ctx_t<std::unique_ptr<T> > stat_comp(ctx_t<comp_id_t> ctx) {
        auto comp = stat_comp(ctx);
        return {ctx.hier, std::make_unique<T>(*comp::get_ptr<T>(comp.val.get()))};
    }

    /** read content */
    static ctx_t<std::unique_ptr<content> > stat_content(ctx_t<content_id_t> ctx) noexcept;

    template<typename T>
    static ctx_t<std::unique_ptr<T> > stat_content(ctx_t<content_id_t> ctx) {
        auto comp = stat_content(ctx);
        return {ctx.hier, std::make_unique<T>(*content::get_ptr<T>(comp.val.get()))};
    }

    /** read references */
    static ctx_t<std::unique_ptr<refs> > stat_refs(ctx_t<refs_id_t> ctx) noexcept; // read comp


    /*** filesystem API ***/
public:
    // all filesystems should have a single root node
    ctx_t<node_id_t> get_root_ctx() noexcept;

    /** node related */
    static void rm_node(ctx_t<node_id_t> ctx, bool recursive = false) noexcept;

    static ctx_t<node_id_t> mv_node(ctx_t<node_id_t> ctx, const path &src, const path &dest) noexcept;

    // search the path going down from the given root node
    static ctx_t<node_id_t> search_node(ctx_t<node_id_t> ctx, const path &p, bool follow_softlink_at_end = true,
                                        bool follow_mount_at_end = true) noexcept;

    // test if a path exists (points to a valid node)
    static inline bool node_exist(ctx_t<node_id_t> ctx, const path &p, bool follow_softlink_at_end,
                                  bool follow_mount_at_end) noexcept;

    // retrieve info about a node
    static ctx_t<std::unique_ptr<node> > stat_node(ctx_t<node_id_t> ctx) noexcept;

    /** directory specific funcs */
    static ctx_t<std::unique_ptr<dir> > stat_dir(ctx_t<node_id_t> ctx) noexcept;

    // search the path going down from the given root node, create any intermediate directory nodes if needed, and create an empty dir at the end
    // (this function is just a wrapper around mk_node)
    static ctx_t<node_id_t> mk_dir(ctx_t<node_id_t> ctx, const path &p) noexcept;

    /** softlink specific funcs */
    static ctx_t<std::unique_ptr<softlink> > stat_softlink(ctx_t<node_id_t> ctx) noexcept;

    static ctx_t<node_id_t> mk_softlink(ctx_t<node_id_t> ctx, const path &p, const softlink &link) noexcept;

    /** mount specific funcs */
    static ctx_t<std::unique_ptr<mount> > stat_mount(ctx_t<node_id_t> ctx) noexcept;

    static ctx_t<node_id_t> mk_mount(ctx_t<node_id_t> ctx, const path &p, const mount &mnt) noexcept;

    static ctx_t<node_id_t> cp_mount(ctx_t<node_id_t> ctx, const path &src, const path &dest) noexcept;

    /** textfile specific funcs */
    // search the path going down from the given root node, create any intermediate directory nodes if needed, and create an empty textfile at the end
    // (this function is just a wrapper around mk_node)
    static ctx_t<node_id_t> mk_textfile(ctx_t<node_id_t> ctx, const path &p) noexcept;

    static ctx_t<std::unique_ptr<textfile> > open_textfile(ctx_t<node_id_t> ctx) noexcept;

    static void close_textfile(hierarchy *fs, textfile *file) noexcept;

    // /** ctl dev specific funcs */
    // // search the path going down from the given root node, create any intermediate directory nodes if needed, and create an empty textfile at the end
    // // (this function is just a wrapper around mk_node)
    // static ctx_t<std::unique_ptr<ctl_dev_pt>> stat_ctl_dev_pt(ctx_t<node_id_t> ctx) noexcept;
    //
    // static ctx_t<node_id_t> mk_ctl_dev_pt(ctx_t<node_id_t> ctx, const path &p, const ctl_dev_pt& pt) noexcept;
    //
    // /** stream dev specific funcs */
    // // search the path going down from the given root node, create any intermediate directory nodes if needed, and create an empty textfile at the end
    // // (this function is just a wrapper around mk_node)
    // static ctx_t<std::unique_ptr<stream_dev_pt>> stat_stream_dev_pt(ctx_t<node_id_t> ctx) noexcept;
    //
    // static ctx_t<node_id_t> mk_stream_dev_pt(ctx_t<node_id_t> ctx, const path &p, const stream_dev_pt& pt) noexcept;

    /** hardlinking */
    static ctx_t<node_id_t> mk_hardlink(ctx_t<node_id_t> ctx, const path &src, const path &dest);
};

#endif //HIERARCHY_H

#ifndef BASIC_HIERARCHY_H
#define BASIC_HIERARCHY_H

#include <memory>

#include "node/node.h"
#include "node/node_id.h"
#include "path/path.h"
#include "content/content.h"

#include "basic_driver.h"

struct basic_hierarchy {
    basic_driver *driver;

    basic_hierarchy(basic_driver *driver)
        : driver(driver) {
    }

    virtual ~basic_hierarchy() {
    }

    /** filesystem api */
public:
    // all filesystems should have a single root node
    virtual node_id_t get_root_node_id() = 0;

protected:
    // create a node in the path comp directly down from the given root node
    node_id_t mk_direct_node(node_id_t parent, const path_comp &p, const comp_data &comp_val,
                             const content_data &content_val);

    // search the path starting from the given root node, create any intermediate nodes if needed
    node_id_t mk_path(node_id_t id, const path &p, const std::vector<comp_data> &comp_vals,
                      const std::vector<content_data> &content_vals);

    // search the path going down from the given root node, create any intermediate directory nodes if needed, and create the end node as given
    node_id_t mk_node(node_id_t id, const path &p, const comp_data &comp_val,
                      const content_data &content_val);

    // read the node's comp
    std::unique_ptr<comp> stat_comp(content_id_t id);

    // read the node's content
    std::unique_ptr<content> stat_content(content_id_t id);

public:
    /** search for a node by a path */
    // search the path component going down from the given root node
    node_id_t search_path_comp(node_id_t parent, const path_comp &p);

    // search the path going down from the given root node
    node_id_t search_path(node_id_t parent, const path &p);

    /** test if a path exists (points to a valid node) */
    inline bool path_comp_exists(node_id_t parent, const path_comp &p) {
        return search_path_comp(parent, p) != node_id_null;
    }

    inline bool path_exists(node_id_t parent, const path &p) {
        return search_path(parent, p) != node_id_null;
    }

    /** retrieve info about a node and its data */
    std::unique_ptr<node> stat_node(node_id_t node);

    comp_type stat_comp_type(node_id_t id);

    content_type stat_content_type(node_id_t id);

    /** softlink */
    node_id_t follow_softlink(node_id_t link_id, int max_steps = 0);

    /** delete nodes */
    void rm_node(node_id_t id, bool recursive = false);
};


#endif //BASIC_HIERARCHY_H

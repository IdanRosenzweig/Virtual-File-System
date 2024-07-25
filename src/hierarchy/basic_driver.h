#ifndef BASIC_DRIVER_H
#define BASIC_DRIVER_H

#include <memory>

#include "node/node.h"
#include "comp/comp.h"
#include "refs/refs.h"
#include "content/content.h"

struct basic_driver {
    friend struct dir;
    friend struct softlink;
    friend struct hardlink;
    friend struct textfile;

    /** internal driver related functions that abstracts the hier logic */

    /* regarding nodes */

    // allcoate a new node in the filesystem under a unique id.
    // return its id
    virtual node_id_t allocte_node() noexcept = 0;

    // deallocate the node with the specified id from the filesystem
    virtual void deallocate_node(node_id_t id) noexcept = 0;

    // returns true if the specified node exists, false otherwise
    virtual bool has_node(node_id_t id) noexcept = 0;

    // read from the filesystem the current data of the node with the specific id.
    // return its data (allocated locally), or nullptr if node doesn't exist
    // todo possible to add optional parameters (bool node_name, bool content, bool node_refs) to indicate whether to read thos or not
    virtual std::unique_ptr<node> read_node(node_id_t id) noexcept = 0;

    // update in the filesystem the corresponding data of the node with the specified id (if id exists)
    virtual void write_node(const node *node) noexcept = 0;

    /* regarding components */

    virtual comp_id_t allocate_comp() noexcept = 0;

    virtual void deallocate_comp(comp_id_t id) noexcept = 0;

    virtual bool has_comp(comp_id_t id) noexcept = 0;

    virtual comp_type read_comp_type(comp_id_t id) noexcept = 0;

    virtual std::unique_ptr<comp> read_comp(comp_id_t id) noexcept = 0;

    virtual void write_comp(const comp *node) noexcept = 0;

    /* regarding references */

    virtual refs_id_t allocte_refs() noexcept = 0;

    virtual void deallocate_refs(refs_id_t id) noexcept = 0;

    virtual bool has_refs(refs_id_t id) noexcept = 0;

    virtual std::unique_ptr<refs> read_refs(refs_id_t id) noexcept = 0;

    virtual void write_refs(const refs *refs) noexcept = 0;

    /* regarding content */

    virtual content_id_t allocate_content() noexcept = 0;

    virtual void deallocate_content(content_id_t id) noexcept = 0;

    virtual bool has_content(content_id_t id) noexcept = 0;

    virtual content_type read_content_type(content_id_t id) noexcept = 0;

    virtual std::unique_ptr<content> read_content(content_id_t id) noexcept = 0;

    virtual void write_content(const content *node) noexcept = 0;

};

#endif //BASIC_DRIVER_H

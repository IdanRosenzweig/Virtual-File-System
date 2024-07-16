#include "node.h"


node::node(): fs_related(nullptr, node_id_null), name(node_name_null), data(null_node()) {
}

node::node(const node_fs_related &fs_related): fs_related(fs_related) {
}

node::node(const node &other): fs_related(other.fs_related),
                               name(other.name),
                               data(other.data) {
}

node::node(node &&other) noexcept: fs_related(std::move(other.fs_related)),
                                   name(std::move(other.name)),
                                   data(std::move(other.data)) {
}

node & node::operator=(const node &other) {
    if (this == &other)
        return *this;
    fs_related = other.fs_related;
    name = other.name;
    data = other.data;
    return *this;
}

node & node::operator=(node &&other) noexcept {
    if (this == &other)
        return *this;
    fs_related = std::move(other.fs_related);
    name = std::move(other.name);
    data = std::move(other.data);
    return *this;
}





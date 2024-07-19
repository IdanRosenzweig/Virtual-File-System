#include "content_fs.h"

node_id_t content_fs::get_root_node_id() {
    return root_id;
}

std::unique_ptr<dir> content_fs::stat_dir(node_id_t id) {
    auto comp = stat_comp(stat_node(id)->content_id);
    return std::make_unique<dir>(*comp::get_ptr<dir>(comp.get()));
}

node_id_t content_fs::mk_dir(node_id_t parent, const path &path) {
    return mk_node(parent, path, dir()/* empty dir at the end */, content(null_content()));
}

node_id_t content_fs::mk_textfile(node_id_t parent, const path &path) {
    return mk_node(parent, path, comp(null_comp()), content(textfile()));
}

std::unique_ptr<textfile> content_fs::open_textfile(node_id_t id) {
    auto content = stat_content(stat_node(id)->content_id);
    return std::make_unique<textfile>(*content::get_ptr<textfile>(content.get()));
}

void content_fs::close_textfile(textfile *file) {
    content temp{content_data{*file}};
    driver->write_content(&temp);
}

std::unique_ptr<softlink> content_fs::stat_softlink(node_id_t id) {
    auto comp = stat_comp(stat_node(id)->content_id);
    return std::make_unique<softlink>(
        *comp::get_ptr<softlink>(comp.get()));
}

node_id_t content_fs::mk_softlink(node_id_t root, const path &path, const softlink &link) {
    return mk_node(root, path, link, content(null_content()));
}

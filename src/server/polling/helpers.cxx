//
// Created by ingvord on 11/1/16.
//

#include "helpers.hxx"
#include "command.hxx"

using namespace std;

using namespace Tango;
//Should be in PollThread but kept here due to std=c++1y
experimental::optional<Tango::WorkItem>
Tango::polling::find_work_item(bool polling_bef_9, const Command &cmd, PollObj &poll_obj,
                        const list<WorkItem> &work_items) {
    experimental::optional<WorkItem> optional_work_item{};
    if (polling_bef_9) return optional_work_item;

    Tango::PollObjType type = poll_obj.get_type();
    bool is_attr = type == POLL_ATTR;
    long idl_version = cmd.dev_->get_dev_idl_version();
    if (is_attr && idl_version >= 4) {
        long update = poll_obj.get_upd();
        auto ite = find_if(work_items.begin(), work_items.end(),
                           [&](const WorkItem &work_item) {
                               return work_item.dev == cmd.dev_ && work_item.update == update && work_item.type == type;
                           });

        if (ite != work_items.end()) {
            experimental::optional<WorkItem> other = experimental::make_optional(*ite);
            optional_work_item.swap(other);
        }
    }
    return optional_work_item;
}
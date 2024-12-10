#pragma once
#ifndef VT_RESOLUTION_SEMINTICTABLE_SEMINTICTABLE_H
#define VT_RESOLUTION_SEMINTICTABLE_SEMINTICTABLE_H
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <Core/CommandList/CommandList.h>

//
//to_main_textbox,  // draw
//put_string,
//clean_textbox,
//draw_background,
//draw_character,
//draw_character_animation,
//clean_all_character,
//clean_background,
//
//create_textbox,
//create_selection_box,
//
//create_node,
//go_to_node,
//end_of_program,
//exit_program_and_go_to_node,
//
//create_character,
//set_character_state,
//set_character_animation,
//
//go_to_file,
//

namespace vtresolution
{
    class SemanticTable
    {
    private:
        typedef std::pair<std::string, vtcore::command::CommandList> StringToCommand;
        std::vector<StringToCommand> semantic_table_
        {
            {"create_character", vtcore::command::create_character},
            {"set_character_state", vtcore::command::set_character_state},
            {"set_character_animation", vtcore::command::set_character_animation},
            {"to_main_textbox", vtcore::command::to_main_textbox},
            {"put_string", vtcore::command::put_string},
            {"define_command", vtcore::command::define_command},
        };
    public:
        SemanticTable()
        {
            std::sort(semantic_table_.begin(), semantic_table_.end(),
                [](const StringToCommand& lhs, const StringToCommand& rhs) -> bool
            {
                return lhs.first < rhs.first;
            }
            );
            vtcore::lst.logIn("Prepare semantic table...", vtcore::logsys::LOG_PRIORITY_INFO, vtcore::logsys::LOG_CATEGORY_APPLICATION);
        }
        vtcore::command::CommandList searchString(const std::string str);
    };
}

#endif
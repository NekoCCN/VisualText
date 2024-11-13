#pragma once
#ifndef _VISUALTEXT_CORE_COMMANDLIST_COMMANDLIST_H_
#define _VISUALTEXT_CORE_COMMANDLIST_COMMANDLIST_H_

namespace vtcore
{
	namespace command
	{
		enum CommandList
		{
			to_main_textbox,  // draw
			put_string,
			clean_textbox,
			draw_background,
			draw_character,
			draw_character_animation,
			clean_all_character,
			clean_background,

			create_textbox,
			create_selection_box,

			create_node,
			go_to_node,
			end_of_program,
			exit_program_and_go_to_node,

			create_character,
			set_character_state,
			set_character_animation,

			go_to_file,
		};
	}
};

#endif
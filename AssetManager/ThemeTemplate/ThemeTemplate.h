#include <string>
#include <vector>

namespace vtasset
{
	class ThemeAssetList
	{
	protected:
		enum ThemeAssetIndex
		{
			FONT1,
			FONT2,
			MAINUILOGO,
			SAVE,
			LOAD_ICON,
			TREE,
			TEXTLIST,
			SETTING_ICON,
			START,
			SETTING,
			LOAD,
		};
		std::vector<std::string> ThemeTemplateStr
		{
			"FONT1.ttf",
			"FONT2.ttf",
			"MAINUILOGO.png",
			"SAVE.svg",
			"LOAD_ICON.svg",
			"TREE.svg",
			"TEXTLIST.svg",
			"SETTING_ICON.svg",
			//"START.png",
			//"SETTING.png",
			//"LOAD.png",
		};
	};
}
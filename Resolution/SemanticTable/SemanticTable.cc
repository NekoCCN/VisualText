#include "SemanticTable.h"

vtcore::command::CommandList vtresolution::SemanticTable::searchString(const std::string str)
{
	int max = semantic_table_.size() - 1;
	int min = 0;
    while (min < max)
    {
        int mid = min + max + 1 >> 1;
        if (semantic_table_[mid].first < str)
        {
            min = mid;
        }
        else
        {
            max = mid - 1;
        }
    }
    if (semantic_table_[min].first == str)
    {
        return semantic_table_[min].second;
    }
    else
    {
        return (vtcore::command::CommandList)-1;
    }
}
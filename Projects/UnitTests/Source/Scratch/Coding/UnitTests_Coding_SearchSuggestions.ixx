module;

#include "Debugging/Logger.h"

export module UnitTests_Coding_SearchSuggestions;

import jpt.DynamicArray;
import jpt.String;

jpt::DynamicArray<jpt::DynamicArray<jpt::String>> SearchSuggestions(const jpt::DynamicArray<jpt::String>& repository, const jpt::String& customerQuery)
{
    jpt::DynamicArray<jpt::DynamicArray<jpt::String>> suggestions;

    for (size_t i = 2; i <= customerQuery.Count(); ++i)
    {
        jpt::DynamicArray<jpt::String> candidates;

        // For each substr, look for any potential matches in the repository
        jpt::String substr = customerQuery.SubStr(0, i);
        
        for (const jpt::String& word : repository)
        {
            if (word.Find(substr.ConstBuffer()) == 0)
            {
                candidates.EmplaceBack(word);
            }
        }

        suggestions.EmplaceBack(candidates);
    }

    return suggestions;
}

export bool UnitTests_Coding_SearchSuggestions()
{
    jpt::DynamicArray<jpt::DynamicArray<jpt::String>> result;
    jpt::DynamicArray<jpt::String> repository;
    jpt::String customerQuery;

    repository = { "mobile", "mouse", "mouth", "moneypot", "monitor", "mousepad" };
    customerQuery = "mouse";
    result = SearchSuggestions(repository, customerQuery);

    return true;
}
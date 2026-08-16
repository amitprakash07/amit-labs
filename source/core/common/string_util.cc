
#include "string_util.h"

#include <algorithm>
#include <map>

using namespace amit::common;
std::string amit::common::RemoveString(const std::string& remove_from,
                                             const std::string& remove_with,
                                             bool               beginning)
{
    std::string returnString;
    if (beginning)
    {
        char* returnStringInChar = new char[remove_from.size() - remove_with.size() + 1];
        memcpy(returnStringInChar, remove_from.c_str() + remove_with.size(), remove_from.size() - remove_with.size());
        returnStringInChar[remove_from.size() - remove_with.size()] = '\0';
        returnString                                                = returnStringInChar;
        delete[] returnStringInChar;
    }
    return returnString;
}
std::string amit::common::ReverseEachWordsInString(std::string sentence, char separator)
{
    size_t start_index = 0;
    size_t end_index   = 0;

    while (sentence[end_index] != '\0')
    {
        if (sentence[end_index + 1] == separator || sentence[end_index + 1] == '\0')
        {
            // reverse word
            size_t word_end_index = end_index;
            while (start_index < word_end_index)
            {
                char temp                = sentence[start_index];
                sentence[start_index]    = sentence[word_end_index];
                sentence[word_end_index] = temp;
                start_index++;
                word_end_index--;
            }

            end_index++;
            start_index = end_index + 1;
        }
        else
        {
            end_index++;
        }
    }

    return sentence;
}

std::string amit::common::ReverseOnlyLetters(std::string s)
{
    size_t start_index = 0;
    size_t end_index   = s.size();
    while (start_index < end_index)
    {
        if (!IsLetter(s[start_index]))
        {
            start_index++;
            continue;
        }

        if (!IsLetter(s[end_index]))
        {
            end_index--;
            continue;
        }

        char temp      = s[start_index];
        s[start_index] = s[end_index];
        s[end_index]   = temp;
        start_index++;
        end_index--;
    }

    return s;
}

bool amit::common::IsLetter(char ch)
{
    // A-Z : 65 - 90
    // a-z : 97 - 122
    if (65 <= ch && ch <= 90)
    {
        return true;
    }

    if (97 <= ch && ch <= 122)
    {
        return true;
    }

    return false;
}
std::string amit::common::RemoveDuplicates(const std::string& s)
{
    std::stack<char> char_stack{};
    char_stack.push(s[0]);

    for (size_t i = 1; i < s.size(); i++)
    {
        if (!char_stack.empty() && s[i] == char_stack.top())
        {
            char_stack.pop();
        }
        else
        {
            char_stack.push(s[i]);
        }
    }

    std::string ret_str;
    ret_str.resize(char_stack.size());
    size_t it = char_stack.size() - 1;

    while (!char_stack.empty())
    {
        ret_str[it] = char_stack.top();
        char_stack.pop();
        it--;
    }

    return ret_str;
}

size_t amit::common::LengthOfLongestSubstring(const std::string& str)
{
    size_t                 right = 0, left = 0;
    std::map<char, size_t> char_last_index;
    size_t                 max_length = 0;

    while (right < str.size())
    {
        if (char_last_index.contains(str[right]))
        {
            left = std::max(left, char_last_index[str[right]] + 1);
        }

        char_last_index[str[right]] = static_cast<int>(right);
        max_length                  = std::max(max_length, right - left + 1);
        right++;
    }

    return max_length;
}
std::string amit::common::SimplifyPath(const std::string& path)
{
    std::string canonical_path;

    size_t it = 0;
    for (char ch : path)
    {
        if (ch == '/')
        {
            if (!canonical_path.empty() && canonical_path.back() == '/')
            {
                continue;
            }
            else if (canonical_path.size() > 2 && canonical_path[it - 1] == '.' && canonical_path[it - 2] == '.')
            {
                canonical_path.pop_back();
                it--;
                canonical_path.pop_back();
                it--;

                if (canonical_path.size() == 1)
                {
                    break;
                }

                canonical_path.pop_back();
                it--;

                char prev_char = canonical_path.back();

                while (prev_char != '/')
                {
                    canonical_path.pop_back();
                    prev_char = canonical_path.back();
                    it--;
                }

                continue;
            }

            canonical_path.push_back(ch);
            it++;
        }
        else
        {
            canonical_path.push_back(ch);
            it++;
        }
    }

    if (canonical_path.size() > 1 && canonical_path.back() == '/')
    {
        canonical_path.pop_back();
    }

    return canonical_path;
}

std::string amit::common::ToUnixFilePath(const std::string& file_path)
{
    std::string file_path_copy;
    file_path_copy.reserve(file_path.size());

    for (size_t it = 0; it < file_path.size(); it++)
    {
        if (file_path[it] == '\\')
        {
            file_path_copy.push_back('/');

            if (file_path[it + 1] == '\\')
            {
                while (it < file_path.size() && file_path[it] == '\\')
                {
                    it++;
                }
            }
        }
        else
        {
            file_path_copy.push_back(file_path[it]);
        }
    }

    return file_path_copy;
}

std::string amit::common::ToWinFilePath(const std::string& file_path)
{
    std::string file_path_copy;
    file_path_copy.reserve(file_path.size());

    for (size_t it = 0; it < file_path.size(); it++)
    {
        if (file_path[it] == '/')
        {
            file_path_copy.push_back('\\');

            if (file_path[it + 1] == '/')
            {
                while (it < file_path.size() && file_path[it] == '/')
                {
                    it++;
                }
            }
        }
        else
        {
            file_path_copy.push_back(file_path[it]);
        }
    }

    return file_path_copy;
}

std::string amit::common::GetWinDirFromAbsPath(const std::string& file_path)
{
    std::string ret_string(file_path.cbegin(), file_path.cbegin() + file_path.find_last_of('\\'));
    return ret_string;
}

std::string amit::common::GetUnixDirFromAbsPath(const std::string& file_path)
{
    std::string ret_string(file_path.cbegin(), file_path.cbegin() + file_path.find_last_of("/"));
    return ret_string;
}

#ifndef CORE_COMMON_STRING_UTIL_H
#define CORE_COMMON_STRING_UTIL_H

#include <stack>
#include <string>

namespace amit::common
{
    [[nodiscard]] std::string RemoveString(const std::string& remove_from,
                                           const std::string& remove_with,
                                           bool               beginning = true);

    [[nodiscard]] std::string ReverseEachWordsInString(std::string sentence, char separator = ' ');

    [[nodiscard]] std::string ReverseOnlyLetters(std::string s);

    bool IsLetter(char ch);

    [[nodiscard]] std::string RemoveDuplicates(const std::string& s);

    [[nodiscard]] size_t LengthOfLongestSubstring(const std::string& str);

    [[nodiscard]] std::string SimplifyPath(const std::string& path);

    [[nodiscard]] std::string MakeGood(const std::string& s);

    [[nodiscard]] std::string ToUnixFilePath(const std::string& file_path);

    [[nodiscard]] std::string ToWinFilePath(const std::string& file_path);

    [[nodiscard]] std::string GetWinDirFromAbsPath(const std::string& file_path);

    [[nodiscard]] std::string GetUnixDirFromAbsPath(const std::string& file_path);

    [[nodiscard]] inline std::wstring ToWideString(const std::string& in);

}  // namespace amit::common

#endif
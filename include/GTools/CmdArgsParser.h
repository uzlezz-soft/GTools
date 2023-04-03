#ifndef GTOOLS_COMMANDLINEPARSER_H
#define GTOOLS_COMMANDLINEPARSER_H

#include "GTools/Common.h"
#include <ios>
#include <string>
#include <vector>

namespace GTools
{

    struct CmdArgDefinition
    {
        String Name;
        String ShortName;
        String Description;

        CmdArgDefinition(ConstString name, ConstString shortName, ConstString description)
        {
            Name = name;
            ShortName = shortName;
            Description = description;
        }
    };

    struct CmdArgsParserSettings
    {
        std::ostream& OutputStream;

        bool AllowOnlySpecifiedOptions = false;
        bool AllowArbitraryValues = true;
        String FullOptionPrefix = "--";
        String ShortOptionPrefix = "-";

        std::vector<CmdArgDefinition> Arguments;

        CmdArgsParserSettings();

        CmdArgsParserSettings& AddArg(ConstString name, ConstString shortName, ConstString description)
        {
            Arguments.emplace_back(name, shortName, description);
            return *this;
        }
    };

    struct CmdArg
    {
        bool HasValue = false;
        bool HasName = true;
        String Name;
        String Value;
    };

    class CmdArgsParser
    {
    public:

        CmdArgsParser(const CmdArgsParserSettings& settings);

        bool Parse(int argc, char** argv);

        const std::vector<CmdArg>& GetArgs() const { return m_arguments; }

        const CmdArg& GetArg(ConstString name) const;

        bool operator[](uint64_t index) const { return index < m_arguments.size(); }
        bool operator[](ConstString name) const;

    private:

        const CmdArgsParserSettings& m_settings;

        std::vector<CmdArg> m_arguments;

        void PrintHelpSummary(char** argv);
        void PrintErrorMessage(char* arg, char** argv);

    };

}

#endif
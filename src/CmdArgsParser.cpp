#include "GTools/CmdArgsParser.h"
#include <iostream>

using GTools::CmdArgsParserSettings;
using GTools::CmdArgsParser;

CmdArgsParserSettings::CmdArgsParserSettings()
    : OutputStream(std::cout)
{
}

static bool StartsWith(char* arg, GTools::ConstString prefix)
{
    for (uint64_t i = 0; arg[i] && prefix.size() > i; ++i)
    {
        if (arg[i] != prefix[i])
            return false;
    }

    return true;
}

CmdArgsParser::CmdArgsParser(const CmdArgsParserSettings& settings)
    : m_settings(settings)
{
}

bool CmdArgsParser::Parse(int argc, char** argv)
{
    for (int i = 1; i < argc; ++i)
    {
        char* arg = argv[i];

        if (StartsWith(arg, m_settings.FullOptionPrefix))
        {
            arg += 2;
            if (std::strcmp(arg, "help") == 0)
            {
                PrintHelpSummary(argv);
                return false;
            }

            const CmdArgDefinition* definition = nullptr;
            for (auto& def : m_settings.Arguments)
            {
                if (def.Name == arg)
                {
                    definition = &def;
                    break;
                }
            }

            if (m_settings.AllowOnlySpecifiedOptions && !definition)
            {
                PrintErrorMessage(arg, argv);
                return false;
            }

            CmdArg cmdArg;
            
            if (++i < argc)
            {
                if (StartsWith(argv[i], m_settings.FullOptionPrefix) || StartsWith(argv[i], m_settings.ShortOptionPrefix))
                {
                    cmdArg.HasValue = false;
                    cmdArg.Name = definition ? definition->Name : arg;
                    --i;
                }
                else
                {
                    cmdArg.HasValue = true;
                    cmdArg.Name = definition ? definition->Name : arg;
                    cmdArg.Value = argv[i];
                }
            }
            else
            {
                cmdArg.HasValue = false;
                cmdArg.Name = definition ? definition->Name : arg;
            }

            m_arguments.emplace_back(cmdArg);
        }
        else if (StartsWith(arg, m_settings.ShortOptionPrefix))
        {
            ++arg;
            if (std::strcmp(arg, "help") == 0)
            {
                PrintHelpSummary(argv);
                return false;
            }

            const CmdArgDefinition* definition = nullptr;
            for (auto& def : m_settings.Arguments)
            {
                if (def.ShortName == arg)
                {
                    definition = &def;
                    break;
                }
            }

            if (m_settings.AllowOnlySpecifiedOptions && !definition)
            {
                PrintErrorMessage(arg, argv);
                return false;
            }

            CmdArg cmdArg;
            
            if (++i < argc)
            {
                if (StartsWith(argv[i], m_settings.FullOptionPrefix) || StartsWith(argv[i], m_settings.ShortOptionPrefix))
                {
                    cmdArg.HasValue = false;
                    cmdArg.Name = definition ? definition->Name : arg;
                    --i;
                }
                else
                {
                    cmdArg.HasValue = true;
                    cmdArg.Name = definition ? definition->Name : arg;
                    cmdArg.Value = argv[i];
                }
            }
            else
            {
                cmdArg.HasValue = false;
                cmdArg.Name = definition ? definition->Name : arg;
            }

            m_arguments.emplace_back(cmdArg);
        }
        else
        {
            if (!m_settings.AllowArbitraryValues)
            {
                m_settings.OutputStream << "Error: arbitrary values not supported (`" << arg << "`)\n";
            }

            CmdArg cmdArg;
            cmdArg.HasName = false;
            cmdArg.HasValue = true;
            cmdArg.Value = arg;

            m_arguments.emplace_back(cmdArg);
        }
    }

    return true;
}

bool CmdArgsParser::operator[](ConstString name) const
{
    for (auto& arg : m_arguments)
    {
        if (arg.Name == name)
            return true;
    }

    return false;
}

const GTools::CmdArg& CmdArgsParser::GetArg(ConstString name) const
{
    for (auto& arg : m_arguments)
    {
        if (arg.Name == name)
            return arg;
    }
}

void CmdArgsParser::PrintHelpSummary(char** argv)
{
    m_settings.OutputStream << argv[0] << " <parameters...>\n";

    for (auto& arg : m_settings.Arguments)
    {
        m_settings.OutputStream << "    --" << arg.Name;
        if (arg.ShortName.size() > 0)
        {
            m_settings.OutputStream << " [-" << arg.ShortName << "]";
        }

        m_settings.OutputStream << "                    " << arg.Description << "\n";
    }
}

void CmdArgsParser::PrintErrorMessage(char* arg, char** argv)
{
    m_settings.OutputStream << "Error: argument with name `" << arg << "` not found in allowed arguments.\nWrite `" 
    << argv[0] << " --help` to get list of known arguments.\n";
}
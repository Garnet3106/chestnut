/* 
 * 
 * Chestnut 0.0.0
 * 
 * GitHub Repository:
 *     https://github.com/Garnet3106/chestnut/
 * 
 * Copyright © 2020 Garnet3106 All rights reserved. 
 * 
 */


#pragma once


// class Options


typedef std::function<void(ches::PropMap&, ches::PropMap&)> cmd_proc;
typedef std::unordered_map<std::string, cmd_proc>           cmd_proc_map;


namespace ches {
    enum CommandErrorType {
        CommandError_DuplicatedOptionName,
        CommandError_InvalidArgument,
        CommandError_UnknownSubCommand
    };


    class CommandError : std::exception {
    public:
        CommandErrorType type;

        CommandError();

        CommandError(CommandErrorType type);
    };


    class Command {
    public:
        std::vector<std::string> args;
        std::string name;
        PropMap options;
        cmd_proc_map procMap;

        bool usedDefaultName = false;

        PropMap settings;

        Command();

        // excep: CommandError
        Command(int argc, char* argv[], std::string defaultCmdName = "");

        inline void addProc(std::string name, cmd_proc proc) {
            this->procMap[name] = proc;
        }

        inline bool existsOptName(std::string optName) {
            return this->options.count(optName) == 1;
        }

        // excep: CommandError
        void run();

    private:
        std::string getCmdName(std::string defaultCmdName);

        PropMap getCmdOptions();
    };
}

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>
#include <cstring>
#include <cstdio>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>

using namespace std;

// Default path for executables
vector<string> default_path = { "/bin", "/usr/bin" };

// Function prototypes
void error_prt();
void parse_cmd(const string& line);
void execute_cmd(const string& line);
bool is_builtin_cmd(const string& para);
void execute_builtin_cmd(const vector<string>& paras);
void execute_external_cmd(const vector<string>& paras);
string find_path(const string& para);
void redirect(const string& fpath);


int main(int argc, char* argv[])
{
    string input;
    ifstream batch_file;


    bool is_end = false;
    bool interactive_mode = false;

    if (argc > 2)
    {
        error_prt();
        exit(1);
    }
    else if (argc == 2)
    {
        interactive_mode = false;
        batch_file.open(argv[1]);
        if (!batch_file.is_open())
        {
            error_prt();
            exit(1);
        }

    }
    else
    {
        interactive_mode = true;
    }

    istream& input_stream = interactive_mode ? cin : batch_file;

    while (!is_end)
    {
        if (interactive_mode)
        {
            cout << "wish> ";
        }

        if (!getline(input_stream, input))
        {
            // End of file or failure
            is_end = true;
        }

        if (!input.empty())
        {
            parse_cmd(input);
        }
    }

    return 0;
}


// print error
void error_prt() {
    char error_message[30] = "An error has occurred\n";
    write(STDERR_FILENO, error_message, strlen(error_message));
}

void parse_cmd(const string& line) {
    // check if is parallel
    bool is_parallel = false;
    for (std::size_t i = 0; i < line.size(); ++i)
    {
        if (line[i] == '&')
            is_parallel = true;
    }

    if (is_parallel)
    {
        istringstream ss(line);
        string str;
        vector<string> cmds;

        while (getline(ss, str, '&'))
        {
            if (!str.empty())
            {
                cmds.push_back(str);
            }
        }

        vector<int> process;
        for (const auto& cmd : cmds)
        {
            pid_t pid = fork();
            if (pid == 0) // child
            {
                execute_cmd(cmd);
                exit(0);
            }
            else if (pid > 0) // parent
            {
                process.push_back(pid);
            }
            else
            {
                error_prt();
            }

        }
        for (auto& p : process)
        {
            waitpid(p, nullptr, 0);
        }

    }
    else
    {
        execute_cmd(line);
    }

}

void execute_cmd(const string& line)
{
    // check if is redirection
    bool is_redirection = false;
    string format_line = "";
    for (std::size_t i = 0; i < line.size(); ++i)
    {
        if (line.at(i) == '>')
        {
            is_redirection = true;
            if (i - 1 >= 0 && !isspace(line.at(i - 1)))
                format_line += " ";
            format_line += line.at(i);
            if (i + 1 < line.size() && !isspace(line.at(i + 1)))
                format_line += " ";
        }
        else
        {
            format_line += line.at(i);
        }

    }

    istringstream ss(format_line);
    string str;
    vector<string> paras;

    while (getline(ss, str, ' '))
    {
        if (!str.empty())
        {
            paras.push_back(str);
        }
    }

    if (!paras.empty())
    {
        if (is_builtin_cmd(paras[0]))
        {
            execute_builtin_cmd(paras);
        }
        else
        {
            pid_t pid = fork();
            if (pid == 0) // child
            {
                if (is_redirection)
                {
                    std::size_t pos = paras.size();
                    for (std::size_t i = 0; i < paras.size(); ++i)
                    {
                        if (paras.at(i) == ">")
                        {
                            pos = i;
                            break;
                        }
                    }

                    if (pos != paras.size() && pos != 0 && pos == paras.size() - 2)
                    {
                        redirect(paras.at(paras.size() - 1));
                        vector<string> new_paras(paras.begin(), paras.begin() + pos);

                        execute_external_cmd(new_paras);

                    }
                    else
                    {
                        error_prt();
                    }
                }
                else
                {
                    execute_external_cmd(paras);
                }
                exit(0);
            }
            else if (pid > 0) // parent
            {
                waitpid(pid, nullptr, 0);
            }
            else
            {
                error_prt();
            }

        }
    }

}

bool is_builtin_cmd(const string& para)
{
    return para == "exit" || para == "cd" || para == "path";
}

void execute_builtin_cmd(const vector<string>& paras)
{
    string key = paras[0];
    int len = paras.size();
    if (key == "exit")
    {
        if (len > 1)
        {
            error_prt();
        }
        else
        {
            exit(0);
        }
    }
    else if (key == "cd")
    {
        if (len != 2 || chdir(paras[1].c_str()) != 0)
        {
            error_prt();
        }
    }
    else if (key == "path")
    {
        default_path.clear();
        for (int i = 1; i < len; ++i)
        {
            default_path.push_back(paras[i]);
        }
    }
    else
    {
        error_prt();
        exit(2);
    }
}

void execute_external_cmd(const vector<string>& paras)
{
    // Searching in the specified path
    string exec_path = find_path(paras[0]);

    if (exec_path.empty())
    {
        error_prt();
    }
    else
    {
        vector<char*> exec_paras;
        for (const auto& para : paras)
        {
            exec_paras.push_back(const_cast<char*>(para.c_str()));

        }
        exec_paras.push_back(nullptr);

        execv(exec_path.c_str(), exec_paras.data());
        error_prt();
        exit(1);

    }

}

string find_path(const string& para)
{
    for (const auto& dir : default_path)
    {
        string exec_path = dir + "/" + para;
        if (access(exec_path.c_str(), X_OK) == 0)
        {
            return exec_path;
        }
    }

    return "";
}

void redirect(const string& fpath)
{
    int fp = open(fpath.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0777);
    if (fp < 0)
    {
        error_prt();
        exit(1);
    }


    dup2(fp, STDOUT_FILENO);

    close(fp);
}




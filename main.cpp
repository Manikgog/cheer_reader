#include <iomanip>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <sstream>

namespace reader_cheer {
    static const std::string read_command = "READ";
    static const std::string cheer_command = "CHEER";

    std::unordered_map<size_t, size_t> user_page; // user -> last_page
    std::vector<size_t> page_counts(1001, 0);  // page -> count of users

    void ProcessRead(const int user, const int page) {
        if (user_page.contains(user)) {
            const size_t prev_page = user_page[user];
            page_counts[prev_page]--;
        }
        user_page[user] = page;
        page_counts[page]++;
    }

    void ProcessCheer(const int user) {
        if (!user_page.contains(user)) {
            std::cout << std::setprecision(6) << 0. << '\n';
            return;
        }

        const size_t total_users = user_page.size();
        if (total_users == 1) {
            std::cout << std::setprecision(6) << 1. << '\n';
            return;
        }

        const size_t current_page = user_page[user];
        int less_read = 0;
        for (int p = 1; p < current_page; ++p) {
            less_read += page_counts[p];
        }

        const double ratio = static_cast<double>(less_read) / static_cast<double>(total_users - 1);
        std::cout << std::setprecision(6) << ratio << '\n';
    }

    void ParseCommand(const std::string& command) {
        std::istringstream iss(command);
        std::string cmd;
        iss >> cmd;

        if (cmd == read_command) {
            int user, page;
            iss >> user >> page;
            ProcessRead(user, page);
        } else if (cmd == cheer_command) {
            int user;
            iss >> user;
            ProcessCheer(user);
        }
    }
}

void ReadInput(std::istream& in_stream) {
    size_t command_count;
    in_stream >> command_count;
    std::string dummy;
    std::getline(in_stream, dummy);

    for (size_t i = 0; i < command_count; ++i) {
        std::string command;
        std::getline(in_stream, command);
        if (!command.empty()) {
            reader_cheer::ParseCommand(command);
        }
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    ReadInput(std::cin);
    return 0;
}
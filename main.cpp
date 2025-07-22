#include <iomanip>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <sstream>
#include "log_duration.h"

namespace reader_cheer {

    static const std::string READ_COMMAND = "READ";
    static const std::string CHEER_COMMAND = "CHEER";
    static const size_t MAX_PAGE_COUNT = 1000;

    class BookReader {
    public:
        BookReader()
            : page_to_user_count_(MAX_PAGE_COUNT + 1) {}

        void ExecuteCommand(const std::string& cmd, const std::vector<std::string>& args) {
            if (cmd == READ_COMMAND && args.size() == 2) {
                size_t user = std::stoi(args[0]);
                size_t page = std::stoi(args[1]);
                Read(user, page);
            }
            else if (cmd == CHEER_COMMAND && args.size() == 1) {
                size_t user = std::stoi(args[0]);
                std::cout << std::setprecision(6) << Cheer(user) << "\n";
            }
        }

    private:

        void Read(size_t user, size_t page) {
            if (user_to_last_page_.contains(user)) {
                const size_t prev_page = user_to_last_page_[user];
                page_to_user_count_[prev_page]--;
            }
            user_to_last_page_[user] = page;
            page_to_user_count_[page]++;
        }

        double Cheer(size_t user) const {
            if (!user_to_last_page_.contains(user)) {
                return 0.;
            }

            const size_t total_users = user_to_last_page_.size();
            if (total_users == 1) {
                return 1.;
            }

            const size_t current_page = user_to_last_page_.at(user);
            int less_read = 0;
            for (int p = 1; p < current_page; ++p) {
                less_read += page_to_user_count_[p];
            }

            const double ratio = static_cast<double>(less_read) / static_cast<double>(total_users - 1);
            return ratio;
        }

        std::unordered_map<size_t, size_t> user_to_last_page_;
        std::vector<size_t> page_to_user_count_;
    };
}

void ProcessInput(std::istream& in_stream) {
    size_t command_count;
    if (!(in_stream >> command_count)) {
        throw std::runtime_error("Invalid command count");
    }
    std::string dummy;
    std::getline(in_stream, dummy);
    reader_cheer::BookReader reader;

    for (size_t i = 0; i < command_count; ++i) {
        std::string line;
        if (!std::getline(in_stream, line) || line.empty()) {
            continue;
        }
        std::istringstream iss(line);
        std::string cmd;
        iss >> cmd;
        std::vector<std::string> args;
        std::string arg;
        while (iss >> arg) {
            args.push_back(arg);
        }
        reader.ExecuteCommand(cmd, args);
    }
}

int main() {
    LOG_DURATION("main");
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    ProcessInput(std::cin);
    return 0;
}
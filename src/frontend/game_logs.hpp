#ifndef GAME_LOGS_HPP
#define GAME_LOGS_HPP

#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <iostream>

/**
 * @brief Keeps text logs for the game.
 */
class Game_Logs {
public:
    Game_Logs(size_t msg_count) : log_count_(msg_count) {}

    bool show_logs = true;

    /**
     * @brief adds logs to logs_.
     */
    void add_logs(const std::string& logs) {
        auto i = logs.begin();
        auto j = logs.begin();
        while (i != logs.end()) {
            j = std::find(i,logs.end(),'\n');
            std::string log_msg(i,j);
            logs_.push_back(log_msg);
            i = j;
            if (i != logs.end()) i++;
        }
        reset_log_start();
        return;
    }

    /**
     * @brief Changes start point from where the logs start printing.
     */
    void change_start(int amount) {
        if (int(logs_from_) + amount < 0 || logs_from_ + amount >= int(logs_.size()) - log_count_ || logs_.size() < log_count_) return;
        logs_from_ = logs_from_ + amount;
        return;
    }

    /**
     * @brief Returns logs from logs_begin to logs_end.
     */
    std::string get_logs() {
        std::stringstream out;
        auto start = logs_begin();
        auto end = logs_end();
        while (start != end) {
            out << *start << "\n";
            start++;
        }
        return out.str();
    }

private:
    std::vector<std::string> logs_;
    size_t log_count_;
    size_t logs_from_ = 0;

    void reset_log_start() {
        size_t current_log_size = logs_.size();
        if (current_log_size < log_count_) return;
        logs_from_ = current_log_size - log_count_;
        return;
    }

    std::vector<std::string>::const_iterator logs_begin() const {
        auto it = logs_.begin();
        size_t it_count = 0;
        while (it != logs_.end() && it_count < logs_from_) {
            it++;
            it_count++;
        }
        return it;
    }

    std::vector<std::string>::const_iterator logs_end() const {
        auto it = logs_begin();
        size_t it_count = 0;
        while (it != logs_.end() && it_count < log_count_) {
            it++;
            it_count++;
        }
        return it;
    }
};

#endif
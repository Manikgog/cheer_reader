#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int Q;
    cin >> Q;

    vector<int> user_pages(1e5 + 1, 0); // Индексы - ID пользователей, значения - последняя прочитанная страница
    vector<int> page_counts(1001, 0);   // Индексы - номера страниц, значения - сколько пользователей дочитали до этой страницы

    int total_users = 0;

    while (Q--) {
        string command;
        cin >> command;

        if (command == "READ") {
            int user, page;
            cin >> user >> page;

            if (user_pages[user] == 0) {
                total_users++;
            } else {
                page_counts[user_pages[user]]--;
            }

            user_pages[user] = page;
            page_counts[page]++;
        }
        else if (command == "CHEER") {
            int user;
            cin >> user;

            int current_page = user_pages[user];
            if (current_page == 0) {
                cout << "0\n";
                continue;
            }

            if (total_users == 1) {
                cout << "1\n";
                continue;
            }

            int less_read = 0;
            for (int i = 1; i < current_page; ++i) {
                less_read += page_counts[i];
            }

            double fraction = static_cast<double>(less_read) / (total_users - 1);
            cout << setprecision(6) << fixed << fraction << '\n';
        }
    }

    return 0;
}
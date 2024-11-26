#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <random>
#include <chrono>
#include <thread>
#include <tgbot/tgbot.h>

using namespace std;
using namespace TgBot;

vector<string> loadCompliments(const string& filename) {
    vector<string> compliments;
    ifstream file(filename);
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            compliments.push_back(line);
        }
        file.close();
    }
    else {
        cerr << "Error opening file: " << filename << endl;
    }
    return compliments;
}

void sendTelegramMessage(Bot& bot, long long chat_id, const string& message) {
    try {
        bot.getApi().sendMessage(chat_id, message);
    }
    catch (TgException& e) {
        cerr << "Error sending Telegram message: " << e.what() << endl;
    }
}

void sendRandomCompliment(Bot& bot, long long chat_id, const vector<string>& compliments) {
    if (compliments.empty()) return;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(0, compliments.size() - 1);
    sendTelegramMessage(bot, chat_id, compliments[distrib(gen)]);
}


int main() {
    string botToken = "7836126099:AAHk75KQAafDya1tTVY_A0j6LbSLtgQ-EBY"; 
    long long groupId = -4598658636; 

    Bot bot(botToken);
    auto compliments = loadCompliments("Slova.txt");


    bot.getEvents().onCommand("love", [&bot, &compliments](Message::Ptr message) {
        sendRandomCompliment(bot, message->chat->id, compliments);
        });


    while (true) {
        sendRandomCompliment(bot, groupId, compliments);
        this_thread::sleep_for(chrono::minutes(10));
    }

    return 0;
}


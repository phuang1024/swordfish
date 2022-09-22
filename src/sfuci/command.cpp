#include <sstream>

#include "sfuci.hpp"
#include "sfutils.hpp"


UCICommand::UCICommand(std::istream& is) {
    std::string line;
    if (!std::getline(is, line))
        mode = "quit";

    std::istringstream iss(line);
    std::string word;

    if (std::getline(iss, word, ' '))
        mode = word;
    else
        return;

    if (mode == "position") {
        // Set up position.
        std::getline(iss, word, ' ');
        if (word == "startpos") {
            pos.setup_std();
        } else if (word == "fen") {
            std::string fen;
            for (int i = 0; i < 6; i++) {
                std::getline(iss, word, ' ');
                fen += word + " ";
            }
            pos.setup_fen(fen);
        }

        if (std::getline(iss, word, ' ') && word == "moves") {
            while (std::getline(iss, word, ' ')) {
                const Move m(word);
                pos.push(m);
            }
        }
    } else {
        // Other args.
        while (std::getline(iss, word, ' ')) {
            std::string word2;
            if (std::getline(iss, word2, ' ')) {
                args[word] = std::stoi(word2);
            } else {
                args[word] = 1;
            }
        }
    }
}

#include "../../includes/Manager.hpp"

void Manager::modeAction(Client &client) {
	if (Parser::modeParse(client)) {
		std::vector<std::string> command = client.getCommand();
		std::string channelName;
		std::string flag = "";
		std::string arg = "";
		channelName = command[1].substr(0, command[1].find(" "));
		flag = command[1].substr(command[1].find(" ") + 1, command[1].size());
		if (flag[1] == 'i') {
			_channels.find(channelName)->second.setModeI(flag[0] != '-');
		}
		else if (flag[1] == 't') {
			_channels.find(channelName)->second.setModeT(flag[0] != '-');
		}
		else if (flag[1] == 'k') {
			if (flag[0] == '+')
				arg = flag.substr(flag.find(" ") + 1, flag.size());
			_channels.find(channelName)->second.setKey(arg);
		}
		else if (flag[1] == 'o') {
			arg = flag.substr(flag.find(" ") + 1, flag.size());
			int target = Manager::getIDbyNick(arg);
			if (flag[0] == '+')
				_channels.find(channelName)->second.addClientToOp(target);
			else
				_channels.find(channelName)->second.removeOp(target);
			// add broadcast user list update
		}
		else if (flag[1] == 'l') {
			arg = flag.substr(flag.find(" ") + 1, flag.size());
			if (flag[0] == '+')
				_channels.find(channelName)->second.setModeL(atoi(arg.c_str()));
			else
				_channels.find(channelName)->second.setModeL(0);
		}
	}
}
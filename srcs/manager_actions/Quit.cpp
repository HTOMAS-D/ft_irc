#include "../../includes/Manager.hpp"

void Manager::removeClientFromChannels(Client &client) {
	for(int i = 0; i < (int)_channelNames.size(); i++){
		std::string channelName = _channelNames[i];
        if (_channels.find(channelName) != _channels.end() && _channels.find(channelName)->second.checkClient(client.getId())) {
			_channels.find(channelName)->second.removeClient(client.getId());
			_channels.find(channelName)->second.channelMessage(formatMessage(client) + " QUIT: has quit\r\n");
		}
    }
}

void Manager::quitAction(Client &client) {
	int id = client.getId();
	removeClientFromChannels(client);
	sendIrcMessage(id, formatMessage(client) + " QUIT :Bye!");
	removeClient(id);
}

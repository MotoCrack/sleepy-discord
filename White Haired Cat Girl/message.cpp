#include "message.h"
namespace SleepyDiscord {
	Message::Message() {

	}

	Message::~Message() {
		delete[] mentions;
		delete[] embeds;
		delete[] attachments;
		delete[] mention_roles;
	}

	Message::Message(DiscordClient* _client, JSON * jsonMessage) {
		client = _client;
		DiscordObject::fillOut(jsonMessage);
	}

	Message::Message(JSON_object * messageJSONObject) {
		DiscordObject::fillOut(messageJSONObject);
	}

	void Message::fillOut(JSON_object * _JSON_object) {
		DiscordObject::fillOut(_JSON_object);
	}

	void Message::fillOut(const char* name, void * value) {
		switch (name[0]) {
		case 't':
			switch (name[1]) {
			case 'i': timestamp = (char*)value; break;
			case 't': tts = (bool*)value; break;
			case 'y': type = (double*)value; break;
			default: break;
			} break;
		case 'p': pinned = (bool*)value; break;
		case 'n':
			try { nonce = boost::lexical_cast<uint64_t>((char*)value); }
			catch (const boost::bad_lexical_cast &) { nonce = 0; }
			break;
		case 'm':
			switch (name[8]) {
			case 0: //mentions
				/*JSON_array* _array = (JSON_array*)value;
				user* _mentions = new user[_array->count];
				for (unsigned int i = 0; i < _array->count; i++) {
					_mentions[i].fillOut((JSON_object*)JSON_accessArray(_array, i));
				}
				mentions = _mentions;
				numberOfMentions = _array->count;*/
				mentions = fillOutArray<User>(value, numOfMentions); break;
			case 'r': {		//add a lambda to the fillOutArray function to remove this mess
				JSON_array* _array = (JSON_array*)value;
				mention_roles = new std::string[_array->count];
				for (unsigned int i = 0; i < _array->count; i++)
					mention_roles[i] = (char*)JSON_accessArray(_array, i);
				numOfmention_roles = _array->count;
			} break;
			case 'e': mention_everyone = (bool*)value; break;
			default: break;
			} break;
		case 'i': id = (char*)value; break;
			case 'e':
				switch (name[1]) {
				case 'm': embeds = fillOutArray<Embed>(value, numOfEmbeds);
				case 'd': edited_timestamp = (char*)value; break;
				default: break;
				}
		case 'c':
			switch (name[1]) {
			case 'o': content = (char*)value; break;
			case 'h': channel_id = (char*)value; break;
			} break;
		case 'a':
			switch (name[1]) {
			case 'u': author.fillOut((JSON_object*)value); break;
			case 't': attachments = fillOutArray<Attachment>(value, numOfAttachments); break;
			default: break;
			} break;
		default: break;
		}
	}

	bool Message::startsWith(char* test) {
		return strncmp(content.c_str(), test, strlen(test)) == 0;
	}

	int Message::length() {
		return content.length();	//if this doesn't work there's a stringLength function in common.h
	}

	bool Message::isMentioned(const std::string id) {
		for (unsigned int i = 0; i < numOfMentions; i++)
			if (mentions[i].id  == id) return true;
		return false;
	}

	bool Message::isMentioned(User& _user) {
		return isMentioned(_user.id);
	}

	void Message::reply(std::string message) {
		client->sendMessage(channel_id, message);
	}
}
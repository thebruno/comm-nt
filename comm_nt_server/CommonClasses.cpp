#include "stdafx.h"
#include "CommonClasses.h"


std::string ToString(MessageType type) {
	switch (type) {
		case MessageType::LOGIN:
			return "LOGIN";
		case MessageType::LOGOUT:
			return "LOGOUT";
		case MessageType::RESULT:
			return "RESULT";			
		case MessageType::USERLIST:
			return "USERLIST";
		case MessageType::MESSAGE:
			return "MESSAGE";
		case MessageType::GROUPMESSAGE:
			return "GROUPMESSAGE";
		default:
			return "MESSAGE_TYPE_NOTSET";
	}
}

MessageType ToMessageType(std::string s) {
	if (strcmp(s.c_str(), "LOGIN") == 0) 
		return MessageType::LOGIN;
	else if (strcmp(s.c_str(), "LOGOUT") == 0) 
		return MessageType::LOGOUT;
	else if (strcmp(s.c_str(), "RESULT") == 0) 
		return MessageType::RESULT;
	else if (strcmp(s.c_str(), "USERLIST") == 0) 
		return MessageType::USERLIST;
	else if (strcmp(s.c_str(), "MESSAGE") == 0) 
		return MessageType::MESSAGE;
	else if (strcmp(s.c_str(), "GROUPMESSAGE") == 0) 
		return MessageType::GROUPMESSAGE;
	else 
		return MessageType::MESSAGE_TYPE_NOTSET;
}


std::string ToString(Result type) {
	switch (type) {
		case Result::OK:
			return "OK";
		case Result::FAILED:
			return "FAILED";
		case Result::DISCONNECTED:
			return "DISCONNECTED";
		case Result::EMPTY:
			return "EMPTY";			
		case Result::NOTEMPTY:
			return "NOTEMPTY";
		case Result::TIMEOUT:
			return "TIMEOUT";
		default:
			return "RESULT_NOTSET";
	}
}

Result ToResult(std::string s) {
	if (strcmp(s.c_str(), "OK") == 0) 
		return Result::OK;
	else if (strcmp(s.c_str(), "FAILED") == 0) 
		return Result::FAILED;
	else if (strcmp(s.c_str(), "DISCONNECTED") == 0) 
		return Result::DISCONNECTED;
	else if (strcmp(s.c_str(), "EMPTY") == 0) 
		return Result::EMPTY;
	else if (strcmp(s.c_str(), "NOTEMPTY") == 0) 
		return Result::NOTEMPTY;
	else if (strcmp(s.c_str(), "TIMEOUT") == 0) 
		return Result::TIMEOUT;
	else 
		return Result::RESULT_NOTSET;
}


User::User(std::string login, std::string ip): Login(login), IP(ip) {
}

User::User(){
}

User::~User() {
}

std::string User::ToString() {
	std::string toReturn;
	toReturn.append(this->Login);
	toReturn += MESSAGE_USER_SEPARATOR;
	toReturn.append(this->IP);
	return toReturn;
}

Result User::Parse(std::string& s) {
	std::vector<std::string> splitString = split(s, MESSAGE_USER_SEPARATOR);
	if (splitString.size() < 2 || splitString[0] == "" || splitString[1] == "")
		return Result::FAILED;
	this->Login = splitString[0];
	this->IP = splitString[1];
	return Result::OK;
}

bool User::operator==(const User& u) const {
	if (u.Login == this->Login && u.IP == this->IP)
		return true;
	else 
		return false;

}

std::string Group::ToString(){
	std::string toReturn;
	std::list<User>::iterator it;
	for (it = GroupMembers.begin(); it != GroupMembers.end(); it++) {
		toReturn.append((*it).ToString());
		toReturn += MESSAGE_GROUP_USER_SEPARATOR;
	}
	toReturn = toReturn.substr(0, toReturn.size() - 1);
	return toReturn;
}

Result Group::Parse(std::string &s){
	User user;
	std::vector<std::string> splitUsers = split(s, MESSAGE_GROUP_USER_SEPARATOR);
	if (splitUsers.empty())
		return Result::FAILED;
	std::vector<std::string>::iterator it;
	for (it = splitUsers.begin(); it != splitUsers.end(); it++) {
		user.Parse(*it);
		GroupMembers.push_back(user);
	}
	return Result::OK;
}

bool Group::operator==(const Group &g) const {
	if (GroupMembers.size() != g.GroupMembers.size())
		return false;
	std::list<User>::const_iterator it1, it2;
	bool found = false;
	for (it1 = GroupMembers.begin(); it1 != GroupMembers.end(); it1++) {
		for (it2 = g.GroupMembers.begin(); it2 != g.GroupMembers.end(); it2++) {
			if (*it1 == *it2) {
				found = true;
				break;
			}
		}
		if (!found)
			return false;
		else 
			found = false;
	}
	return true;
}



Message::Message(MessageType type, User sender, Group recipients, std::string text):
	Type(type), Sender(sender), Recipients(recipients), Text(text){
}

Message::Message(){}

std::string Message::ToString(){
	std::string toReturn;
	toReturn.append(::ToString(Type));
	toReturn += MESSAGE_SEPARATOR;
	switch (Type){
		case MessageType::MESSAGE_TYPE_NOTSET:
			return "NOTSET";
		case MessageType::LOGIN:
			return "LOGIN";
		case MessageType::LOGOUT:
			return "LOGOUT";
		case MessageType::RESULT:
			return "RESULT";			
		case MessageType::USERLIST:
			return "USERLIST";
		case MessageType::MESSAGE:
			return "MESSAGE";
		case MessageType::GROUPMESSAGE:
			return "GROUPMESSAGE";
		default:
			return "";
	}
	toReturn.append(Sender.ToString());
	toReturn += MESSAGE_SEPARATOR;
	toReturn.append(Recipients.ToString());
	toReturn += MESSAGE_SEPARATOR;
	toReturn.append(Text);
	return toReturn;
}

Result Message::Parse(std::string &s){
	User user;
	std::vector<std::string> splitFields = split(s, MESSAGE_SEPARATOR);
	if (splitFields.empty())
		return Result::FAILED;
	Type = ToMessageType(splitFields[0]);

	// parsowanie messaga wg typu
	// je�li wiadomo�ci b�d� sie skleja�, to trzeba trzeba doda� separator wiadomo�ci
	switch (Type)
	{
		case MessageType::MESSAGE_TYPE_NOTSET:
		//	return "NOTSET";
		case MessageType::LOGIN:
		//	return "LOGIN";
		case MessageType::LOGOUT:
		//	return "LOGOUT";
		case MessageType::RESULT:
		//	return "RESULT";			
		case MessageType::USERLIST:
		//	return "USERLIST";
		case MessageType::MESSAGE:
		//	return "MESSAGE";
		case MessageType::GROUPMESSAGE:
		//	return "GROUPMESSAGE";
			;
	}
	Sender.Parse(splitFields[1]);
	Recipients.Parse(splitFields[2]);
	Text = splitFields[3];
	return Result::OK;
}
#include "stdafx.h"
#include "CommonClasses.h"


std::string ToString(MessageType type) {
	switch (type) {
		case LOGIN:
			return "LOGIN";
		case LOGOUT:
			return "LOGOUT";
		case RESULT:
			return "RESULT";			
		case USERLIST:
			return "USERLIST";
		case MESSAGE:
			return "MESSAGE";
		case GROUPMESSAGE:
			return "GROUPMESSAGE";
		default:
			return "MESSAGETYPE_NOTSET";
	}
}

MessageType ToMessageType(std::string s) {
	if (strcmp(s.c_str(), "LOGIN") == 0) 
		return LOGIN;
	else if (strcmp(s.c_str(), "LOGOUT") == 0) 
		return LOGOUT;
	else if (strcmp(s.c_str(), "RESULT") == 0) 
		return RESULT;
	else if (strcmp(s.c_str(), "USERLIST") == 0) 
		return USERLIST;
	else if (strcmp(s.c_str(), "MESSAGE") == 0) 
		return MESSAGE;
	else if (strcmp(s.c_str(), "GROUPMESSAGE") == 0) 
		return GROUPMESSAGE;
	else 
		return MESSAGETYPE_NOTSET;
}


std::string ToString(Result type) {
	switch (type) {
		case OK:
			return "OK";
		case FAILED:
			return "FAILED";
		case DISCONNECTED:
			return "DISCONNECTED";
		case EMPTY:
			return "EMPTY";			
		case NOTEMPTY:
			return "NOTEMPTY";
		case TIMEOUT:
			return "TIMEOUT";
		default:
			return "RESULT_NOTSET";
	}
}

Result ToResult(std::string s) {
	if (strcmp(s.c_str(), "OK") == 0) 
		return OK;
	else if (strcmp(s.c_str(), "FAILED") == 0) 
		return FAILED;
	else if (strcmp(s.c_str(), "DISCONNECTED") == 0) 
		return DISCONNECTED;
	else if (strcmp(s.c_str(), "EMPTY") == 0) 
		return EMPTY;
	else if (strcmp(s.c_str(), "NOTEMPTY") == 0) 
		return NOTEMPTY;
	else if (strcmp(s.c_str(), "TIMEOUT") == 0) 
		return TIMEOUT;
	else 
		return RESULT_NOTSET;
}


User::User(std::string login, std::string ip): Login(login), IP(ip) {
}

User::User(){
}

User::~User() {
}

std::string User::ToString(char separator) {
	std::string toReturn;
	toReturn.append(this->Login);
	toReturn += separator;
	toReturn.append(this->IP);
	return toReturn;
}


Result User::Parse(std::string& s) {
	std::vector<std::string> splitString = split(s, MESSAGE_USER_SEPARATOR);
	if (s.size() == 1 && s[0] == MESSAGE_USER_SEPARATOR) 
		// empty user - allow it
		return OK;
	if (splitString.size() < 2 || splitString[0] == "" || splitString[1] == "")
		return FAILED;
	this->Login = splitString[0];
	this->IP = splitString[1];
	return OK;
}

bool User::operator==(const User& u) const {
    if (u.Login == this->Login && u.IP == this->IP)
            return true;
    else
            return false;

}

// sortby user login
bool User::operator<(const User & u2) const {
    if (this->Login < u2.Login)
        return true;
    return false;
}

std::string Group::ToString(char separator){
	std::string toReturn;
	std::list<User>::iterator it;
	for (it = GroupMembers.begin(); it != GroupMembers.end(); it++) {
		toReturn.append((*it).ToString());
		toReturn += separator;
	}
	toReturn = toReturn.substr(0, toReturn.size() - 1);
	return toReturn;
}

Group::Group(){}

Group::~Group(){}

Group::Group(std::list<User> &u):GroupMembers(u){
	 
}

Result Group::Parse(std::string &s){
	User user;
	std::vector<std::string> splitUsers = split(s, MESSAGE_GROUP_USER_SEPARATOR);
	if (splitUsers.empty())
		return FAILED;
	std::vector<std::string>::iterator it;
	for (it = splitUsers.begin(); it != splitUsers.end(); it++) {
		user.Parse(*it);
		GroupMembers.push_back(user);
	}
	return OK;
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

Message::Message(MessageType type, std::string created, User receiver, MessageType previousOperation, Result previousResult, std::string previusOperationInfo) {
        Type = type;
		Created = created;
        Receiver = receiver;
        PreviousOperation = previousOperation;
        PreviousResult = previousResult;
        PreviusOperationInfo = previusOperationInfo;
}

Message::Message(MessageType type, std::string created, User sender, User receiver, Group involvedGroup, std::string text) {
        Type  = type;
		Created = created;
        Sender = sender;
        Receiver = receiver;
        InvolvedGroup = involvedGroup;
        Text = text;
        PreviousOperation = MESSAGETYPE_NOTSET;
        PreviousResult = RESULT_NOTSET;
}


Message::Message():Type(MESSAGETYPE_NOTSET), PreviousOperation (MESSAGETYPE_NOTSET), PreviousResult (RESULT_NOTSET){
	Created = DateTimeNow();
}
Message::~Message() {}

std::string Message::ToString(char separator){
	std::string toReturn;
	toReturn.append(::ToString(Type));
	toReturn += MESSAGE_SEPARATOR;
	switch (Type){
		case LOGIN: 
		case LOGOUT: {
			toReturn.append(Created);
			toReturn += separator;
			toReturn.append(Sender.ToString());		
			break;
		}
		case RESULT: {
			toReturn.append(Created);
			toReturn += separator;
			toReturn.append(Receiver.ToString());
			toReturn += separator;
			toReturn.append(::ToString(PreviousOperation));
			toReturn += separator;
			toReturn.append(::ToString(PreviousResult));
			toReturn += separator;
			toReturn.append(PreviusOperationInfo);
			break;
		}
		case USERLIST: {
			toReturn.append(Created);
			toReturn += separator;
			toReturn.append(Receiver.ToString());
			toReturn += separator;
			toReturn.append(InvolvedGroup.ToString());
			break;
		}
		case MESSAGE: {
			toReturn.append(Created);
			toReturn += separator;
			toReturn.append(Sender.ToString());
			toReturn += separator;
			toReturn.append(Receiver.ToString());
			toReturn += separator;
			toReturn.append(Text);
			break;
		}
		case GROUPMESSAGE: {
			toReturn.append(Created);
			toReturn += separator;
			toReturn.append(Sender.ToString());
			toReturn += separator;
			//toReturn.append(Receiver.ToString());
			//toReturn += MESSAGE_SEPARATOR;
			toReturn.append(InvolvedGroup.ToString());
			toReturn += separator;
			toReturn.append(Text);
			break;
		}
		default:
			break;
	}

	return toReturn;
}

Result Message::Parse(std::string &s){
	User user;
	std::vector<std::string> splitFields = split(s, MESSAGE_SEPARATOR);
	if (splitFields.empty())
		return FAILED;
	Type = ToMessageType(splitFields[0]);

	// parsowanie messaga wg typu
	// jeœli wiadomoœci bêd¹ sie sklejaæ, to trzeba trzeba dodaæ separator wiadomoœci
	switch (Type) {
		case LOGIN:		
		case LOGOUT: {
			if (splitFields.size() != 3 || (Sender.Parse(splitFields[2]) != OK)) 
					return FAILED;
			Created = splitFields[1];
			break;
		}
		case RESULT: {
			if (splitFields.size() != 6 || 				
				(Receiver.Parse(splitFields[2]) != OK) ||
				(PreviousOperation = ToMessageType(splitFields[3])) == MESSAGETYPE_NOTSET ||
				(PreviousResult = ToResult(splitFields[4])) == RESULT_NOTSET )				
				return FAILED;
			Created = splitFields[1];
			PreviusOperationInfo = splitFields[5];				
			break;
		}
		case USERLIST: {
			if (splitFields.size() != 4 || 
				(Receiver.Parse(splitFields[2]) != OK) ||
				(InvolvedGroup.Parse(splitFields[3]) != OK)) 
					return FAILED;
			Created = splitFields[1];
			break;
		}
		case MESSAGE: {
			if (splitFields.size() != 5 || 
				(Sender.Parse(splitFields[2]) != OK) ||
				(Receiver.Parse(splitFields[3]) != OK)) 
					return FAILED;
			Created = splitFields[1];
			Text = splitFields[4];
			break;
		}
		case GROUPMESSAGE: {
			if (splitFields.size() != 5 || 
				(Sender.Parse(splitFields[2]) != OK) ||
				// omit receiver field - it is used only on server
				//(Receiver.Parse(splitFields[2]) != OK) ||
				(InvolvedGroup.Parse(splitFields[3]) != OK)) 
					return FAILED;
				Created = splitFields[1];
				Text = splitFields[4];
			break;
		}
		default:
			return FAILED;
	}
	return OK;
}


std::string DateTimeNow(){
	std::stringstream toReturn;
	SYSTEMTIME st;
    GetSystemTime(&st);
	toReturn << st.wYear << "-" 
		<< std::setw(2) << std::setfill('0') << st.wMonth << "-"
		<< std::setw(2) << std::setfill('0') << st.wDay << " " 
                << std::setw(2) << std::setfill('0') << (st.wHour + 1)<< ":"
		<< std::setw(2) << std::setfill('0') << st.wMinute << ":"
		<< std::setw(2) << std::setfill('0') << st.wSecond;    
	return toReturn.str();
}

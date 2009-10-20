#ifndef COMMON_CLASSES_H
#define COMMON_CLASSES_H
#include "stdafx.h"
#include "Utilities.h"


class Serializable{
	virtual std::string ToString() = 0;
	virtual Result Parse(std::string &s) = 0;
};


#define MESSAGE_DELIMITER (char)0xFF // add at the end of every message while sending
#define MESSAGE_SEPARATOR (char)0xFE // separates fields in the message
#define MESSAGE_GROUP_USER_SEPARATOR (char)0xFD // separates users in group
#define MESSAGE_USER_SEPARATOR (char)0xFC // separate user fields
#define MESSAGE_QUEUE_LENGTH 1024

enum MessageType
{
	// s = server
	// c = client
	// | - begining end ending of message
	// # - MESSAGE_SEPARATOR
	// % - MESSAGE_GROUP_USER_SEPARATOR
	// $ - MESSAGE_USER_SEPARATOR
	MESSAGE_TYPE_NOTSET,
	LOGIN, // add user to list on server, c->s, |LOGIN#USER| ==> |LOGIN#LOGIN$IP| 
	LOGOUT, // disconnectiong from server, c->s, |LOGOUT#USER|
	RESULT, // result of performed operation, s->c, |RESULT#OPERATION#OPERATION RESULT#ADDITIONAL INFO|
	USERLIST, // list of users, s->c, |USERLIST#USERS GROUP#| ==> |USERLIST#USER1%USER2%...%USERN|
	MESSAGE, // simple text message, c->s->c, |MESSAGE#FROM USER#TO USER|
	GROUPMESSAGE, // simple text message for group of recipients, c->s->c1, c2, c3, |MESSAGE#FROM USER#USERS GROUP|
};

std::string ToString(MessageType command);
MessageType ToMessageType(std::string s);
std::string ToString(Result command);
Result ToResult(std::string s);

//HANDLE InputMutex;
//InputMutex = CreateMutex(0, false, 0);
//CloseHandle(InputMutex);


struct User : public Serializable{
	std::string Login;
	std::string IP;
	User(std::string login, std::string ip);
	User();
	~User();
	virtual std::string ToString();
	virtual Result Parse(std::string &object);
	bool operator==(const User & u2) const;

};

struct Group: public Serializable{
	std::list<User > GroupMembers;
	virtual std::string ToString();
	virtual Result Parse(std::string &object);
	bool operator==(const Group & u1) const;
};

struct Message: public Serializable {
	MessageType Type;
	User Sender;
	Group Recipients;
	std::string Text;
	Message(MessageType type, User sender, Group recipients, std::string text);
	Message();
	virtual std::string ToString();
	virtual Result Parse(std::string &object);
};

#endif
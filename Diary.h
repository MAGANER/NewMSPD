#pragma once
#include<string>
using namespace std;

struct DiaryPage
{
	string body, topic, date;
	DiaryPage(const string& body,
			  const string& topic,
			  const string& date)
	{
		this->body = body;
		this->topic= topic;
		this->date = date;
	}
	~DiaryPage() {}
};
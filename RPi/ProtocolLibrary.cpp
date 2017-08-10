#include <cstring>
#include <iostream>
#include <cmath>
#include "ProtocolLibrary.h"

using std::to_string;
using std::cout;
using std::endl;

bool ProtocolLibrary::createMessage(char* out, string command, string value, bool request)
{
	string header = ProtocolLibrary::createHeader(request, command, false, 0);
	if (header.length() + 18 + value.length() >= ML){
		return false;
	}
		
	string message = header + "message::" + value + "::message";

	strncpy(out, message.c_str(), message.length());
	out[message.length()] = 0;
	return true;
}

//creates a header string
//header looks like this:
//"header::type:(request/answer),command: commandValue[, parted: yes, partnr: 00, parts: 00]::header"
string ProtocolLibrary::createHeader(bool request, string command, bool parted, int parts, int partnr)
{
	string header = "header::";
	if (request)
		header += "type: request,";
	else
		header += "type: answer,";
	header += "command: " + command;
	if (parted){
		header += ",parted: yes,";
		header += "partnr: " + to_string(partnr);
		header += ",parts: " + to_string(parts);
	}
	header += "::header";
	
	return header;
}

int ProtocolLibrary::createSplittedMessage(char* out, string command, string value, bool request, int parts, int part)
{
	string header;
	int overhead;
	if (parts == 0)
	{
		auto estimatedParts = (int) ceil((double)value.length() / (double)(ML - (18 + 76)));
		header = ProtocolLibrary::createHeader(request, command, true, estimatedParts, part);
		overhead = (int) header.length() + 18;
		parts = (int) ceil((double)value.length() / (double)(ML-overhead));
		cout << "createSplittedMessage: " << parts << endl;
	}
	header = ProtocolLibrary::createHeader(request, command, true, parts, part);
	overhead = (int) header.length() + 18;
	string valuePart;
	if ((int) value.length() < (ML- overhead - 1))
		valuePart = value;
	else
		valuePart = value.substr(0, (unsigned long) (ML - overhead - 1));
	string message = header + "message::" + valuePart + "::message";

	cout << message << endl;
	strncpy(out, message.c_str(), message.length());
	out[message.length()] = 0;
	return (int) valuePart.length();
}

//extracts information from package to struct Message and tests for transfer failures
ProtocolLibrary::Message ProtocolLibrary::extractHeader(string value)
{
    Message mes;
    size_t endPos = value.find("::header");
    string header = value.substr(value.find("header::")+8, endPos-8);

    string headerField;
    size_t pos;
    while((pos = header.find(',')) != string::npos) {
        headerField = header.substr(0, pos);
        if (!ProtocolLibrary::extractField(headerField, &mes)){
            break;
        }
        header = header.substr(pos+1);
    }

    if (!mes.transferFailure && !header.empty()){
        ProtocolLibrary::extractField(header, &mes);
    }
    value = value.substr(endPos + 8);
    try {
        mes.value = value.substr(value.find("message::") + 9, value.find("::message") - 9);
    } catch (...) {
        mes.transferFailure = true;
    }

    return mes;
}
bool ProtocolLibrary::extractField(string headerField, Message *mes){
    size_t fieldPos = headerField.find(':');
    string fieldType = headerField.substr(0, fieldPos);
    string fieldValue = headerField.substr(fieldPos + 2);

    if (fieldType == "type") {
        mes->request = (fieldValue == "request");
    } else if (fieldType == "command") {
        mes->command = fieldValue;
    } else if (fieldType == "parted") {
        mes->parted = (fieldValue == "yes");
    } else if (fieldType == "parts") {
        mes->parts = stoi(fieldValue);
    } else if (fieldType == "partnr") {
        mes->part = stoi(fieldValue);
    } else {
        mes->transferFailure = true;
        return false;
    }

    return true;
}

//"header::type:(request/answer),command: commandValue[, parted: yes, partnr: 00, parts: 00]::header"
void ProtocolLibrary::extractHeaderFieldValue(char *BufferIn, char *Command, char *HeaderFieldType)
{
    int i = 0;
    int startHeader = (int)(strstr(BufferIn, "header::") - BufferIn + 8);
    int endHeader = (int)(strstr(BufferIn, "::header") - BufferIn);
    char fullHeaderFieldType[strlen(HeaderFieldType) + 2];
    strcpy(fullHeaderFieldType, HeaderFieldType);
    strcat(fullHeaderFieldType, ": ");
    char headerField[40];
    while(startHeader < endHeader){
        i = startHeader;
        while (BufferIn[i] != ',' && i < endHeader){i++;};
        memset(&headerField[0], 0, sizeof(headerField));
        strncat(headerField, BufferIn + startHeader,(size_t) (i - startHeader));
        if (strstr(headerField, fullHeaderFieldType) != NULL){
            strncat(Command, headerField + strlen(fullHeaderFieldType) , (size_t)(i - startHeader - strlen(fullHeaderFieldType)));
            return;
        }
        startHeader = i+1;
    }
}

void ProtocolLibrary::extractValue(char *BufferIn, char *Value)
{
    int startValue = (int)(strstr(BufferIn, "message::") - BufferIn + 9);
    int endValue = (int)(strstr(BufferIn, "::message") - BufferIn);

    strncat(Value, BufferIn + startValue, (size_t)(endValue - startValue));

    /*
    while (BufferIn[i] != ':' && i< BUFSIZ-2){i++;};
    int j = i+1;
    while (BufferIn[j] != ':' && j< BUFSIZ-2){j++;};

    int k, l = 0;
    for (k=0, l=i+1; k< BUFSIZ-2, l<j; k++, l++)
    {
        Value[k] = BufferIn[l];
    }
    Value[k] = '\0';
    */
    //str_cut(Value, 0, j-i-1);
}
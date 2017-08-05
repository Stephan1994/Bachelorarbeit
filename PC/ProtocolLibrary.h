#ifndef PROTLIB_H
#define PROTLIB_H

#include <string>
using std::string;

//MessageLength
#define ML 8192

class ProtocolLibrary
{
	public:
		struct Message{
			bool transferFailure{false};
			bool request{true};
			string command;
			string value;
			bool parted;
			int parts;
			int part;
		};
		static Message extractHeader(string value);
		static bool createMessage(char* out, string command, string value, bool request = true);
		static string createHeader(bool request, string command, bool parted, int parts = 0, int partnr = 0);
		static int createSplittedMessage(char* out, string command, string value, bool request, int parts = 0, int part = 0);
        static void extractValue(char *BufferIn, char *Value);
        static void extractHeaderFieldValue(char *BufferIn, char *Command, char *HeaderFieldType = (char *)("command"));
private:
    static bool extractField(string HeaderField, Message *mes);
};

#endif //PROTLIB_H
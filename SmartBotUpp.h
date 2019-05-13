#ifndef _SmartUppBot_SmartBotUpp_h_
#define _SmartUppBot_SmartBotUpp_h_

#include <Core/Core.h>
#include <DiscordUpp/Discord.h>
using namespace Upp;

class SmartBotUpp;
class DiscordModule;

class SmartBotUpp{
	private: 
		Upp::Vector<DiscordModule> AllModules;
		Discord bot;
		
		Upp::String name="";
		Upp::String token="";
		
		void Event(ValueMap payload);
	
	public:
		SmartBotUpp(Upp::String _name, Upp::String _token);
		void Launch();
		void AddModule(DiscordModule &module);
		void DeleteModule(DiscordModule &module);
};


class DiscordModule : Upp::Moveable<DiscordModule>{
	private: 
		Upp::String name="";
		Upp::String prefixe = "";
		void Event(Upp::String message);
	public:
		DiscordModule(Upp::String _name, Upp::String _prefixe );
		
};


#endif

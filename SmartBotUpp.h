#ifndef _SmartUppBot_SmartBotUpp_h_
#define _SmartUppBot_SmartBotUpp_h_

#include <Core/Core.h>
#include <DiscordUpp/Discord.h>
using namespace Upp;

/* 
Project created 14/05/2019
By Clément Hamon 
Lib used to give life to the Smartest bot ever ! (not even joking)
This project have to be used with Ultimate++ FrameWork and required the Core Librairy from it
http://www.ultimatepp.org
Copyright © 1998, 2019 Ultimate++ team
All those sources are contained in "plugin" directory. Refer there for licenses, however all libraries have BSD-compatible license.
Ultimate++ has BSD license:
License : https://www.ultimatepp.org/app$ide$About$en-us.html
Thanks to UPP team !
*/
class SmartBotUpp{
	private: 
		Upp::Vector<void*> AllModules;
		Discord bot;
		
		Upp::String name="";
		Upp::String token="";
		
		void Event(ValueMap payload);
	
	public:
		SmartBotUpp(Upp::String _name, Upp::String _token);
		void Launch();
		void AddModule(void* module);
		void DeleteModule(void* module);
};

class DiscordModule : Upp::Moveable<DiscordModule>{
	public:
		Upp::String name="";
		Upp::String prefix="";
		Upp::Vector<Event<ValueMap>> Events;
	
		virtual void Event(ValueMap payload);
};
#endif

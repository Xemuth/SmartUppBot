#include <Core/Core.h>
#include <SmartUppBot/SmartBotUpp.h>
#include <Discord_Overwatch/Discord_Overwatch.h>
#include <Discord_Minecraft/Discord_Minecraft.h>

/* 
Project created 01/18/2019 
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

using namespace Upp;

CONSOLE_APP_MAIN {
	StdLogSetup(LOG_COUT|LOG_FILE);
	SmartBotUpp mybot("314391413200650250","MzE0MzkxNDEzMjAwNjUwMjUw.XLbeew.4-EvNJLFiPGMVoZ6s7pTnoqEObc");
	
	Discord_Overwatch ow("OverWatch","ow");
	mybot.AddModule(&ow);
	
	Discord_Minecraft mc("Minecraft","mc");
	mybot.AddModule(&mc);
	
	mybot.Launch();
}	

SmartBotUpp::SmartBotUpp(Upp::String _name, Upp::String _token){
	name=_name;
	token=_token;
	bot.SetToken(token);
	bot.SetName(name);
	bot.Start();
}

void SmartBotUpp::Launch(){
	bot.WhenMessageCreated =[&](ValueMap payload){this->Event(payload);};
	bot.Listen();
}

void SmartBotUpp::AddModule(void* module){
	AllModules.Add(module);
}

void SmartBotUpp::DeleteModule(void* module){
	int cpt = 0;
	for (void* &e: AllModules){
		if(&e == &module){
			AllModules.Remove(cpt);
			break;
		}
		cpt++;
	}
}

void SmartBotUpp::Event(ValueMap payload){
	String channel  = payload["d"]["channel_id"];
    String content  = payload["d"]["content"];
    String userName = payload["d"]["author"]["username"];
    String id = payload["d"]["author"]["id"];
    String discriminator = payload["d"]["author"]["discriminator"];
    
    for(auto &e : AllModules){
     	((DiscordModule*) e)->Event(payload);
    }

}

void DiscordModule::Event(ValueMap payload){
}



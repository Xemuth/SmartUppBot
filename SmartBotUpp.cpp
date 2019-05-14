#include <Core/Core.h>
#include <SmartUppBot/SmartBotUpp.h>

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

using namespace Upp;

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

void SmartBotUpp::AddModule(DiscordModule* module){
	module->ptrBot = getBotPtr();
	AllModules.Add(module);
}

Discord* SmartBotUpp::getBotPtr(){
	return &bot;	
}

void SmartBotUpp::DeleteModule(DiscordModule* module){
	int cpt = 0;
	for (DiscordModule* &e: AllModules){
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
    
    String prefixe =~payload["d"]["content"];
    try{
	    prefixe = (prefixe.Find(" ",0)>0)? prefixe.Left(prefixe.Find(" ",0)) :  prefixe;
	    Cout() << prefixe <<"\n";
	    prefixe.Replace("!","");
	    for(auto &e : AllModules){
	       	if(((DiscordModule*) e)->goodPrefix(prefixe))
	     		((DiscordModule*) e)->Event(payload);
	    }
    }catch(...){
     	bot.CreateMessage(channel, "Commande inconnue !");
    }
}

void DiscordModule::Event(ValueMap payload){
}

bool DiscordModule::goodPrefix(Upp::String prefixToTest){
	return !ToUpperAscii(prefix).Compare(ToUpper(prefixToTest));
}


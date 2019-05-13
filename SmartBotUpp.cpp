#include <Core/Core.h>

#include <SmartUppBot/SmartBotUpp.h>
#include <SmartUppBot/sql_lite.h>
#include <RconManager/RconManager.h>
#include <chrono>

#ifdef _DEBUG
#include <Sql/sch_schema.h>
#endif

#include <Sql/sch_source.h>

using namespace Upp;

CONSOLE_APP_MAIN {
	StdLogSetup(LOG_COUT|LOG_FILE);
	
	SmartBotUpp mybot("314391413200650250","MzE0MzkxNDEzMjAwNjUwMjUw.XLbeew.4-EvNJLFiPGMVoZ6s7pTnoqEObc");
	DiscordModule ow("Overwatch");
	mybot.AddModule(ow);
	
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

void SmartBotUpp::AddModule(DiscordModule &module){
	AllModules.Add(module);
}

void SmartBotUpp::DeleteModule(DiscordModule &module){
	int cpt = 0;
	for (DiscordModule &e: AllModules){
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
    
    Cout() << "Event : " << channel << "\n";
    
}

void DiscordModule::Event(Upp::String message){
	Cout() <<"Event trigerred"<<"\n";
}	
DiscordModule::DiscordModule(Upp::String _name, Upp::String _prefixe ){
	name= _name;
	prefixe =_namme;
}


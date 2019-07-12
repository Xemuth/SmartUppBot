#include <Core/Core.h>
#include <SmartUppBot/SmartBotUpp.h>
#include <GraphBuilder/GraphBuilder.h>
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
	
	if((~payload["d"]["content"])[0]  == '!' && !(~payload["d"]["author"]["id"]).IsEqual(name)){
		bool resolved =false;
	    Vector<String> command;
	    if ((~payload["d"]["content"]).Find(";") != -1){
	    	command = Split((~payload["d"]["content"]),";");
	    }else{
	        command.Add((~payload["d"]["content"]));
	    }
	    for(String &s : command){
	        if(s.GetCount()> 0){
		        String content =s;
		        String prefixe =s;
		        try{
			    prefixe = (prefixe.Find(" ",0)>0)? prefixe.Left(prefixe.Find(" ",0)) :  prefixe;
			    prefixe.Replace("!","");
			    
			    for(auto &e : AllModules){
			       	if(((DiscordModule*) e)->goodPrefix(prefixe)){
			       		((DiscordModule*) e)->ClearMessageArgs();
						((DiscordModule*) e)->SetChannelLastMessage( payload["d"]["channel_id"]); //HEre we hook chan  
						((DiscordModule*) e)->SetAuthorId(payload["d"]["author"]["id"]);
						content.Replace(String("!" +prefixe +" "),"");
						((DiscordModule*) e)->SetMessage(content);
						if(content.Find(" ") !=-1){
							((DiscordModule*) e)->SetMessageArgs( Split(content," "));
						}
						else{
							((DiscordModule*) e)->SetMessageArgs(Vector<String>{content});
						}
						((DiscordModule*) e)->EventsMessageCreated(payload);
						resolved =true;
						break;
			       	}
				}
			    }catch(...){
			        resolved =true;
			     	bot.CreateMessage(payload["d"]["channel_id"], s + " : Commande inconnue !");
			    }
			    if(prefixe.IsEqual("delete")){
			        content.Replace(String("!" + prefixe +" "),"");
		        	if(content.Find(" ") !=-1){
						auto buff = Split(content," ");
						int deletion = atoi(buff[1]);
						if(deletion > 10 ){
							deletion =10;
							bot.CreateMessage(payload["d"]["channel_id"], "Tststs tu ne peux supprimer que 10 messages par 10 messages !");
						}
						if(isStringisANumber(buff[1])) bot.BulkDeleteMessages(payload["d"]["channel_id"],atoi(buff[1]));
						else bot.BulkDeleteMessages(payload["d"]["channel_id"],2);
					}
					else{
						bot.BulkDeleteMessages(payload["d"]["channel_id"],2);
					}
			    	resolved =true;
			    	bot.CreateMessage(payload["d"]["channel_id"], "Supression effectuée !");
			    }
			    if(!resolved) bot.CreateMessage(payload["d"]["channel_id"], s + " : Commande inconnue !");
	        }
		}
	}
}
   


void DiscordModule::EventsMessageCreated(ValueMap payload){
}

void DiscordModule::SetChannelLastMessage(Upp::String _ChannelLastMessage){ChannelLastMessage = _ChannelLastMessage;}
void DiscordModule::SetAuthorId(Upp::String _AuthorId){AuthorId =_AuthorId;}
void DiscordModule::SetMessage(Upp::String _Message){Message = _Message;}
void DiscordModule::SetMessageArgs(const Upp::Vector<String>& _Args){
	MessageArgs.Clear();
	MessageArgs.Append(_Args);
	MessageArgs[0] =ToLower(MessageArgs[0]);
}
void DiscordModule::ClearMessageArgs(){
	MessageArgs.Clear();
}

bool DiscordModule::goodPrefix(Upp::String prefixToTest){
	return !ToUpperAscii(prefix).Compare(ToUpper(prefixToTest)); //Must be override if you want disable prefix checking and set event to a single prefix commande
}


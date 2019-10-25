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

String htmlEntitiesDecode(String s) {
	auto toGet = Vector<String>
	{"&amp;","&#039;","&apos;","&lt;","&gt;","&ccedil;","&ecirc;","&eacute;","&quot;","&agrave;","&icirc;","&ocirc;"};
	auto toTransform = Vector<String>{"&","'","\"","<",">","ç","ê","é","\"","à","î","ô"};
	return Replace(s,toGet,toTransform);
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
	for(;;){
		bot.Listen();
	}
}

Discord& SmartBotUpp::getBot(){
	return bot;	
}

void SmartBotUpp::DeleteModule(DiscordModule& module){
	int cpt = 0;
	for (DiscordModule &e: AllModules){
		if(&e == &module){
			AllModules.Remove(cpt);
			break;
		}
		cpt++;
	}
}
void SmartBotUpp::trace(){
	bot.Trace();
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
		        
		        String Function = "";
		        Vector<String> Args;
		        
		        try{
				    prefixe = (prefixe.Find(" ",0)>0)? prefixe.Left(prefixe.Find(" ",0)) :  prefixe;
				    prefixe.Replace("!","");
				    for(auto &e : AllModules){
				       	if(e.goodPrefix(prefixe)){
				       		e.ClearMessageArgs();
							e.SetChannelLastMessage( payload["d"]["channel_id"]); //HEre we hook chan  
							e.SetAuthorId(payload["d"]["author"]["id"]);
							e.SetMessage(content);
							content.Replace(String("!" +prefixe +" "),"");
							if(content.Find("(") == -1 || content.Find(")") == -1) break;
							e.SetNameOfFunction(TrimBoth(content.Left(content.Find("("))));
							
							content.Replace(content.Left(content.Find("(")),"");
							content = Replace(content,Vector<String>{"(",")"},Vector<String>{"",""});
							if(content.Find(",") !=-1){
									e.SetMessageArgs(  Split(content,",")  );	
							}else if( TrimBoth(content).GetCount()>0){
								e.SetMessageArgs(Vector<String>{content});
							}
							e.ShowInformation();
							e.EventsMessageCreated(payload);
							resolved =true;
						//	break; Finalement plusieurs modules peuvent eventuellement répondre
				       	}
					}
					if(!resolved){
						if(Function.GetCount() == 0) Function = TrimBoth(content.Left(content.Find("(")));
						Function.Replace("!","");
						if(content.Find(",") !=-1){
							if(Args.GetCount() ==0) Args.Append( Split(content,","));	
						}else if( TrimBoth(content).GetCount()>0){
							
							if(Args.GetCount() ==0) Args.Append(Vector<String>{content});
						}
						if(ToLower(Function).IsEqual("modules")){
							String modules = "";
							bool first = false;
							for(auto &e : AllModules){
								modules += ((first)? ", ":"")+ e.name;
								first = true;	
							}
							bot.CreateMessage(payload["d"]["channel_id"], "SmartUppBotV2 Les modules actuelles sont : " +modules);
							resolved =true;
						}
					}
			    }catch(...){
			        resolved =true;
			     	
			    }
			    /*
				prefixe = TrimBoth(prefixe.Left(prefixe.Find("(")));
			    Cout() << prefixe <<"\n";
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
			    }else if(prefixe.IsEqual("chuck")){
			    	HttpRequest reqApi("https://chucknorrisfacts.fr/api/get?data=nb:1;type:txt;tri:alea");
					reqApi.GET();
					ValueMap json = ParseJSON(reqApi.Execute());
					if(~json[0]["fact"].GetCount())
						bot.CreateMessage(payload["d"]["channel_id"],htmlEntitiesDecode(~json[0]["fact"]) );
					else 
						bot.CreateMessage(payload["d"]["channel_id"], "C'est chuck qui décide...");
					resolved =true;
			    }
			    */
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
	for(String &str : MessageArgs){
		str=TrimBoth(str);	
	}
}
void DiscordModule::SetNameOfFunction(String functionName){NameOfFunction =ToLower(functionName);}

void DiscordModule::ClearMessageArgs(){
	MessageArgs.Clear();
}

void DiscordModule::ShowInformation(){
	String info ="";
	info << "Channel : " << ChannelLastMessage <<"\n";
	info << "Author ID : " << AuthorId <<"\n";
 	info << "Message : " << Message <<"\n";
 	info << "Name of Function : " << NameOfFunction <<"\n";
	info << " Args : ";
		for(String &t : MessageArgs){
			info << t <<" ";	
		}
	info <<"\n";
	Cout() << info <<"\n";
}

void DiscordModule::SetBotPtr(Discord& discord){
	BotPtr = &discord;
}
Discord* DiscordModule::GetBotPtr(){
	return BotPtr;
}

void DiscordModule::Help(ValueMap json){
	BotPtr->CreateMessage(ChannelLastMessage, "This module have not implemented Help function yet !");
}

bool DiscordModule::goodPrefix(Upp::String prefixToTest){
	return !ToUpperAscii(prefix).Compare(ToUpper(prefixToTest)); //Must be override if you want disable prefix checking and set event to a single prefix commande
}


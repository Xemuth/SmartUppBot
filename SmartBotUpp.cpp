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
	    	command = Split((~payload["d"]["content"]),"|"); //TODO : séparateur | ou ||
	    }else{
	        command.Add((~payload["d"]["content"]));
	    }
	    for(String &s : command){
	        if(s.GetCount()> 0){
		        String content =s;
		        
		        //Setting prefix
		        String prefixe =s;
		        prefixe = (prefixe.Find(" ",0)>0)? prefixe.Left(prefixe.Find(" ",0)) :  prefixe;
				prefixe.Replace("!","");
		        
		        String Function = "";
		        Vector<String> Args;
		        
		        
		        
		        
		        try{
				    prefixe = (prefixe.Find(" ",0)>0)? prefixe.Left(prefixe.Find(" ",0)) :  prefixe;
				    prefixe.Replace("!","");
				    for(auto &e : AllModules){
				        // If prefixe match a module
				       	if(e.goodPrefix(prefixe)){
				       		e.ClearMessageArgs();
							e.SetChannelLastMessage( payload["d"]["channel_id"]); 						// Set channel
							e.SetAuthorId(payload["d"]["author"]["id"]); 								// Set user Discord ID
							e.SetMessage(content); 														// Set message
							content.Replace(String("!" +prefixe +" "),"");								// Remove "!<prefixe>" from message
							
							if(content.Find("(") == -1 || content.Find(")") == -1) break;				// break if no parenthesis
							e.SetNameOfFunction(TrimBoth(content.Left(content.Find("("))));				// Set name of function
							
							content.Replace(content.Left(content.Find("(")),"");						// Remove name of function from message
							content = Replace(content,Vector<String>{"(",")"},Vector<String>{"",""});	// Remove parenthesis from message
							
							// Setting args
							if(content.Find(";") !=-1){
									//e.SetMessageArgs(  Split(content,",")  );
									e.SetArgsTest(  Split(content,";")  );
							}else if( TrimBoth(content).GetCount()>0){
								//e.SetMessageArgs(Vector<String>{content});
								e.SetArgsTest(Vector<String>{content});
							}
							e.ShowInformation();														// Show message info in console
							e.EventsMessageCreated(payload);											// Call child event
							resolved =true;
						//	break; Finalement plusieurs modules peuvent eventuellement répondre
				       	}
					}
					
					// If the prefix didn't match any module
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
							bot.CreateMessage(payload["d"]["channel_id"],"```"+ version+"\nLes modules actuelles sont : " +modules +"```");
							resolved =true;
						}else if(ToLower(Function).IsEqual("help")){
							String help = "```";
							help <<  version+"\n\n";
							help << "!Credit() "<<" -> Affiche les crédit de SmartUppBot.\n";
							help << "!Modules()"<<" -> Affiche les modules actuellement chargé par SmartUppBot.\n\n";
							help << "Pour obtenir l'aide des différents modules utilisez les commandes suivantes : " << "\n";
							for(auto &e : AllModules){
								help << "!" << e.prefix << " Help()" << "\n";
							}
							help << "\nPour afficher les crédits des différents modules utilisez les commandes suivantes : " << "\n";
							for(auto &e : AllModules){
								help << "!" << e.prefix << " Credit()" << "\n";
							}
							help << "```";
							bot.CreateMessage(payload["d"]["channel_id"], help);
							resolved =true;
						}else if(ToLower(Function).IsEqual("credit")){
							String credit = "```";
							credit <<  version+"\n\n";
							credit << "SmartUppBot Copyright (C) 2019 Clément Hamon\n\n";
							credit << "Lib used to give life to the Smartest discord bot ever ! (not even joking)\n";
							credit << "This project have to be used with Ultimate++ FrameWork and required the Core Library from it\n";
							credit << "http://www.ultimatepp.org\n";
							credit << "Copyright © 1998, 2019 Ultimate++ team\n";
							credit << "All those sources are contained in 'plugin' directory. Refer there for licenses, however all libraries have BSD-compatible license.\n";
							credit << "Ultimate++ has BSD license:\n";
							credit << "License : https://www.ultimatepp.org/app$ide$About$en-us.html\n";
							credit << "Thanks to UPP team !\n";
							credit << "Special thanks to jjacksonRIAB !\n\n";
							for(auto &e : AllModules){
								credit << "Credit of " + e.name + " module :\n";
								credit << e.Credit(payload,false) <<"\n\n";
							}
							credit << "```";
							bot.CreateMessage(payload["d"]["channel_id"], credit);
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

void DiscordModule::SetArgsTest(const Upp::Vector<String>& _Args){
	//ArgsTest.Clear();
	

	for(String arg : _Args){
		Cout() << "ARGUMENTS : " << arg << "; ";
		
	}
}

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
	BotPtr->CreateMessage(ChannelLastMessage, "```This module have not implemented Help function yet !```");
}

String DiscordModule::Credit(ValueMap json,bool sendCredit){
	String credit =  "This module have been made by a stanger !";
	if(sendCredit) BotPtr->CreateMessage(ChannelLastMessage,"```"+credit +"```");
	return credit;
}

bool DiscordModule::goodPrefix(Upp::String prefixToTest){
	return !ToUpperAscii(prefix).Compare(ToUpper(prefixToTest)); //Must be override if you want disable prefix checking and set event to a single prefix commande
}

//checking if string could be a number
bool DiscordModule::IsANumber(Upp::String stringNumber){
	if (std::isdigit(stringNumber[0]) || (stringNumber.GetCount() > 1 && (stringNumber[0] == '+'))){
        for (int i = 1 ; i < stringNumber.GetCount(); ++i)
            if (!std::isdigit(stringNumber[i]))
                return false;
        return true;
    }
    return false;
}

//Function to allow inheritance of type from a string
Value DiscordModule::ResolveType(String valueToResolve){
    if(valueToResolve.GetCount()> 0 && DiscordModule::IsANumber(valueToResolve)){
        if(valueToResolve.GetCount() > 9){
            return Value(std::stoi(valueToResolve.ToStd()));
        }else if(valueToResolve.Find(",") || valueToResolve.Find(".")){
            return Value(std::stoi(valueToResolve.ToStd()));
        }else{
            return Value(std::stoi(valueToResolve.ToStd()));
        }
    }else if(valueToResolve.GetCount()> 0 && ((valueToResolve[0] == 'b' && DiscordModule::IsANumber(valueToResolve.Right(valueToResolve.GetCount()-1))) || (ToLower(valueToResolve).IsEqual("true") || ToLower(valueToResolve).IsEqual("false")))  ){
        if(valueToResolve.Find("b")>-1 && DiscordModule::IsANumber(valueToResolve.Right(valueToResolve.GetCount()-1)) ){
            valueToResolve.Replace("b","");
            return Value(((std::stoi(valueToResolve.ToStd())!=0)? true:false));
        }else if(valueToResolve.IsEqual("true") || valueToResolve.IsEqual("false")){
            return Value(((valueToResolve.IsEqual("true"))? true:false));
        }
    }
    return Value(valueToResolve);

}


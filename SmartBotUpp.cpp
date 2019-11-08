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
	    if ((~payload["d"]["content"]).Find(";") != -1){// TODO : use config file
	    	command = Split((~payload["d"]["content"]),"|"); // TODO : use config file
	    }else{
	        command.Add((~payload["d"]["content"]));
	    }
	    for(String &s : command){
	        if(s.GetCount()> 0){
		        String content = s;
		        String prefixe =s;
		        String Function;
		        
		        Vector<String> Args;
		        VectorMap<String, Value> NamedArgs;
		        
		        // TODO : use config file
		        content.Replace("!", "");
		        
		        //  1 - Chercher les args
		        
		        // Setting args
		        if(content.Find('(') != -1){
		            // There are args
		            String mess = content;
		            // Get everything inside parenthesis
		            String argsString = mess.Mid(mess.Find('(') + 1, mess.ReverseFind(')') - mess.Find('(') - 1);
		            
		            //Removed args from content
		            content.Replace('(' + argsString + ')',"");
		            
		            // TODO : use config file
		           	Args.Append(Split(argsString, ';'));
		           	
		           	for (String& arg : Args){
		           		if (arg.Find(':') != -1){
		           			// Named args
		           			// TODO : use config file
		           			auto mySplit = Split(arg, ':');
		           			String key = ToLower(TrimBoth(mySplit[0]));
		           			String value = ToLower(TrimBoth(mySplit[1]));
		           			
		           			NamedArgs.Add(key, DiscordModule::ResolveType(value));
		           		}
		           		else{
		           			// Go commit no live
		           		}
		           	}
		        }
		        
		        // 2 - Chercher la fonction & prefix
		        if (content.Find(" ") != -1){
		            // Prefix + Func name
		            auto mySplit = Split(content, " ");
		            
		            Function = ToLower(TrimBoth(mySplit[0]));
		            prefixe = ToLower(TrimBoth(mySplit[1]));           
		        }
		        else{
		            // No prefix
		            Function = ToLower(TrimBoth(content));
		        }
		        
		        // Reset content to full command
		        content = s;
		        
		        try{
				    for(auto &e : AllModules){
				        // If prefixe match a module
				       	if(e.goodPrefix(prefixe)){
				       		e.ClearMessageArgs();
							e.SetChannelLastMessage( payload["d"]["channel_id"]); 						// Set channel
							e.SetAuthorId(payload["d"]["author"]["id"]); 								// Set user Discord ID
							e.SetMessage(content); 														// Set message
	      					e.SetNameOfFunction(Function);												// Set name of function
	      					
	      					if (NamedArgs.GetCount() > 0){
	      						e.SetMessageArgs(NamedArgs);
	      					}
	      					
							e.ShowInformation();														// Show message info in console
							e.EventsMessageCreated(payload);											// Call child event
							resolved = true;
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
void DiscordModule::SetMessageArgs(const Upp::VectorMap<String, Value>& _Args){
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
	info << " Args : \n";
		for(String &key : MessageArgs.GetKeys()){
			info << key <<" -> " << MessageArgs[key] << "\n";	
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
	String credit =  "This module have been made by a stranger !";
	if(sendCredit) BotPtr->CreateMessage(ChannelLastMessage,"```"+credit +"```");
	return credit;
}


String& DiscordModule::AddPrefix(String _prefix){
	_prefix = ToLower(_prefix);
	for(String& str : prefix){
		if(ToLower(str).IsEqual(_prefix))
			return str;
	}
	return prefix.Add(_prefix);
}

bool DiscordModule::AddPrefix(Vector<String>& _prefix){
	bool b =false;
	for(String& str0 : _prefix){
		b =false;
		for(String& str : prefix){
			if(ToLower(str).IsEqual(ToLower(str0)))
				b =true;
		}
		if(!b)
			prefix.Add(ToLower(str0));
	}
	return b;
}

bool DiscordModule::RemovePrefix(String _prefix){
	int cpt = 0;
	for(String& str : prefix){
		if(ToLower(str).IsEqual(_prefix)){
			prefix.Remove(cpt);
			return true;
		}
		cpt++;
	}
	return false;
}
		
		
bool DiscordModule::goodPrefix(Upp::String prefixToTest){
	prefixToTest = ToLower(prefixToTest);
	for(String& str : prefix){
		if( ToLower(str).IsEqual(prefixToTest) || str.GetCount() ==0) //Ici soit le prefix correspond au prefixes du module soit le module possède un prefix vide et donc est enclanché
			return true;
	}
	return false;
	
	//return !ToUpperAscii(prefix).Compare(ToUpper(prefixToTest)); //Must be override if you want disable prefix checking and set event to a single prefix commande
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


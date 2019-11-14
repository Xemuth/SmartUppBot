#ifndef _SmartUppBot_SmartBotUpp_h_
#define _SmartUppBot_SmartBotUpp_h_

#include <Core/Core.h>
#include "Discord.h"
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
class DiscordModule;

class SmartUppBotException : public std::exception { //classe to managed every exception
	private:
	    int m_numero;               //Id of Error
	    Upp::String m_phrase;       //Error summaries
	    int m_niveau;               //level of Error
	    char* myChar=NULL;
	public:
		
	    SmartUppBotException(int numero=0, Upp::String phrase="", int niveau=0){
	        m_numero = numero;
	        m_phrase = phrase;
	        m_niveau = niveau;
			myChar =  new char[m_phrase.GetCount()+1];
	        strcpy(myChar,this->m_phrase.ToStd().c_str());
	    }
	    
	    virtual const char* what() const throw() {
			return  (const char *)  myChar;
	    }
	    int getNiveau() const throw(){
			return m_niveau;
	    }
		virtual ~SmartUppBotException(){
			delete [] myChar;
		}
};

class SmartBotUpp{
	private: 
		Upp::Array<DiscordModule> AllModules;
		Discord bot;
		
		Upp::String name="";
		Upp::String token="";
		
		Upp::String version = " ---------------- SmartUppBot -- V2.0.1 -- 25/10/2019 ----------------";
		
		void Event(ValueMap payload);
		Discord& getBot();
	public:
		
		SmartBotUpp(Upp::String _name, Upp::String _token);
		
		void trace();
		void Launch();
		
		void DeleteModule(DiscordModule& module);
		template <class T,class... Args>
		T& CreateModule(Args&&... args){
			T& module = AllModules.Create<T>(std::forward<Args>(args)...);
			if(dynamic_cast<DiscordModule*>(&module)){
				module.SetBotPtr(bot);
				return module;
			}
			else{
				AllModules.Remove(AllModules.GetCount()-1,1);
				throw SmartUppBotException(6,"T& SmartBotUpp::CreateModule<type T>(...) => Invalide type not inherited from DiscordModule !",1);
			}
		}	
};

class DiscordModule{
	protected:
		String ChannelLastMessage=""; //hook to latest message chan 
		String NameOfFunction="";
		VectorMap<String, Value> MessageArgs;
		
	    String AuthorId =""; 
	    String Message ="";
	    
	    Discord* BotPtr;
	public:
		virtual void Help(ValueMap json);
	    virtual String Credit(ValueMap json,bool sendCredit = true);
		
		void SetChannelLastMessage(Upp::String _ChannelLastMessage);
		void ShowInformation();
		
		void SetAuthorId(Upp::String _AuthorId);
		void SetMessage(Upp::String _Message);
		
		void SetMessageArgs(const Upp::VectorMap<String, Value>& _Args);
		void SetNameOfFunction(Upp::String functionName);
		
		void SetBotPtr(Discord& discord);
		Discord* GetBotPtr();
		
		void ClearMessageArgs();
		
		Upp::String name="";
		//Upp::String prefix="";
		Vector<Upp::String> prefix;
		
		
		
		Upp::Array<Event<ValueMap>> EventsMapMessageCreated{ 
			[&](ValueMap e){if(NameOfFunction.IsEqual("help"))Help(e);},
			[&](ValueMap e){if(NameOfFunction.IsEqual("credit"))Credit(e);}
		};
		
		
		String& AddPrefix(String _prefix);
		bool AddPrefix(Vector<String>& _prefix);
		bool RemovePrefix(String _prefix);
	
		virtual bool goodPrefix(Upp::String prefixToTest);
		virtual void EventsMessageCreated(ValueMap payload);
		
		static Value ResolveType(String valueToResolve);
		static bool IsANumber(String stringNumber);
		static int Levensthein_Distance(const String& s1, const String& s2);
		static Date TransformStringToEuropeanDate(const String& dateToTransform);
};



#endif

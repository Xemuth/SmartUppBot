#ifndef _SmartUppBot_overwatch_h_
#define _SmartUppBot_overwatch_h_
#include <SmartUppBot/SmartBotUpp.h>

class smartUppBot_Overwatch() : public DiscordModule{
	private:
		const Upp::String prefixe = "OW";
		Upp::Vector<Event<>> events;
		
		
	public:
		
		void CallOverwatch();
		
		smartUppBot_Overwatch();
		bool isPrefix
		
	
	
	
};

#endif

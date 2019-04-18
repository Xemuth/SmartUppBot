#include <Core/Core.h>
#include <DiscordUpp/Discord.h>

using namespace Upp;


CONSOLE_APP_MAIN {

    Discord bot("314391413200650250","MzE0MzkxNDEzMjAwNjUwMjUw.XLbeew.4-EvNJLFiPGMVoZ6s7pTnoqEObc");
    
    bot.WhenMessageCreated = [&](ValueMap payload) {
        Cout() << payload["d"]<<"\n";
  		String channel  = payload["d"]["channel_id"];
        String content  = payload["d"]["content"];
        String userName = payload["d"]["author"]["username"];
        String id = payload["d"]["author"]["id"];
        String discriminator = payload["d"]["author"]["discriminator"];
        Cout() << "Contente : " << content<< "\n";
	        if(content.Find("314391413200650250")!=-1){
	            bot.CreateMessage(channel, String("<@"+ id + "> tu es un idiot !") );
	        }
	        else if(userName.Compare("Bot_Hendek")!=0){
	        	bot.CreateMessage(channel, String("Je suis vivant !") );
	        }
 	    };	
    
   
    bot.Listen();
}
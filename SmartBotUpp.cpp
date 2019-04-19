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
        
        ValueMap json;
        
        Cout() << "Content : " << content<< "\n";
	        /*if(content.Find("314391413200650250")!=-1){
	            bot.CreateMessage(channel, String("<@"+ id + "> tu es un idiot !") );
	        }
	        else if(userName.Compare("Bot_Hendek")!=0){
	        	bot.CreateMessage(channel, String("Je suis vivant !") );
	        }*/
	        
	        if (content.Find("!stats")!=-1){
	        	content.Remove(0, 7);
	        	String battletag = content;
	        	
	        	
	        	
	        	if (userName.Find("Xemuth")!=-1){
	        		bot.CreateMessage(channel, "Je répond pas à toi");
	        		return;
	        	}
	        	else if(userName.Find("Natty")!=-1){
	        		bot.CreateMessage(channel, "Bonjour Maître");
	        	}
	        	
	        	if (battletag.Find("BASTION") != -1){
	        		bot.CreateMessage(channel, "Recherche de stats pour l'autre salope");
	        	}
	        	else{
					bot.CreateMessage(channel, "Recherche de stats pour " + battletag);
	        	}
				HttpRequest query("https://ow-api.com/v1/stats/pc/eu/" + battletag + "/profile");
				query.GET();
				query.Timeout(5000);
				
				json = ParseJSON(query.Execute());
				
				Cout() << json["rating"] << " -> rating \n";
				
				Cout() << ~json["rating"]<< "\n";
				
				if (double(json["rating"]) < 3000){
				}
				else{
					bot.CreateMessage(channel, "");
				}
	        }
	        
	        bot.CreateMessage(channel,~json["rating"]);
 	    };
    
   
    bot.Listen();
}
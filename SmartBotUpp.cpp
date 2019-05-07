#include <Core/Core.h>
#include <DiscordUpp/Discord.h>
#include <SmartUppBot/sql_lite.h>

#ifdef _DEBUG
#include <Sql/sch_schema.h>
#endif

#include <Sql/sch_source.h>

using namespace Upp;

//Je te la synchro auto
CONSOLE_APP_MAIN {

    Discord bot("314391413200650250","MzE0MzkxNDEzMjAwNjUwMjUw.XLbeew.4-EvNJLFiPGMVoZ6s7pTnoqEObc");
    
    bot.WhenMessageCreated = [&](ValueMap payload) {
        Cout() << payload["d"]<<"\n";
  		String channel  = payload["d"]["channel_id"];
        String content  = payload["d"]["content"];
        String userName = payload["d"]["author"]["username"];
        String id = payload["d"]["author"]["id"];
        String discriminator = payload["d"]["author"]["discriminator"];
        
        String battletag;
        ValueMap json;
        
        //Cout() << "Content : " << content<< "\n";
	        
	        //Affichage des stats d'un BattleTag
	        if (content.Find("!stats")!=-1){
	        	content.Remove(0, 7);
	        	battletag = content;
	       		
			  	bot.CreateMessage(channel, "Recherche de stats pour " + battletag);
	        		        	
				HttpRequest query("https://ow-api.com/v1/stats/pc/eu/" + battletag + "/profile");
				query.GET();
				query.Timeout(10000);
				
				json = ParseJSON(query.Execute());
	        	bot.CreateMessage(channel,json["rating"]);
	       
	        }//MAJ des stats d'un BattleTag
	        else if(content.Find("!upd")!=-1){
 	      		content.Remove(0, 5);
 	      		battletag = content;
 	      		
 	      		//bot.CreateMessage(channel, "Mise à jours des stats pour " + battletag);
	        	
	        	HttpRequest query("https://ow-api.com/v1/stats/pc/eu/" + battletag + "/profile");
				query.GET();
				query.Timeout(5000);
				
				json = ParseJSON(query.Execute());
	        	
	        	Sqlite3Session sqlite3;
				sqlite3.LogErrors(true);
				if(!sqlite3.Open(ConfigFile("simple.db"))) {
					Cout() << "Can't create or open database file\n";
					return;
				}
				
				SQL = sqlite3;
				
				#ifdef _DEBUG
					SqlSchema sch(SQLITE3);
					All_Tables(sch);
					if(sch.ScriptChanged(SqlSchema::UPGRADE))
						SqlPerformScript(sch.Upgrade());
					if(sch.ScriptChanged(SqlSchema::ATTRIBUTES)) {
						SqlPerformScript(sch.Attributes());
					}
					if(sch.ScriptChanged(SqlSchema::CONFIG)) {
						SqlPerformScript(sch.ConfigDrop());
						SqlPerformScript(sch.Config());
					}
					sch.SaveNormal();
				#endif
				
				S_SESSION rowSession;
				Sql sqlInsert;
				
				String rang = ~json["rating"];
				
				sqlInsert*Insert(SESSION)(BATTLETAG, battletag)(RANK, rang)(DATE_SESSION, Date());
				
				/*
				sql*Select(rowCompte).From(SESSION).Where(BATTLETAG == battletag);
				Cout() << sql.ToString() << "\n";
				
				if (!sql.Fetch()){
					Cout() << "Compte " + battletag + " introuvable, création necessaire.\n";
					sql*Insert(SESSION)(BATTLETAG, battletag)(RANK, ~json["rating"])(DATE_SESSION, Date());
				}
				else{
					Cout() << "Compte " + battletag + " recuperer, mise à jours.\n";
						
					bot.CreateMessage(channel, "Le rang actuel de " + ~sql[1] + " est de " + ~sql[2]);
				}
				*/
				Sql sqlSelect;
				
				sqlSelect*Select(rowSession).From(SESSION).Where(BATTLETAG == battletag);
				
				bot.CreateMessage(channel, "Mise à jours de " + ~sqlSelect[1] + " : ELO " + ~sqlSelect[2]);
				
				
				while(sqlSelect.Fetch()){
					Cout() << ~sqlSelect[0] + " : BT=" + ~sqlSelect[1] + " ; RANK=" + ~sqlSelect[2] + " at " + ~sqlSelect[3] + "\n";
				}
 	    	}/*
	        else if(content.Find(1, '!')!=-1){
	            bot.CreateMessage(channel, "Commande introuvable");
	        }*/
 	    };
 	    
    bot.Listen();
}
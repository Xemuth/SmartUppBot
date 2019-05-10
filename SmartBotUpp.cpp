#include <Core/Core.h>
#include <DiscordUpp/Discord.h>
#include <SmartUppBot/sql_lite.h>
#include <RconManager/RconManager.h>
#include <chrono>

#ifdef _DEBUG
#include <Sql/sch_schema.h>
#endif

#include <Sql/sch_source.h>

using namespace Upp;

CONSOLE_APP_MAIN {
	RconManager rcon("51.254.91.2", 27066,"respect","minecraft");
    Discord bot("314391413200650250","MzE0MzkxNDEzMjAwNjUwMjUw.XLbeew.4-EvNJLFiPGMVoZ6s7pTnoqEObc");
    
    Cout() << "Debut programme\n";
        
        // ----------- Instantiation SQL LITE 3 --------------------
        Sqlite3Session sqlite3;
		sqlite3.LogErrors(true);
		if(!sqlite3.Open(ConfigFile("simple.db"))) {
			Cout() << "Can't create or open database file\n";
			return;
		}
		
		SQL = sqlite3;
		
		Cout() << "Fichier .db ouvert\n";
		
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
    
    bot.WhenMessageCreated = [&](ValueMap payload) {
        Cout() << payload["d"]<<"\n";
  		String channel  = payload["d"]["channel_id"];
        String content  = payload["d"]["content"];
        String userName = payload["d"]["author"]["username"];
        String id = payload["d"]["author"]["id"];
        String discriminator = payload["d"]["author"]["discriminator"];
        
        String battletag;
        ValueMap json;
        
        
        Cout() << "Debut traitement \n";
	        
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
       
        }else if (content.Find("!minecraft")!=-1 && id == "131915014419382272"){
	        	content.Remove(0, 11);
	        	String command = content;
       		
		  		bot.CreateMessage(channel, "Execution de la commande " + command +" sur le serveur minecraft");
        		rcon.SendCommand(command);
				bot.CreateMessage(channel, "Commande envoyée !");
       
        }//------------------ MAJ des stats d'un BattleTag ---------------------- 
        else if(content.Find("!upd")!=-1){
            S_SESSION rowSession;
            	            
            if (content.GetLength() == 4){ //Commande = !upd
 	      		Sql sqlGetBattletag;
 	      		
 	      		//------------------ Recherche du battletag sur la bdd ----------------------
 	      		sqlGetBattletag*Select(rowSession).From(SESSION).Where(DISCORD_NAME == userName);
				
				if (sqlGetBattletag.Fetch()){
					Cout() << "Battletag trouvé\n";
					battletag = ~sqlGetBattletag[1];
				}
				else{
					Cout() << "Battletag non trouvé\n";
					bot.CreateMessage(channel, "Aucun compte BattleNet n'est associé à votre nom discord, pour l'ajouter merci d'utiliser la commande '!upd [BattleTag]' en remplacant le '#' par un '-'.");
					return;
				}
            }
           //------------------ MAJ des stats
            else{//Commande = !upd NattyRoots-21691
                content.Remove(0, 5);
                battletag = content;
            }
      		//-------------------------- REQUETE API OW ------------------------------	
      		Cout() << "Requete API OW\n";
      		        	
        	HttpRequest query("https://ow-api.com/v1/stats/pc/eu/" + battletag + "/profile");
			query.GET();
			query.Timeout(5000);
			
			json = ParseJSON(query.Execute());
        	
			String rang = ~json["rating"];
			
			//----------------- INSERTION SESSION ---------------------------------------
			Cout() << "Insertion nouvel ligne (" + battletag + ", " + userName + ", " + rang + ")\n";

			std::chrono::milliseconds ms = std::chrono::duration_cast< std::chrono::milliseconds >(
			    std::chrono::system_clock::now().time_since_epoch()
			);
			
			Sql sqlInsert;
			sqlInsert*Insert(SESSION)(BATTLETAG, battletag)(DISCORD_NAME, userName)(RANK, rang)(DATE_SESSION, ms.count());

			Sql sqlSelect;
			
			sqlSelect*Select(rowSession).From(SESSION).Where(DISCORD_NAME == userName).OrderBy(Descending(DATE_SESSION));
			
			bot.CreateMessage(channel, "Mise à jour de " + ~sqlSelect[1] + " : ELO " + ~sqlSelect[3]);
			
			while(sqlSelect.Fetch()){
				Cout() << ~sqlSelect[0] + " : BT = " + ~sqlSelect[1] + " ; DISCORD = " + ~sqlSelect[2] + " ; ELO = " + ~sqlSelect[3] + /*" at " + ~sqlSelect[4] + */"\n";
			}
			
    	}
    	else if(content.Find("!leaderboard") != -1){
    		Sql sqlSelect;
    		S_SESSION rowSession;
    		
    		sqlSelect*Select(Distinct(rowSession.BATTLETAG)).From(SESSION);
    		
    		int i = 0;
    		while (sqlSelect.Fetch()){
    			bot.CreateMessage(channel, i + " : " + ~sqlSelect[0]);
    		}
    	}/*
        else if(content.Find(1, '!')!=-1){
            bot.CreateMessage(channel, "Commande introuvable");
        }*/
        
    };
 	    
    bot.Listen();
}
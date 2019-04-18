#include <Core/Core.h>
#include <fstream>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <random>
#include <chrono>
#include <DiscordUpp/Discord.h>

using namespace Upp;

const int colMot = 1;
const int coltype =4;



class Mot : Upp::Moveable<Mot>{	
	public :
	String mot;
	String type;	
	
	Mot(String _mot,String _type){
		mot = _mot;
		type = _type;
	}
};
 int random(Vector<Mot> &mesMots){
	
	return (int)(Randomf() * mesMots.GetCount() -0)+0;
}

Vector<Mot> mesMots;

String getFromLine(String line,int col){
	return Split(line,"\t")[col-1];
}


String selectAleatoire(String typeToGet){
	int rnd = random(mesMots) ;
	for(int i = rnd; i << mesMots.GetCount();i++){
		Cout() << mesMots[i].mot << " , " << mesMots[i].type <<"\n";
		if(mesMots[i].type.Compare(typeToGet) ==0){
			return mesMots[i].mot;	
		}
	}
	return "";
}


CONSOLE_APP_MAIN {
	SYSTEMTIME systime;
	GetLocalTime(&systime);
    SeedRandom( systime.wSecond);
    
	std::ifstream infile("C:\\Users\\Clement Hamon\\Downloads\\Lexique382.tsv");
	std::string line;
	std::getline(infile, line);
	while (std::getline(infile, line))
	{
		mesMots.Add(Mot(getFromLine( String(line),colMot), getFromLine(String(line),coltype)));
	    // process pair (a,b)
	}


	Cout() << "Fin :  " << mesMots[  random(mesMots)  ].mot <<"\n";
	
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
	            bot.CreateMessage(channel, String("<@"+ id + "> tu es un " + selectAleatoire("ADJ") +" !") );
	        }
	        else if(userName.Compare("Bot_Hendek")!=0){
	        	bot.CreateMessage(channel, String( selectAleatoire("NOM") + " "+ selectAleatoire("VER") + " " + selectAleatoire("ADJ") +" !") );
	        }
 	    };	
    
   
    bot.Listen();
}
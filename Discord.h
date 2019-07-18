#ifndef _Discord_Discord_h_
#define _Discord_Discord_h_


/*
This code as been write by jjacksonRIAB 
https://www.ultimatepp.org/forums/index.php?t=usrinfo&id=1150&

Here is his original post about sharing the code : https://www.ultimatepp.org/forums/index.php?t=msg&goto=51566&&srch=Discord#msg_51566
Here it his Github : https://github.com/BornTactical/DiscordUpp


Special thanks to him !
*/


#include <Core/Core.h>
#include <atomic>

namespace Upp {
    enum  OP {
        DISPATCH,
        HEARTBEAT,
        IDENTIFY,
        STATUS_UPDATE,
        VOICE_STATUS_UPDATE,
        VOICE_SERVER_PING,
        RESUME,
        RECONNECT,
        REQUEST_GUILD_MEMBERS,
        INVALID_SESSION,
        HELLO,
        HEARTBEAT_ACK
    };
    
    class Discord {
        String gatewayAddr = "https://gateway.discord.gg";
        String baseUrl     = "https://discordapp.com";
        String token                  { Null  };
        String name                   { Null  };
        Value  sessionID              { Null  };
        Value  lastSequenceNum        { Null  };
        int    heartbeatInterval      { 10000 };
        std::atomic<bool> keepRunning { true  };
        bool   shouldResume           { false };
        
        // rate limiting
        void ApplyRateLimits(HttpRequest& req);
            
        WebSocket   ws;
        HttpRequest req;
        
        void Resume();
        void ObtainGatewayAddress();
        void RepeatSendHeartbeat(unsigned int ms, std::atomic<bool>& keepRunning);
        void Identify();
        void Reconnect(ValueMap payload);
        void Hello(ValueMap payload);
        void HeartbeatAck(ValueMap payload) {
           // LOG(payload);
        }
        void Dispatch(ValueMap payload);
        void InvalidSession(ValueMap payload);
        
    public:
        Event<ValueMap> WhenReady,
            WhenError,
            WhenGuildStatusChanged,
            WhenGuildCreated,
            WhenChannelCreated,
            WhenVoiceChannelCreated,
            WhenVoiceChannelSelected,
            WhenVoiceStateUpdated,
            WhenVoiceStateDeleted,
            WhenVoiceSettingsUpdated,
            WhenVoiceConnectionStatusChanged,
            WhenSpeakingStarted,
            WhenSpeakingStopped,
            WhenMessageCreated,
            WhenMessageUpdated,
            WhenMessageDeleted,
            WhenNotificationCreated,
            WhenCaptureShortcutChanged,
            WhenActivityJoined,
            WhenActivitySpectated,
            WhenActivityJoinRequested;
        
        Event<> BeforeSocketReceive;
        
        int GetMessages(String channel, int count, JsonArray& messageIDs);
        void SendFile(String channel, String message, String title, String fileName);
        void BulkDeleteMessages(String channel, int count);
        void CreateMessage(String channel, String message);
        void Listen();
        
        void SetName(Upp::String _name);
	    void SetToken(Upp::String _token);
	    void Start();
        void Trace();
        Discord();
        Discord(String configFile);
        Discord(String name, String token);
        ~Discord();
        
        typedef Discord CLASSNAME;
    };
}
#endif

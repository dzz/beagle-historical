import client.system.audio as audio
class music_system:
    def __init__(self,music_file = None ):
        audio.get_track("FX").play_clip(audio.get_clip("audio/level_up.ogg"))
        pass
    
    def trigger_event(self,event_name):
        if event_name == "level_up":
            audio.get_track("FX").play_clip(audio.get_clip("audio/level_up.ogg"))

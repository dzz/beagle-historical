import client.system.audio as audio
class music_system:
    def __init__(self,music_file = None ):

        #prime the cache so we don't block when starting to play

        audio.get_clip("audio/Tram01_DrumStatic.ogg")
        audio.get_clip("audio/Tram01_DrumEffects.ogg")
        audio.get_clip("audio/Tram01_Bass.ogg")
        audio.get_clip("audio/level_up.ogg")

        audio.get_track("DrumStatic").play_clip(audio.get_clip("audio/Tram01_DrumStatic.ogg"), True)
        audio.get_track("DrumEffects").play_clip(audio.get_clip("audio/Tram01_DrumEffects.ogg"), True).set_volume(0)
        audio.get_track("Bass").play_clip(audio.get_clip("audio/Tram01_Bass.ogg"), True).set_volume(1)
    

    def track_volume(self,track,vol):
        audio.get_track(track).set_volume(vol)

    def trigger_event(self,event_name):
        if event_name == "level_up":
            audio.get_track("game_effects_0").play_clip(audio.get_clip("audio/level_up.ogg"))

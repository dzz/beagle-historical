import client.system.audio as audio
class music_system:
    def __init__(self,music_file = None ):

        self.effect_queue = []
        self.beat = 0
        #prime the cache so we don't block when starting to play

        print("-====LOADING MUSIC====-")
        #audio.get_clip("audio/Tram01_DrumStatic.ogg")
        #audio.get_clip("audio/Tram01_DrumEffects.ogg")
        #audio.get_clip("audio/Tram01_Bass.ogg")
        #audio.get_clip("audio/Tram01_Pads.ogg")
        #audio.get_clip("audio/Tram01_Leads.ogg")
        #audio.get_clip("audio/Tram01_PickupHi.ogg")
        #audio.get_clip("audio/Tram01_PickupLow.ogg")
        #audio.get_clip("audio/level_up.ogg")

        audio.get_track("DrumStatic", 128.0, True).play_clip(audio.get_clip("audio/Tram01_DrumStatic.ogg"), True)
        audio.get_track("DrumEffects",128.0, True).play_clip(audio.get_clip("audio/Tram01_DrumEffects.ogg"), True).set_volume(0)
        audio.get_track("Bass",       128.0, True).play_clip(audio.get_clip("audio/Tram01_Bass.ogg"), True).set_volume(1)
        audio.get_track("Pads",       128.0, True).play_clip(audio.get_clip("audio/Tram01_Pads.ogg"), True).set_volume(0)
        audio.get_track("Lead",       128.0, True).play_clip(audio.get_clip("audio/Tram01_Leads.ogg"), True).set_volume(1.0)
        audio.get_track("PickupHi",   128.0, True).play_clip(audio.get_clip("audio/Tram01_PickupHi.ogg"), True).set_volume(0.0)
        audio.get_track("PickupLow",  128.0, True).play_clip(audio.get_clip("audio/Tram01_PickupLow.ogg"), True).set_volume(0.0)
    

    def track_volume(self,track,vol):
        audio.get_track(track).set_volume(vol)

    def track_pan(self,track,pan):
        audio.get_track(track).set_pan(pan)

    def trigger_event(self,event_name):
        if event_name == "level_up":
            audio.get_track("game_effects_0").play_clip(audio.get_clip("audio/level_up.ogg"))

    def tick(self):
        pass

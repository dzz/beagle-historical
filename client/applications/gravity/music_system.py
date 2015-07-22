import client.system.audio as audio
from random import choice
class music_system:
    def __init__(self,music_file = None ):


        audio.get_track("DrumStatic", 32.0, True).play_clip(audio.get_clip("audio/Tram01_DrumStatic.ogg"), True)
        audio.get_track("DrumEffects",128.0, True).play_clip(audio.get_clip("audio/Tram01_DrumEffects.ogg"), True).set_volume(0)
        audio.get_track("Bass",       128.0, True).play_clip(audio.get_clip("audio/Tram01_Bass.ogg"), True).set_volume(1)
        audio.get_track("Pads",       64.0, True).play_clip(audio.get_clip("audio/Tram01_Pads.ogg"), True).set_volume(0)
        audio.get_track("Lead",       256.0, True).play_clip(audio.get_clip("audio/level_up.ogg"), True).set_volume(1.0)
        audio.get_track("PickupHi",   128.0, True).play_clip(audio.get_clip("audio/Tram01_PickupHi.ogg"), True).set_volume(0.0)
        audio.get_track("PickupLow",  64.0, True).play_clip(audio.get_clip("audio/Tram01_PickupLow.ogg"), True).set_volume(0.0)

        audio.get_track("game_effects_0",128.0, True)
    

    def track_volume(self,track,vol):
        audio.get_track(track).set_volume(vol)

    def track_pan(self,track,pan):
        audio.get_track(track).set_pan(pan)

    def trigger_event(self,event_name):
        if event_name == "level_up":
           track_name = choice(["Bass", "Lead","Pads", "Lead", "PickupLow","DrumEffects"])
           audio.get_track(track_name).retrigger()

           if choice([False,True]):
               track_a = audio.get_track(choice(["PickupHi","Pads","PickupLow", "DrumEffects","Leads"]))
               track_b = audio.get_track(choice(["PickupHi","Pads","PickupLow", "DrumEffects","Leads"]))
               tmp = track_a.active_clip
               track_a.active_clip = track_b.active_clip
               track_b.active_clip = tmp



    def tick(self):
        pass

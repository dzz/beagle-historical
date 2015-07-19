import client.ctt2.host_config as host_config
import audio

tracks = {}
clips = {}

class clip:
    def __init__(self,filename):
        self.audio_clip = audio.clip_create(host_config.get_config("app_dir") + filename)

    def __del__(self):
        audio.clip_drop(self.audio_clip)

class track:
    def __init__(self):
        self.audio_track = audio.track_create()

    def __del__(self):
        audio.track_drop(self.audio_track)

    def play_clip(self,clip,loop = False):
        print("python side audioclip id:{0}".format(clip.audio_clip))
        if(loop):
            audio.track_play_clip( self.audio_track, clip.audio_clip, 1)
        else:
            audio.track_play_clip( self.audio_track, clip.audio_clip, 0)

    def set_pan(self,pan):
        audio.track_set_pan(self.audio_track, pan)

    def set_volume(self,v):
        audio.track_set_volume(self.audio_track, v)


def get_track(name):
    global tracks
    if not name in tracks:
        tracks[name] = track()
    return tracks[name]
    

def get_clip(name):
    global clips
    if not name in clips:
        clips[name] = clip(name)
    return clips[name]

def flush_clips():
    global clips
    clips = []


def reset_tracks():
    global tracks
    tracks = {}
    audio.track_reset_all()
    


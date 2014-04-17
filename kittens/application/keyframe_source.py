class clip_source(object):
    def __init__(self):
        pass

class keyframe_source(clip_source):
    def __init__(self):
        self.keyframes = {} 

    def add_keyframe(frame, keyframe):
        self.keyframes[frame] = keyframe

    def rem_keyframe(frame):
        if frame in self.keyframes.keys:
            del self.keyframes[frame]

    def get_frame(frame):
        if len(self.keyframes) == 0:
            return None
        if len(self.keyframes) ==1:
            return self.keyframes.items()[0][1]

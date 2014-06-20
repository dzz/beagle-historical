import client.hockeyquest.template_content as template_content
import random

def pick(l):
    r = random.random()*len(l)
    return l[ int(r) ]

class _player:

    Ailing = False
    Dead = False
    Location = "Hotel"
    Class = "Goon"
    Status = "Welcome to Hockey Quest"
    Story = []
    Age = 0
    _mins_since_prev_update = 0

    TimeVitals = {
                "Hunger"    : [0, 60*6, [ "You could eat {{pronoun_large_eatable_animal}}.", "You could eat just about anything.", "You are hungry.", "You feel a rumbling in the pit of your stomach.", "You {{must}} eat soon.", "You're feeling your health draining from skipping meals" ]],
                "Tiredness" : [0, 60*16, ["You are tired.", "Your eyelids feel heavy.", "You {{must}} sleep soon.", "You're feeling your {{health}} draining from not resting.",
                                            "Why haven't you slept yet?", "You are plagued by hallucinations", "You find yourself slipping out of consciousness",
                                            ]]
             }

    ComputedStatVitals = { 
                    #stat : value : good_thresh : bad_thresh : wellmsgs[] : okaymsgs[] : losemsgs[]
                    "HP" : [ 85, 75, 50 , 

                    ["You're feeling {{healthy_synonyms}}.", "You feel {{positive_adverb}}." "People on the street are envious of your physique", "Your physical condition is {{excellent}}" ],
                    ["Your body feels ok.", "You're not in the best shape, but not in the worst.","You're not setting any records for physical fitness."],
                    ["You're not feeling {{healthy_synonyms}} lately.", "You don't feel too healthy.","You're not taking care of yourself."]
                 ],
                    "PS" : [ 85, 75, 60,

                    ["You've been performing well.","Keep up the good job on the ice.","You're doing great."],
                    ["Maybe you should try a little harder.","In general, you're doing ok.", "You're not blowing anybody's mind." ],
                    ["Sometimes you wonder if hockey is really for you.", "You feel unhappy about your overall performance.","You feel like you just don't have it some days."]
                 ]
          }


    def _post_update(x):
        x._computeStatus()
        x._computeStory()

    def tick(x, delta_mins):
        commit_status_mins = 72
        x._mins_since_prev_update += delta_mins
        if x._do_health(delta_mins):
            for k in x.TimeVitals:
                tv = x.TimeVitals[k]
                tv[0]+=delta_mins
            x.Age += delta_mins
            x._do_playerscore(delta_mins)
            if( x._mins_since_prev_update > commit_status_mins):
                if( random.random() > 0.5 ): #noise
                    x._mins_since_prev_update = 0
                    x._post_update()


    def _do_playerscore(x,delta_mins):
        ail_delta_mins = 24*60

        if x.Ailing:
            x.ComputedStatVitals["PS"][0] -= 15 / ail_delta_mins

    def _do_health(x,delta_mins):
        if(x.Dead):
            return False

        delta_t = 60*24 # injury from computed time vitals spread over 60*x minutes
        computed_tv_injure = 0

        for k in x.TimeVitals:
            tv = x.TimeVitals[k]
            if(tv[0]>tv[1]):
                if(computed_tv_injure==0):
                    print("computing tv injury")
                    computed_tv_injure=1
                else:
                    computed_tv_injure+=((tv[1]-tv[0])/16)
                    computed_tv_injure*=1 + (computed_tv_injure/16)

        x.ComputedStatVitals["HP"][0]-= computed_tv_injure / delta_t
        print(x.ComputedStatVitals["HP"])

        ##edge case for death
        if(x.ComputedStatVitals["HP"][0] <= 0):
            x.Status = "You are dead."
            x._computeStory()
            x.Dead = True

        hp = x.ComputedStatVitals["HP"]
        if(hp[0]<hp[2]):
            x.Ailing = True

        return (x.Dead == False)

    def signalDayChange(x,day):
        x.Story.append( "\n*** {0} ***\n--------------\n".format(day))

    def _computeStory(x):
        x.Story.append( "{1}\n".format(x.Age, x.Status) )

    def _computeStatus(x):
        msgs =[]
        for k in x.TimeVitals:
            tv = x.TimeVitals[k]
            if tv[0] > tv[1]:
                msgs.append(pick(tv[2]))

        for k in x.ComputedStatVitals:
            csv = x.ComputedStatVitals[k]
            if csv[0]>csv[1]:
                msgs.append(pick(csv[3]))
            elif csv[0]<csv[2]:
                msgs.append(pick(csv[5]))
            else:
                msgs.append(pick(csv[4]))

        idx = int(random.random()*len(msgs))
        x.Status = template_content.parse( msgs[idx] )
        
player = _player()

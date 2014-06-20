import random

def pick(l):
    r = random.random()*len(l)
    return l[ int(r) ]

content = {

        "{{pronoun_large_eatable_animal}}" :
        [ "a dog", "a horse", "an iguana", "a cougar","a sasquatch","a mammoth",
            "a giant squid", "a rhinocerous", "an alien", "a siberian tiger",
            "an entire collection of led zeppelin records" ],
        "{{positive_health_descriptors}}" :
        [ "your best", "very well", "particularily decent","like you're awesome",
            "like you're doing the right things", "so good" ],
        "{{positive_adverb}}" :
        [ "awesome", "great", "sweet", "fantastic", "wonderful" ],
        "{{healthy_synonyms}}" :
        [ "healthy", "well", "in good physical condition", "physically sound" ],
        "{{must}}" :
        [ "must", "got to", "have to", "had better", "need to" ],
        "{{health}}" :
        [ "constitution", "health", "body" ],
        "{{excellent}}" :
        [ "execelent","fantastic","awesome","outstanding","pretty great",
            "wonderful" ]
}

def parse( msg ):
    for key in content:
        item = content[key]
        msg = msg.replace(key, pick(content[key]))
    return msg

# tilemap
```python
from client.gfx.tilemap import tilemap
```
Tilemaps are used to draw a collection of gids. They manage their own tilesets, and can be loaded from files or constructed dynamically.

```python
#from a Tiled json exported file:
tm = tilemap.from_json_file( "json/mymap.json", img_path = "tileimages/", filtered=False ):

#at runtime
configuration = { 
    "tileheight":16,
    "layers":[
    {
        "width":3,
        "height":3,
        "data":[ 1,1,1,1,0,1,1,1,1 ] #these are gids, see tileset
    }, 
    ],
    "tilesets":[    #each item in this array is the same as you would pass to a tileset constructor
    {
        "firstgid":1,
        "image":"mytiles.png",
        "imageheight":526,
        "imagewidth":968,
        "margin":0,
        "name":"roguelikeSheet_transparent",
        "tileproperties": { },
        "properties" : { },
        "spacing":1,
        "tileheight":16,
        "tilewidth":16
    }]
}

tm = tilemap( configuration, img_path, filtered=False )
```

#rendering a tilemap

```python

tm.render( x = int(camera[0]), y = int(camera[1]), scale = 2, False)
```

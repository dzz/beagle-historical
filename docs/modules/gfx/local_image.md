# local_image

```python
from client.gfx.local_image import local_image
```

a local_image represents a bitmap in system (non-gpu) memory. Local images ares used to load texture. A local_image would also be a suitable target downloading something you render on the gpu onto. See the texture class for more details.

# usage

```python
from client.gfx.local_image import local_image

my_image = local_image.from_file("dogs.png")
blank_image = local_image.from_dims( 1920,1080)
```

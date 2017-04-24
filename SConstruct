#######################3
# BUILD NOTES
#
# needs sdl2, sdl_mixer, sdl_image, python-devel
#

import fnmatch
import os
import sys


build_debug_mode = True

def populate_c_matches(matches, dir):
    for root, dirnames, filenames in os.walk(dir):
        for filename in fnmatch.filter(filenames, '*.c'):
            matches.append(os.path.join(root, filename))

def populate_h_matches(matches, dir):
    for root, dirnames, filenames in os.walk(dir):
        for filename in fnmatch.filter(filenames, '*.h'):
            path = os.dirname(os.path.join( root, filename))
            if not path in matches:
                matches.append(path)

def configure_portaudio(includes,libs):
    includes.append("/usr/include/portaudio/")
    libs.extend(['libportaudio', 'asound'])

def configure_python(includes, libs):
    includes.append("/usr/include/python3.5/")
    libs.extend( [ 'python3.5m', 'pthread', 'dl', 'util', 'm' ] )
    
def configure_sdl(includes,libs):
    includes.append("/usr/include/SDL2/")
    libs.extend( ['SDL2', 'SDL_image', 'SDLmain' ] )

def configure_opengl(includes,libs):
    libs.extend( ['GL'])
    includes.append("./gl_includes/")

Libs            = []
SourceFiles     = []
IncludePaths    = []

populate_c_matches(SourceFiles, "src")
populate_h_matches(IncludePaths, "DSPFilters/source")

configure_opengl(IncludePaths, Libs)
configure_portaudio(IncludePaths, Libs) # first because python also needs pthead
configure_python(IncludePaths, Libs)
configure_sdl(IncludePaths, Libs)

if build_debug_mode:
    print SourceFiles
    print IncludePaths
    print Libs

env = Environment()

if sys.platform == "linux2":
    CCFLAGS="-pthread -Wno-incompatible-pointer-types"
if sys.platform == "win32":
    ##CXXFLAGS="/EHsc /O2 /Ot /arch:AVX"
    CCFLAGS+""
    #env.Append(CXXFLAGS = ['/DEBUG'])


env.Program(
        './beagle_runtime', 
        SourceFiles, 
        CPPPATH= IncludePaths,
        LIBPATH=['.','./lib/'],
        LIBS=Libs,
        CCFLAGS=CCFLAGS
)


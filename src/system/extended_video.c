#include <GL/glew.h>

#include "extended_video.h"

void initExtendedVideo() {
    glewExperimental = GL_TRUE;
    glewInit();
}

void dropExtendedVideo() {
}

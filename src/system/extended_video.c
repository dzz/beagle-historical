#include <GL/glew.h>

#include "extended_video.h"
#include "../hwgfx/label.h"

void initExtendedVideo() {
    glewExperimental = GL_TRUE;
    glewInit();
    initLabels();
}

void dropExtendedVideo() {
    dropLabels();
}

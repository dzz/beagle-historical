#version 330
in vec2 uv;

void main(void) {
    gl_FragColor = vec4( uv.x,uv.y,uv.x*uv.y,1.0);
}

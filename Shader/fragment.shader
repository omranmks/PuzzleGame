#version 330

in vec4 vColor;
in vec2 texCoord;

out vec4 color;

uniform sampler2D theTexture;

uniform int toggle;

uniform vec4 fColor;

void main(){
    if(toggle == 0){
        color = texture(theTexture, texCoord);
    }
    else if (toggle == 1) {
        color = fColor; 
    }
    else if (toggle == 2) {
        color = texture(theTexture, texCoord) * fColor; 
    }

}
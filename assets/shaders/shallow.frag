uniform sampler2D texture; // Player's texture
uniform float splitY;      // Y-coordinate for the split
uniform vec2 offset;       // Offset for the bottom half

void main()
{
    vec2 uv = gl_TexCoord[0].xy; // Texture coordinates
    vec4 color = texture2D(texture, uv);

    // If the fragment is above the split line, render normally
    if (gl_FragCoord.y < splitY) {
        gl_FragColor = color;
    }
    // If the fragment is below the split line, apply offset
    else {
        vec2 offsetUV = vec2(uv.x, uv.y - offset.y);
        gl_FragColor = texture2D(texture, offsetUV);
    }
}

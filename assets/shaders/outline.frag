// Outline Shader with "L"-shaped Corners
uniform sampler2D texture; // The object's texture
uniform vec2 textureSize; // The size of the texture in pixels
uniform vec4 cornerColor; // The color of the corners
uniform float cornerSize; // Size of the corners in pixels
uniform float frameThickness; // Thickness of the "L" corner outlines

void main() {
    // Normalized texture coordinates (0.0 to 1.0)
    vec2 uv = gl_TexCoord[0].xy;

    // Convert to pixel coordinates
    vec2 pixelCoords = uv * textureSize;

    // Calculate distances to edges
    float left = pixelCoords.x;
    float right = textureSize.x - pixelCoords.x;
    float top = pixelCoords.y;
    float bottom = textureSize.y - pixelCoords.y;

    // Define the "L"-shaped corners
    bool isTopLeftCorner = (left < cornerSize && top < frameThickness) || (top < cornerSize && left < frameThickness);
    bool isTopRightCorner = (right < cornerSize && top < frameThickness) || (top < cornerSize && right < frameThickness);
    bool isBottomLeftCorner = (left < cornerSize && bottom < frameThickness) || (bottom < cornerSize && left < frameThickness);
    bool isBottomRightCorner = (right < cornerSize && bottom < frameThickness) || (bottom < cornerSize && right < frameThickness);

    // Determine if the pixel is in any of the "L" corners
    bool isInLCorner = isTopLeftCorner || isTopRightCorner || isBottomLeftCorner || isBottomRightCorner;

    // Base color from the texture
    vec4 baseColor = texture2D(texture, uv);

    // Apply corner color if in an "L" corner, otherwise use the base color
    gl_FragColor = isInLCorner ? cornerColor : baseColor;
}

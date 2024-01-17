// ====================================================
#version 330 core

// ======================= uniform ====================
// Our light sources
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform float ambientIntensity;
// Used for our specular highlights
uniform mat4 view;
// If we have texture coordinates, they are stored in this sampler.
uniform sampler2D u_DiffuseMap; 

// ======================= IN =========================
in vec3 myNormal; // Import our normal data
in vec2 v_texCoord; // Import our texture coordinates from vertex shader
in vec3 FragPos; // Import the fragment position

// ======================= out ========================
// The final output color of each 'fragment' from our fragment shader.
out vec4 FragColor;

// ======================= Globals ====================
// We will have another constant for specular strength
float specularStrength = 0.5f;

// textures for each of the different heights
uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;

// height value cutoffs for different regions
uniform float height0 = 5.0;
uniform float height1 = 20.0;
uniform float height2 = 90.0;
uniform float height3 = 135.0;

vec4 mixTextureHeights(sampler2D textureOne, sampler2D textureTwo, float startZone, float endZone, float height){
        vec4 ret;

    	vec4 colorOne = texture(textureOne, v_texCoord);
		vec4 colorTwo = texture(textureTwo, v_texCoord);

		float delta = endZone - startZone;

		float factor = (height - startZone)/delta;
        ret = mix(colorOne, colorTwo, factor);

        return ret;
}

vec4 getColor(){
	vec4 mixedTexture;

	float height = FragPos.y;

	if(height < height0){
		mixedTexture = texture(texture0, v_texCoord);
	} else if(height < height1){
        mixedTexture = mixTextureHeights(texture0, texture1, height0, height1, height);
	} else if(height < height2){
        mixedTexture = mixTextureHeights(texture1, texture2, height1, height2, height);
    } else if(height < height3){
        mixedTexture = mixTextureHeights(texture2, texture3, height2, height3, height);
    } else{
        mixedTexture = texture(texture3, v_texCoord);
    }

    return mixedTexture;
}

void main()
{
    // Store our final texture color
    vec3 diffuseColor;
    diffuseColor = getColor().rgb;

    // (1) Compute ambient light
    vec3 ambient = ambientIntensity * lightColor;

    // (2) Compute diffuse light

    // Compute the normal direction
    vec3 norm = normalize(myNormal);
    
    // From our lights position and the fragment, we can get
    // a vector indicating direction
    // Note it is always good to 'normalize' values.
    vec3 lightDir = normalize(lightPos - FragPos);

    // Now we can compute the diffuse light impact
    float diffImpact = max(dot(norm, lightDir), 0.0);
    vec3 diffuseLight = diffImpact * lightColor;

    // (3) Compute Specular lighting
    vec3 viewPos = vec3(0.0,0.0,0.0);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    // Our final color is now based on the texture.
    // That is set by the diffuseColor
    vec3 Lighting = diffuseLight + ambient + specular;

    // Final color + "how dark or light to make fragment"
    if(gl_FrontFacing){
        FragColor = vec4(diffuseColor * Lighting,1.0);
    }else{
        // Additionally color the back side the same color
         FragColor = vec4(diffuseColor * Lighting,1.0);
    }
}
// ==================================================================

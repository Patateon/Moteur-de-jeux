#version 330 core

// Interpolated values from the vertex shaders
in vec2 UV;
in vec3 position_worldspace;
in vec3 normal_worldspace;

// Ouput data
out vec4 fragmentColor;

// Values that stay constant for the whole mesh.
uniform sampler2D texture_Mesh;
uniform vec3 lightPosition_worldspace;
uniform vec3 lightColor;
uniform vec3 cameraPosition;
uniform vec3 color_Mesh;
uniform int hasTexture;

void main(){

	// vec3 lightColor = vec3(1.0, 1.0, 1.0);
	// Ambient
    float ambient_str = 0.7;
    vec3 ambient = ambient_str * lightColor;

    // Diffuse
    float diffuse_str = 0.5;
    vec3 normal = normalize(normal_worldspace);
    vec3 light_dir = normalize(lightPosition_worldspace - position_worldspace);
    float diffuse_k = max(dot(normal, light_dir), 0.0);
    vec3 diffuse = diffuse_str * diffuse_k * lightColor;

    // Specular
    float specular_str = 1.0;
    vec3 view_dir = normalize(cameraPosition - position_worldspace);
    vec3 reflect_dir = reflect(-light_dir, normal);
    float specular_k = pow(max(dot(view_dir, reflect_dir), 0.0), 32);
    vec3 specular = specular_str * specular_k * lightColor;
	
	// Material properties
	vec3 MaterialColor;
	if(hasTexture > 0){
		MaterialColor = texture(texture_Mesh, UV).xyz;
	}else{
		MaterialColor = color_Mesh;
	}

    vec3 final_color = ((ambient + diffuse) * MaterialColor) /* + specular */;

    fragmentColor = vec4(final_color.xyz, 1.0);
}
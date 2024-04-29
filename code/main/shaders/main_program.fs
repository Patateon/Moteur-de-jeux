#version 330 core

// Ouput data
out vec4 fragColor;

in vec3 vertex_ws_vs;
in vec3 normal_vs;
in vec2 texCoord_vs;

uniform vec3 color;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 cameraPos;
uniform int has_texture;

uniform sampler2D myTextureSampler;

vec4 currentColor;

void main(){

    // Ambient
    float ambient_str = 0.7;
    vec3 ambient = ambient_str * lightColor;

    // Diffuse
    float diffuse_str = 1.0;
    vec3 normal = normalize(normal_vs);
    vec3 light_dir = normalize(lightPos - vertex_ws_vs);
    float diffuse_k = max(dot(normal, light_dir), 0.0);
    vec3 diffuse = diffuse_str * diffuse_k * lightColor;

    // Specular
    float specular_str = 1.0;
    vec3 view_dir = normalize(cameraPos - vertex_ws_vs);
    vec3 reflect_dir = reflect(-light_dir, normal);
    float specular_k = pow(max(dot(view_dir, reflect_dir), 0.0), 32);
    vec3 specular = specular_str * specular_k * lightColor;

    if (has_texture == 1){
        currentColor = texture(myTextureSampler, texCoord_vs);
    }else{
        currentColor = vec4(color, 1.0);
    }

    vec3 final_color = ((ambient + diffuse) * currentColor.xyz) + specular;

    fragColor = vec4(final_color.xyz, 1.0);
}

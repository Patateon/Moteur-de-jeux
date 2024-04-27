#ifndef TEXTURE_HPP
#define TEXTURE_HPP


// Texture en niveau de gris
GLuint loadTexture2DGrayFromFilePath(const std::string& path, int *min_value, int *max_value);

// Texture couleur
GLuint loadTexture2DFromFilePath(const std::string& path);

// Setup les paramètres de la texture de base (Interpolation ou non, etc.)
void setDefaultTexture2DParameters(GLuint texture);

// Load a .BMP file using our custom loader
GLuint loadBMP_custom(const char * imagepath);

//GLuint loadTGA_glfw(const char * imagepath);

// Load a .DDS file using GLFW's own loader
GLuint loadDDS(const char * imagepath);


#endif
#ifndef TEXT2D_HPP
#define TEXT2D_HPP


// Il parait que ca fait des texture aussi
void initText2D(const char * texturePath);
void printText2D(const char * text, int x, int y, int size);
void cleanupText2D();

#endif